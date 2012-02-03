;; -*- mode: scheme; coding: utf-8; -*-
#<(sagittarius regex)>
(library (sagittarius cgen stub)
    (export cgen-genstub)
    (import (except (rnrs) define)
	    (rnrs eval)
	    (core base) ;; for print
	    (sagittarius cgen cise)
	    (sagittarius cgen unit)
	    (sagittarius cgen syntax)
	    (sagittarius cgen literal)
	    (sagittarius cgen type)
	    (sagittarius)
	    (sagittarius io)
	    (sagittarius vm)
	    (rename (sagittarius control) (define-with-key define))
	    (sagittarius regex)
	    (srfi :1  lists)
	    (srfi :13 strings)
	    (srfi :26 cut)
	    (srfi :39 parameters)
	    (match)
	    (shorten)
	    (util file)
	    (util port)
	    (clos user)
	    (clos core))

  (define-class <cgen-stub-unit> (<cgen-unit>)
    ((c-name-prefix :init-keyword :c-name-prefix)))

  (define (cgen-genstub stubfile :key (predef-syms '()))
    (unless (file-readable? stubfile)
      (error 'cgen-genstub
	     "Could not read input file" stubfile))
    (let-values (((dir base ext) (decompose-path stubfile)))
      (let1 prefix
	  (cgen-safe-name-friendly (path-sans-extension base))
	(parameterize ((cgen-current-unit
			(make <cgen-stub-unit>
			  :name (path-sans-extension stubfile)
			  :c-name-prefix (string-append prefix "_")
			  :preamble '("/*Generated by genstub. Do not edit.*/")
			  :init-prologue (format "void Sg_Init_~a() {\n" prefix)
			  )))
	  (for-each cgen-define predef-syms)
	  ;;(cgen-include "<sagittarius.h>")
	  (with-input-from-file stubfile
	    (cut port-fold
		 (lambda (form decl-strings?)
		   (cond ((and decl-strings? (string? form))
			  (cgen-decl form) #t)
			 (else (cgen-stub-parse-form form) #f)))
		 #t read))
	  (cgen-emit-c (cgen-current-unit))))))
  ;; utilities
  (define (f fmt . args) (apply format #t fmt args))
  (define (p . args) (apply print args))
  (define (get-c-name prefix scheme-name)
    (cgen-safe-name-friendly (string-append prefix (format "~a" scheme-name))))

  ;; form parsers
  (define-class <form-parser> ()
    ((name    :init-keyword :name)
     (args    :init-keyword :args)
     (handler :init-keyword :handler)))

  (define *instance-pool* '())

  (define-syntax define-form-parser
    (er-macro-transformer
     (lambda (form rename compare)
       (match form
	 ((_ name args . body)
	  (let ((p (gensym)))
	    `(let ((,p (make <form-parser> :name ',name :args ',args
			     :handler (lambda ,args ,@body))))
	       (set! *instance-pool* (append *instance-pool* (list ,p))))))))))

  (define-method invoke ((self <form-parser>) form)
    (define (badform)
      (error 'cgen-genstub 
	     (format "maformed ~a: ~s" (slot-ref self'name) form)))
    (apply (slot-ref self 'handler)
	   (let loop ((llist (slot-ref self 'args))
		      (form (cdr form)))
	     (cond ((null? llist) (if (null? form) '() (badform)))
		   ((pair? llist)
		    (if (null? form)
			(badform)
			(cons (car form) (loop (cdr llist) (cdr form)))))
		   (else form)))))

  (define (cgen-stub-parser key)
    (cond ((find (^p (eq? key (slot-ref p 'name))) *instance-pool*)
	   => (lambda (parser) 
		(cut invoke parser <>)))
	  (else #f)))

  (define (cgen-stub-parse-form form)
    (match form
      ((? string?) (cgen-body form))
      (((= cgen-stub-parser (? values p)) . _) (p form))
      (_ error 'cgen-stub-parse-form "invalid stub form" form)))

  (define-form-parser library (name export import . body)
    (define (library->init name)
      (format "void Sg__Init~a() {~%" 
	      (list->string (map (lambda (c)
				   (case c
				     ((#\( #\space) #\_)
				     ((#\)) #\space) ;; assume it's the end
				     (else c))) (string->list
						 (format "~a" name))))))
    ;; re-init prologue
    (cgen-unit-init-prologue 
     (cgen-current-unit)
     (string-append 
      (library->init name)
      (format
       "  SgLibrary *lib = SG_LIBRARY(Sg_FindLibrary(SG_INTERN(\"~a\"), TRUE));"
       name)))
    (fold (lambda (form decl-strings?)
	    (cond ((and decl-strings? (string? form))
		   (cgen-decl form) #t)
		  (else (cgen-stub-parse-form form) #f)))
	  #t body))

  (define *this-library* (find-library '(sagittarius cgen stub) #f))

  (define-form-parser define-cise-macro args
    (eval `(define-cise-macro ,@args) *this-library*))
  (define-form-parser define-cise-stmt args
    (eval `(define-cise-stmt ,@args) *this-library*))
  (define-form-parser define-cise-expr args
    (eval `(define-cise-expr ,@args) *this-library*))
  (define-form-parser define-cfn args
    (cgen-body (cise-render-to-string `(define-cfn ,@args) 'toplevel)))

  (define-form-parser decl-code codes
    (dolist (c codes)
      (if (string? c)
	  (cgen-decl c)
	  (cgen-decl (call-with-output-string
		       (cut cise-render c 'stmt <>))))))

  (define (name->type name)
    (or (cgen-type-from-name name)
	(error 'stub "unknown stub-type" name)))

  ;; <arg>
  (define-class <arg> ()
    ((name    :init-keyword :name)
     (c-name)
     (scm-name)
     (count   :init-keyword :count)
     (type    :init-keyword :type)
     (default :init-keyword :default :init-value #f)
     (proc    :accessor arg-proc)))

  (define-class <required-arg> (<arg>) ())
  (define-class <optional-arg> (<arg>) ((opt-count :init-keyword :opt-count)))
  (define-class <rest-arg>     (<arg>) ())

  (define-method write-object ((self <arg>) out)
    (format out "#<~a ~a>" (class-of self) (slot-ref self 'name)))

  (define-method initialize ((self <arg>) initargs)
    (call-next-method)
    (slot-set! self 'c-name   (get-c-name "" (slot-ref self 'name)))
    (slot-set! self 'scm-name (string-append (slot-ref self 'c-name) "_scm")))

  (define (get-arg-default arg)
    (cond ((slot-ref arg 'default) => 
	   (lambda (default)
	     ;; assume pair is expression
	     (if (pair? default)
		 (cise-render-to-string default)
		 (cgen-cexpr default))))
	  (else "SG_UNBOUND")))

  ;; literal
  ;;(define-class <special-literal> () ())
  ;;(define-cgen-literal <raw-c-literal> <special-literal>
  ;;()
  ;;(static (self) #t))

  (define (make-literal obj . args)
    (cgen-literal obj))

  (define-form-parser define-type args
    (unless (<= 2 (length args) 6)
      (error 'define-type "malformed define-type" `(define-type . ,args)))
    (apply make-cgen-type args))

  (define *scm-type* (name->type '<top>))

  ;; stub base class of declarations
  (define-class <stub> (<cgen-node>)
    ((scheme-name :init-keyword :scheme-name :init-value #f)
     (c-name      :init-keyword :c-name)))

  (define (get-stubs class)
    (filter (cut is-a? <> class) 
	    (cgen-unit-toplevel-nodes (cgen-current-unit))))

  (define-class <procstub> (<stub>)
    ((args            :init-value () :init-keyword :args)
     (num-reqargs     :init-value 0   :init-keyword :num-reqargs)
     (num-optargs     :init-value 0   :init-keyword :num-optargs)
     (have-rest-args? :init-value #f  :init-keyword :have-rest-arg?)
     (return-type     :init-value #f  :init-keyword :return-type)
     (decls           :init-value ())
     (stmts           :init-value ())
     ;; for future, like :constant or something
     (flags           :init-value () :init-keyword :flags)))

  (define (get-arg cproc arg) (find (^x (eq? arg (slot-ref x 'name)))
				    (slot-ref cproc 'args)))
  (define (push-stmt! cproc stmt)
    (slot-set! cproc 'stmts (append (slot-ref cproc 'stmts) (list stmt))))

  (define-generic c-stub-name)

  ;; define-c-proc
  (define-class <c-proc> (<procstub>)
    ((inline-insn :init-value #f)
     (proc-name   :init-keyword :proc-name)))
  ;; not used
  (define-constant c-proc-flags '(:constant))

  (define-form-parser define-c-proc (scheme-name argspec . body)
    (define (extract-flags body)
      (receive (flags body) (span keyword? body)
	(unless (for-all (cut memq <> c-proc-flags) flags)
	  (error 'define-c-proc "invalid c-proc flag" flags))
	(values flags body)))

    (check-arg symbol? scheme-name 'define-c-proc)
    (check-arg list argspec 'define-c-proc)
    (let*-values (((args nreqs nopts rest?)
		   (process-c-proc-args scheme-name argspec))
		  ((body rettype) (extract-rettype body))
		  ((flags body)   (extract-flags body)))
      (let1 cproc (make <c-proc>
		    :scheme-name scheme-name
		    :c-name (get-c-name (slot-ref (cgen-current-unit)
						  'c-name-prefix)
					scheme-name)
		    :proc-name (make-literal (symbol->string scheme-name))
		    :return-type rettype :flags flags
		    :args args
		    :num-reqargs nreqs
		    :num-optargs nopts
		    :have-rest-arg? rest?)
	;; set arg-proc here
	(for-each (lambda (arg) (arg-proc arg cproc)) args)
	(process-body cproc body)
	(cgen-add! cproc))))

  (define-method c-stub-name ((cproc <c-proc>))
    (format "~a__STUB" (slot-ref cproc 'c-name)))

  (define (make-arg class argname count . rest)
    (define (grok-argname argname)
      (let1 namestr (symbol->string argname)
	(receive (realname typename) (string-scan namestr "::" 'both)
	  (if realname
	      (values (string->symbol realname)
		      (name->type (string->symbol typename)))
	      (values argname *scm-type*)))))
    (receive (arg type) (grok-argname argname)
      (apply make class :name arg :type type :count count rest)))

  (define (process-c-proc-args name argspecs)
    (define (badarg arg)
      (error 'define-c-proc
	     "bad argument in argspec" arg nam))
    (define (required specs args nreqs)
      (match specs
	(()                  (values (reverse args) nreqs 0 #f))
	((:optional . specs) (optional specs args nreqs 0))
	((:rest     . specs) (rest specs args nreqs 0))
	(((? symbol? sym) . specs)
	 (required specs
		   (cons (make-arg <required-arg> sym nreqs) args)
		   (+ nreqs 1)))
	(_ (badarg (car specs)))))
    (define (optional specs args nreqs nopts)
      (match specs
	(()                  (values (reverse args) nreqs nopts #f))
	((:optional . specs)
	 (error 'define-c-proc "extra :optional parameter" name))
	((:rest     . specs) (rest specs args nreqs nopts))
	(((? symbol? sym) . specs)
	 (optional specs
		   (cons (make-arg <optional-arg> sym (+ nreqs nopts)
				   :opt-count nopts)
			 args)
		   nreqs
		   (+ nopts 1)))
	((((? symbol? sym) default) . specs)
	 (optional specs
		   (cons (make-arg <optional-arg> sym (+ nreqs nopts)
				   :opt-count nopts
				   ;; default can be expression
				   ;; so if it's a list assume it's an
				   ;; expression
				   :default (if (pair? default)
						default
						(make-literal default)))
			 args)
		   nreqs
		   (+ nopts 1)))
	(_ (badarg (car specs)))))
    (define (rest specs args nreqs nopts)
      (match specs
	(() (values (reverse args) nreqs nopts #t))
	(((? symbol? sym))
	 (values (reverse (cons (make-arg <rest-arg> sym (+ nreqs nopts)) args))
		 nreqs (+ nopts 1) #t))
	(_ (badarg (car specs)))))
    (required argspecs '() 0))

  (define (extract-rettype forms)
    (define (type-symbol? s)
      (and (keyword? s) (looking-at #/^:[^:]/ (keyword->string s))))
    (define (type-symbol-type s)
      (string->symbol (string-drop (keyword->string s) 1)))

    (match forms
      ((:: type . body) (values body type))
      (((? type-symbol? ts) . body) (values body (type-symbol-type ts)))
      (_ (values forms #f))))

  (define-method process-body ((cproc <c-proc>) body)
    (let loop ((body body))
      (match body
	(() #f)
	(((? string? s) . r) (push-stmt! cproc s) (loop r))
	((('inline opcode) . r) (slot-set! cproc 'inline-insn opcode) (loop r))
	(((? symbol? s)) ;; 'call' convention
	 (let* ((args (map (cut slot-ref <> 'name) (slot-ref cproc 'args)))
		(form (if (eq? (slot-ref cproc 'return-type) '<void>)
			  `((,s ,@args))
			  `((result (,s ,@args)))))
		(rettype (if (pair? (slot-ref cproc 'return-type))
			     '<top>
			     (slot-ref cproc 'return-type))))
	   (process-body-inner cproc rettype form)))
	(_ (process-body-inner cproc (slot-ref cproc 'return-type) body)))))

  (define-method process-body-inner ((cproc <procstub>) rettype body)
    (define (expand-stmt stmt)
      (push-stmt! cproc (if (string? stmt)
			    stmt
			    (cise-render-to-string stmt))))
    (define (typed-result rettype stmts)
      (push-stmt! cproc "{")
      (push-stmt! cproc (format "~a SG_RESULT = (~a)NULL;"
				(slot-ref rettype 'c-type)
				(slot-ref rettype 'c-type)))
      (for-each expand-stmt stmts)
      (push-stmt! cproc (cgen-return-stmt (cgen-box-expr rettype "SG_RESULT")))
      (push-stmt! cproc "}"))
    (match rettype
      (#f				;default, <top>
       (typed-result *scm-type* body))
      ('<void>				; no results
       (for-each expand-stmt body)
       (push-stmt! cproc "SG_RETURN(SG_UNDEF);"))
      ((? symbol? t)
       (typed-result (name->type t) body))
      ;;((? list? ts)) ;; we don't support this
      (_ (error 'define-c-proc "invalid cproc return type" rettype))))

  ;; emit
  (define-method cgen-emit-body ((cproc <c-proc>))
    (p "static SgObject " (slot-ref cproc 'c-name)
       "(SgObject *SG_FP, int SG_ARGC, void *data_)")
    (p "{")
    (for-each emit-arg-decl (slot-ref cproc 'args))
    (p "  SG_ENTER_SUBR(\"" (slot-ref cproc 'scheme-name) "\");")
    ;; argument count check
    (cond ((and (> (slot-ref cproc 'num-optargs) 0)
		(not (slot-ref cproc 'have-rest-args?)))
	   (p "  if (SG_ARGC > " (+ (slot-ref cproc 'num-reqargs) 
				    (slot-ref cproc 'num-optargs)) "||")
	   (p "      SG_ARGC < " (slot-ref cproc'num-reqargs) ")")
	   (p "    Sg_WrongNumberOfArgumentsBetweenViolation(")
	   (f "     SG_INTERN(\"~a\"), ~d, ~d, SG_ARGC, SG_NIL);~%"
	      (slot-ref cproc'scheme-name) (slot-ref cproc'num-reqargs)
	      (+ (slot-ref cproc'num-reqargs) (slot-ref cproc'num-optargs))))
	  ((slot-ref cproc 'have-rest-args?)
	   (p "  if (SG_ARGC < " (slot-ref cproc 'num-reqargs) ")")
	   (p "    Sg_WrongNumberOfArgumentsAtLeastViolation(")
	   (f "     SG_INTERN(\"~a\"), ~d, SG_ARGC, SG_NIL);~%"
	      (slot-ref cproc'scheme-name) (slot-ref cproc'num-reqargs)))
	  (else
	   (p "  if (SG_ARGC != " (slot-ref cproc 'num-reqargs) ")")
	   (p "    Sg_WrongNumberOfArgumentsViolation(")
	   (f "     SG_INTERN(\"~a\"), ~d, SG_ARGC, SG_NIL);~%"
	      (slot-ref cproc'scheme-name) (slot-ref cproc'num-reqargs))))
    (for-each emit-arg-unbox (slot-ref cproc 'args))
    (p "  {")
    (for-each p (slot-ref cproc'stmts))
    (p "  }")
    (p "}") ;; closing the function
    ;; emit stub record
    (let1 flags (slot-ref cproc 'flags)
      (f "static SG_DEFINE_SUBR(")
      (f "~a, ~a, ~a,"
	 (c-stub-name cproc)
	 (slot-ref cproc 'num-reqargs)
	 (cond ((zero? (slot-ref cproc 'num-optargs)) 0)
	       ((slot-ref cproc 'have-rest-args?) (slot-ref cproc 'num-optargs))
	       (else (+ (slot-ref cproc'num-optargs) 1))))
      ;;(f "~a, " (cgen-c-name (slot-ref cproc'proc-name)))
      (f "~a, ~a, NULL);~%"
	 (slot-ref cproc 'c-name)
	 (cond ((slot-ref cproc'inline-insn)
		=> (lambda (i) (format "SG_MAKE_INT(~a)" i)))
	       (else "SG_FALSE"))))
    (p))

  (define-method cgen-emit-init ((cproc <c-proc>))
    (when (symbol? (slot-ref cproc 'scheme-name))
      (f "  Sg_InsertBinding(lib, Sg_Intern(~a), &~a);~%"
	 (cgen-c-name (slot-ref cproc'proc-name)) (c-stub-name cproc))
      (f "  SG_PROCEDURE_NAME(&~a) = ~a;~%"
	 (c-stub-name cproc)
	 (cgen-c-name (slot-ref cproc'proc-name))))
    #;(call-next-method))

  (define-method emit-arg-decl ((arg <arg>))
    (p "  SgObject " (slot-ref arg'scm-name) ";")
    (p "  " (slot-ref (slot-ref arg'type) 'c-type) " " 
       (slot-ref arg'c-name) ";"))

  (define (emit-arg-unbox-rec arg)
    (let1 pred (slot-ref (slot-ref arg 'type) 'c-predicate)
      (when (and pred (not (string-null? pred)))
	(let ((default (slot-ref arg'default)))
	  (cond ((and (not (eq? (class-of arg) <required-arg>))
		      (not (pair? default)))
		 (f "  if (!~a(~a) && !~a)~%"
		    (cond (default (cgen-cpred default))
			  (else "SG_UNBOUNDP"))
		    (slot-ref arg'scm-name)
		    (cgen-pred-expr (slot-ref arg'type)
				    (slot-ref arg'scm-name))))
	      (else
	       (f "  if (!~a)~%" (cgen-pred-expr (slot-ref arg'type)
						 (slot-ref arg'scm-name)))))
	    )
	(p "    Sg_WrongTypeOfArgumentViolation(")
	(f "       ~a, SG_MAKE_STRING(\"~a\"), ~a, SG_NIL);~%"
	   (cgen-c-name (slot-ref (slot-ref arg'proc) 'proc-name))
	   (slot-ref (slot-ref arg'type) 'description) (slot-ref arg'scm-name)))
      (f "  ~a = " (slot-ref arg'c-name))
      (if (slot-ref (slot-ref arg'type) 'unboxer)
	  (f (cgen-unbox-expr (slot-ref arg'type) (slot-ref arg'scm-name)))
	  (f (slot-ref arg'scm-name)))
      (p ";")))

  (define-method emit-arg-unbox ((arg <required-arg>))
    (p "  "(slot-ref arg'scm-name) " = SG_ARGREF(" (slot-ref arg'count) ");")
    (emit-arg-unbox-rec arg))

  (define-method emit-arg-unbox ((arg <optional-arg>))
    (p "  if (SG_ARGC >= " (slot-ref arg'count) "+1) {")
    (p "  "(slot-ref arg'scm-name) " = SG_ARGREF(" (slot-ref arg'count) ");")
    (p "  } else {")
    (p "  "(slot-ref arg'scm-name) " = " (get-arg-default arg) ";")
    (p "  }")
    (emit-arg-unbox-rec arg))

  (define-method emit-arg-unbox ((arg <rest-arg>))
    (p "  " (slot-ref arg'scm-name)
       (format " = Sg_ArrayToList(SG_FP+~d, SG_ARGC-~d);"
	       (slot-ref arg'count) (slot-ref arg'count)))
    (emit-arg-unbox-rec arg))


  ;; boolean handler
  (define-method render-boolean ((b <boolean>) env)
    (if b "SG_TRUE" "SG_FALSE"))
)