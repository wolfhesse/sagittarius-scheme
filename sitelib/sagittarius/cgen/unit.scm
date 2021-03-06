;; -*- mode: scheme; coding: utf-8; -*-
#!read-macro=sagittarius/regex
(library (sagittarius cgen unit)
    (export <cgen-unit> cgen-current-unit
	    ;; accessors
	    cgen-unit-name
	    cgen-unit-c-file
	    cgen-unit-h-file
	    cgen-unit-toplevel-nodes
	    cgen-unit-init-prologue
	    cgen-unit-preamble
	    cgen-unit-init-prologue
	    cgen-unit-init-epilogue
	    
	    cgen-add! cgen-emit-h cgen-emit-c
	    cgen-emit-preamble cgen-emit-prologue cgen-emit-epilogue

	    <cgen-node> cgen-body cgen-extern cgen-decl cgen-init
	    cgen-include cgen-define cgen-cpp-conditions

	    cgen-safe-name
	    cgen-safe-name-friendly
	    cgen-safe-comment

	    cgen-emit-part
	    cgen-emit-xtrn
	    cgen-emit-decl
	    cgen-emit-body
	    cgen-emit-init
	    cgen-emit-static-data)
    (import (except (rnrs) define)
	    (core base) ;; for print
	    (sagittarius)
	    (rename (sagittarius control) (define-with-key define))
	    (sagittarius io)
	    (sagittarius regex)
	    (srfi :2 and-let*)
	    (srfi :13 strings)
	    (srfi :26 cut)
	    (srfi :39 parameters)
	    (srfi :42 eager-comprehensions)
	    (match)
	    (util list)
	    (shorten)
	    (clos core)
	    (clos user))

  (define-class <cgen-unit> ()
    (;; public
     (name     :accessor cgen-unit-name   :init-keyword :name
	       :init-value "cgen")
     (c-file   :init-keyword :c-file :init-value #f)
     (h-file   :accessor cgen-unit-h-file :init-keyword :h-file :init-value #f)
     (preamble :accessor cgen-unit-preamble :init-keyword :preamble
	       :init-value '("/* Generated by (sagittarius cgen) */"))
     (init-prologue :accessor cgen-unit-init-prologue
		    :init-keyword :init-prologue
		    :init-value #f)
     (init-epilogue :accessor cgen-unit-init-epilogue
		    :init-keyword :init-epilogue
		    :init-value #f)
     ;; private
     (toplevels :init-value () :accessor cgen-unit-toplevel-nodes)
     (transients :init-value ())
     (literals :init-value #f)
     (static-data-list :init-value ())
     (unit-id :init-form (gensym "cgen"))))

  (define cgen-current-unit (make-parameter #f))
  ;; generic methods
  (define-generic cgen-unit-c-file)
  (define-generic cgen-emit-part)
  (define-generic cgen-emit-preamble)
  (define-generic cgen-emit-prologue)
  (define-generic cgen-emit-epilogue)
  (define-generic cgen-emit-h)
  (define-generic cgen-emit-c)
  (define-generic cgen-decl-common)
  (define-generic cgen-emit-xtrn)
  (define-generic cgen-emit-decl)
  (define-generic cgen-emit-body)
  (define-generic cgen-emit-init)
  (define-generic cgen-node-traverse)
  (define-generic cgen-emit)
  
  (define-method cgen-unit-c-file ((unit <cgen-unit>))
    (or (slot-ref unit 'c-file)
	(format "~a.c" (cgen-unit-name unit))))
  
  (define (cgen-add! node)
    (and-let* ((unit (cgen-current-unit)))
      (let ((nodes (cgen-unit-toplevel-nodes unit)))
	(cgen-unit-toplevel-nodes unit (append! nodes (list node)))))
    node)

  (define-method cgen-emit-part ((unit <cgen-unit>) part)
    (let1 context (make-eq-hashtable)
      (define (walker node)
	(unless (hashtable-ref context node #f)
	  (hashtable-set! context node #t)
	  (cgen-node-traverse node walker)
	  (cgen-emit node part)))
      (for-each walker (cgen-unit-toplevel-nodes unit))))

  (define-method cgen-emit-preamble ((unit <cgen-unit>))
    (cond ((cgen-unit-preamble unit) => emit-raw)))
  (define-method cgen-emit-prologue ((unit <cgen-unit>))
    (cond ((cgen-unit-init-prologue unit) => emit-raw)
	  (else
	   (print "void Sg__Init_" (cgen-safe-name (cgen-unit-name unit))
		  "()")
	   (print "{"))))
  (define-method cgen-emit-epilogue ((unit <cgen-unit>))
    (cond ((cgen-unit-init-epilogue unit) => emit-raw)
	  (else (print "}"))))
  
  (define-method cgen-emit-h ((unit <cgen-unit>))
    (and-let* ((h-file (cgen-unit-h-file unit)))
      (cgen-with-output-file h-file
       (lambda ()
	 (cgen-emit-preamble unit)
	 (cgen-emit-part unit 'extern)))))

  (define-method cgen-emit-c ((unit <cgen-unit>))
    (and-let* ((c-file (cgen-unit-c-file unit)))
      (cgen-with-output-file c-file
       (lambda ()
	 (cgen-emit-preamble unit)
	 (cgen-emit-part unit 'decl)
	 (cgen-emit-static-data unit)
	 (cgen-emit-part unit 'body)
	 (cgen-emit-prologue unit)
	 (cgen-emit-part unit 'init)
	 (cgen-emit-epilogue unit)))))


  (define-class <cgen-node> ()
    ((extern?        :init-keyword :extern? :init-value #f)
     (cpp-conditions :init-keyword :cpp-condition)))

  (define-generic cgen-emit-static-data)
  (define-method initialize ((node <cgen-node>) initargs)
    (call-next-method) ;; call object initialize
    (slot-set! node 'cpp-conditions (cgen-cpp-conditions)))

  (define %cgen-cpp-conditions (make-parameter '()))
  (define (cgen-cpp-conditions) (%cgen-cpp-conditions))

  (define-syntax cgen-with-cpp-condition
    (syntax-rules ()
      ((_ condition . body)
       (let1 new-conditions (cons (cpp-condition->string condition)
				  (%cgen-cpp-conditions))
	 (parameterize ((%cgen-cpp-conditions new-conditions))
	   . body)))))

  (define (cpp-condition->string condition)
    (define (rec c)
      (match c
	((and (or (? string?) (? symbol?) (? integer?)) c) (format "~a" c))
	(('defined c) (format "defined(~a)" (rec c)))
	(('not c)     (format "!(~a)" (rec c)))
	(('and c ...) (n-ary "&&" c))
	(('or c ...)  (n-ary "||" c))
	(((and (or '+ '*) op) c ...) (n-ary op c))
	(((and (or '- '/) op) c0 c1 ...)
	 (if (null? c1) (format "~a(~a)" op (rec c0)) (n-ary op (cons c0 c1))))
	(((and (or '> '>= '== '< '<= '!= 'logand 'logior 'lognot  '>> '<<) op)
	  c0 c1)
	 (binary op c0 c1))
	(else (error 'cpp-condition->string
		     "Invalid C preprocessor condition expression" condition))))
    (define (n-ary op cs)
      (string-concatenate (intersperse (format "~a" op)
				       (map (^c (format "(~a)" c)) cs))))
    (define (binary op c0 c1)
      (format "(~a)~a(~a)" (rec c0) op (rec c1)))
    (rec condition))

  ;; fallback methods
  (define-method cgen-decl-common ((node <cgen-node>)) #f)
  
  (define-method cgen-emit-xtrn ((node <cgen-node>)) #f)
  (define-method cgen-emit-decl ((node <cgen-node>)) #f)
  (define-method cgen-emit-body ((node <cgen-node>)) #f)
  (define-method cgen-emit-init ((node <cgen-node>)) #f)

  ;; TODO make slot-definition-name
  (define-method cgen-node-traverse ((node <cgen-node>) walker)
    (do-ec (: slot (map car (class-slots (class-of node))))
	   (if #t)
	   (and-let* ((var (slot-ref node slot))
		      (  (is-a? var <cgen-node>) ))
	     (walker var))))

  (define-method cgen-emit ((node <cgen-node>) part)
    (define (method-overridden? gf)
      (and-let* ((methods (compute-applicable-methods gf (list node)))
		 ( (not (null? methods)) ))
	(match (method-specializers (car methods))
	  (((? (cut eq? <> <cgen-node>))) #f)
	  (_ #t))))
    (define (with-cpp-condition gf)
      (cond ((slot-ref node 'cpp-conditions)
	     => (^(cppc) (cond ((method-overridden? gf)
				(for-each (cut print "#if " <>) cppc)
				(gf node)
				(for-each (cut print "#endif /* " <> " */")
					  (reverse cppc)))
			       (else (gf node)))))))
    (case part
      ((extern) (with-cpp-condition cgen-emit-xtrn))
      ((decl)   (with-cpp-condition cgen-emit-decl))
      ((body)   (with-cpp-condition cgen-emit-body))
      ((init)   (with-cpp-condition cgen-emit-init))))

  (define-class <cgen-raw-xtrn> (<cgen-node>)
    ((code  :init-keyword :code :init-value "")))
  (define-method cgen-emit-xtrn ((node <cgen-raw-xtrn>))
    (emit-raw (slot-ref node'code)))

  (define-class <cgen-raw-decl> (<cgen-node>)
    ((code  :init-keyword :code :init-value "")))
  (define-method cgen-emit-decl ((node <cgen-raw-decl>))
    (emit-raw (slot-ref node'code)))

  (define-class <cgen-raw-body> (<cgen-node>)
    ((code  :init-keyword :code :init-value "")))
  (define-method cgen-emit-body ((node <cgen-raw-body>))
    (emit-raw (slot-ref node'code)))

  (define-class <cgen-raw-init> (<cgen-node>)
    ((code  :init-keyword :code :init-value "")))
  (define-method cgen-emit-init ((node <cgen-raw-init>))
    (emit-raw (slot-ref node'code)))


  (define (cgen-extern . code)
    (cgen-add! (make <cgen-raw-xtrn> :code code)))

  (define (cgen-decl . code)
    (cgen-add! (make <cgen-raw-decl> :code code)))
  
  (define (cgen-body . code)
    (cgen-add! (make <cgen-raw-body> :code code)))

  (define (cgen-init . code)
    (cgen-add! (make <cgen-raw-init> :code code)))

  ;; cpp
  ;; #include
  (define-class <cgen-include> (<cgen-node>)
    ((path :accessor cgen-include-path :init-keyword :path)))

  (define (include-common node)
    (print "#include " (if (string-prefix? "<" (cgen-include-path node))
			   (cgen-include-path node)
			   (format "\"~a\"" (cgen-include-path node)))))
  (define-method cgen-emit-xtrn ((node <cgen-include>))
    (include-common node))
  (define-method cgen-emit-decl ((node <cgen-include>))
    (include-common node))
  
  (define (cgen-include path)
    (cgen-add! (make <cgen-include> :path path)))

  ;; #define
  (define-class <cgen-cpp-define> (<cgen-node>)
    ((name  :init-keyword :name)
     (value :init-keyword :value)))

  (define (cpp-define-common node)
    (print "#define " (slot-ref node'name) " " (slot-ref node 'value)))

  (define-method cgen-emit-xtrn ((node <cgen-cpp-define>))
    (cpp-define-common node))
  (define-method cgen-emit-decl ((node <cgen-cpp-define>))
    (cpp-define-common node))

  (define (cgen-define name . maybe-value)
    (cgen-add! 
     (make <cgen-cpp-define> :name name :value (get-optional maybe-value ""))))

  ;; utilities
  (define (cgen-with-output-file file thunk)
    ;; we don't create tmpfile
    (let ((port (open-file-output-port file (file-options no-fail) 'block
				       (native-transcoder))))
      (dynamic-wind
	  (lambda () #t)
	  (lambda () (with-output-to-port port thunk))
	  (lambda () (close-output-port port)))))
	
  (define (emit-raw code)
    (if (list? code)
	(for-each print code)
	(print code)))
  ;; safe name for C.
  (define (cgen-safe-name str)
    (call-with-string-output-port
     (lambda (out)       
       (call-with-input-string str
	 (lambda (in)
	   (let loop ((c (read-char in)))
	     (unless (eof-object? c)
	       (let ((b (char->integer c)))
		 (cond ((or (<= 48 b 57)
			    (<= 65 b 90)
			    (<= 97 b 122))
			(display c out) (loop (read-char in)))
		       (else
			(format out "_~2,'0x" b) (loop (read-char in))))))))))))
  
  (define-constant +c-keywords+ '("default" "switch" "template" "case"))
  (define (cgen-safe-name-friendly str)
    ;; some c/c++ keyword handling...
    (if (member str +c-keywords+ string=?)
	(string-append str "_") ;; simple conversion should be enough
	(call-with-string-output-port
	 (lambda (out)       
	   (call-with-input-string str
	     (lambda (in)
	       (let loop ((c (read-char in)))
		 (unless (eof-object? c)
		   (case c
		     ((#\-) (let ((d (read-char in)))
			      (cond ((eqv? d #\>)
				     (display "_TO" out) (loop (read-char in)))
				    (else (display #\_ out) (loop d)))))
		     ((#\?) (display #\P out) (loop (read-char in)))
		     ((#\!) (display #\X out) (loop (read-char in)))
		     ((#\<) (display "_LT" out) (loop (read-char in)))
		     ((#\>) (display "_GT" out) (loop (read-char in)))
		     ((#\* #\> #\@ #\$ #\% #\^ #\& #\* #\+ #\= #\: #\. #\/ #\~)
		      (display #\_ out)
		      (display (number->string (char->integer c) 16) out)
		      (loop (read-char in)))
		     (else (display c out) (loop (read-char in))))))))))))

  (define (cgen-safe-comment str)
    (let* ((str (format "~a" str))
	   (str2 (regex-replace-all #/\/\*/ str "/ *")))
      (regex-replace-all #/\*\// str2 "* /")))
				 
)
