;;; -*- mode: scheme; coding: utf-8; -*-
;;;
;;; sagittarius/cgen/precomp.scm - Scheme->C translator
;;;  
;;;   Copyright (c) 2010-2013  Takashi Kato  <ktakashi@ymail.com>
;;;   
;;;   Redistribution and use in source and binary forms, with or without
;;;   modification, are permitted provided that the following conditions
;;;   are met:
;;;   
;;;   1. Redistributions of source code must retain the above copyright
;;;      notice, this list of conditions and the following disclaimer.
;;;  
;;;   2. Redistributions in binary form must reproduce the above copyright
;;;      notice, this list of conditions and the following disclaimer in the
;;;      documentation and/or other materials provided with the distribution.
;;;  
;;;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;;;   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;;;   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;;;   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;;;   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;;;   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
;;;   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
;;;   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;;;   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
;;;   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
;;;   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;;;  

;; the precomp library is basically different implementation of
;; boot/tools/code2c2.scm. In near future, we want to remove the
;; vm.scm to generate compiled C code.

;; 
#!read-macro=sagittarius/regex
(library (sagittarius cgen precomp)
    (export cgen-precompile cgen-precompile-file
	    default-name-generator
	    +replace-prefix+
	    )
    (import (rnrs)
	    (rnrs eval)
	    (clos user)
	    (sagittarius)
	    (sagittarius clos)
	    (sagittarius object)
	    (sagittarius control)
	    (sagittarius regex)
	    (sagittarius vm)
	    (sagittarius vm debug)
	    (sagittarius compiler)
	    (sagittarius cgen stub)
	    (sagittarius cgen unit)
	    (sagittarius cgen literal)
	    (match)
	    (core base) ;; for print
	    (srfi :1 lists)
	    (srfi :13 strings)
	    (srfi :26 cut)
	    (srfi :39 parameters)
	    (util file)
	    (util list))

  ;; if library name starts this then the library will be replaced
  ;; to current library.
  ;; mostly for hygine macro so make sure the compiling library
  ;; can refer it
  (define-constant +replace-prefix+ "~precomp.")
  (define-constant replace-pattern #/~precomp\./)

  (define-class <cgen-precomp-unit> (<cgen-stub-unit>)
    ((library :init-keyword :library)
     (imports :init-keyword :imports)
     ;; not used...
     (exports :init-keyword :exports)))

  ;; for future though
  (define (read-with-source in) (read in :source-info? #t))

  (define (default-name-generator filename library-name)
    (let1 safe-name (library-name->string library-name)
      (values (or (and filename
		       (let1 base (path-basename (path-sans-extension filename))
			 (string-append base ".c")))
		  (string-append safe-name ".c"))
	      safe-name)))

  (define (cgen-precompile-file file . options)
    (apply cgen-precompile (call-with-input-file file read-with-source)
	   :in-file file
	   options))

  (define (cgen-precompile form
			   :key (name-generator default-name-generator) 
			   (in-file #f)
			   (predef-syms '()))
    (unless (pair? form) (error 'cgen-precompile "form must be a list" form))
    ;; for now we only support library form
    ;; define-library is a bit too much to handle
    (match form
      (('library name 
	   ('export exports ...) 
	   ('import imports ...)
	 toplevels ...)
       (let-values (((out-file initialiser) (name-generator in-file name)))
	 (let1 safe-name (encode-library-name name)
	   (parameterize ((cgen-current-unit (get-unit in-file initialiser
						       out-file predef-syms)))
	     ;; should be handled in get-unit but i'm lazy...
	     (set! (~ (cgen-current-unit) 'library) safe-name)
	     (set! (~ (cgen-current-unit) 'imports) imports)
	     (set! (~ (cgen-current-unit) 'exports) exports)
	     (do-it safe-name exports imports toplevels)))))
      (_ (error 'cgen-precompile "invalid form"  form))))

  (define (do-it safe-name exports imports toplevels)
    (emit-toplevel-executor safe-name imports exports
     (compile-form
      (construct-safe-library safe-name exports imports toplevels)))
    (cgen-emit-c (cgen-current-unit)))
  
  (define (emit-toplevel-executor name imports exports topcb)
    (cgen-body "static SgCodeBuilder *toplevel = ")
    (cgen-body 
     (format "   SG_CODE_BUILDER(~a);" (cgen-cexpr topcb)))
    (let1 library (find-library name #f) ;; get the library
      ;; emit imports
      (for-each (lambda (i)
		  ;; we don't resolve any condition but for
		  ;; and for will be ignored
		  (cond ((not (pair? i))
			 (cgen-init (format "  Sg_ImportLibrary(~a, ~a);~%"
					    (cgen-cexpr (cgen-literal library))
					    (cgen-cexpr (cgen-literal i)))))
			((eq? (car i) 'for))
			(else
			 (cgen-init 
			  (format "  Sg_ImportLibrary(~a, ~a);~%"
				  (cgen-cexpr (cgen-literal library))
				  (cgen-cexpr 
				   (cgen-literal 
				    (string->symbol (format "~a" i)))))))))
		imports)
      ;; emit exports
      (for-each (lambda (e)
		  (cgen-init (format "  SG_APPEND1(h, t, ~a); /* ~a */"
				     (cgen-cexpr (cgen-literal e))
				     (cgen-safe-comment e)))) exports)
      (cgen-init (format "  Sg_LibraryExportedSet(~a, Sg_Cons(h, SG_NIL));~%"
			 (cgen-cexpr (cgen-literal library))))
      (cgen-init (format "  Sg_VM()->currentLibrary = ~a;" 
			 (cgen-cexpr (cgen-literal library))))
      (cgen-init (format "  Sg_VMExecute(SG_OBJ(toplevel));"))
      (cgen-init "  Sg_VM()->currentLibrary = save;")))

  ;; use '.' joined library form...
  ;; it's legal on Sagittarius for historical reason
  ;; but we can use it anyway...
  (define (encode-library-name name)
    (string->symbol
     (string-join (map (lambda (s) (symbol->string s)) name) ".")))

  (define (decode-library-name name)
    (define (encoded? name)
      (string-contains (symbol->string name) "."))
    (if (encoded? name)
	(map string->symbol (string-split (symbol->string name)  #/\./))
	name))

  ;; To maximise the compiler optimisation we need to compile it
  ;; with library form
  (define (construct-safe-library name exports imports toplevels)
    `(library ,name
	 (export ,@exports)
	 (import ,@imports)
       ,@toplevels))
  
  (define (compile-form form)
    (let1 cb (compile form (environment '(only (sagittarius) library)))
      (cgen-literal cb)))

  (define (get-unit in-file initfun-name out.c predef-syms)
    (let* ((base (path-sans-extension (path-basename out.c)))
	   (safe-name (regex-replace-all #/[-+]/ base "_")))
      (rlet1 u 
	  (make <cgen-precomp-unit>
	    :name base :c-name-prefix safe-name
	    :preamble 
	    `(,(format "/* Generated automatically from ~a. DO NOT EDIT! */"
		       (or in-file in-file "<anonymous>")))
	    :init-prologue 
	    (format "void Sg__Init_~a() {~%  \
                       SgObject save = Sg_VM()->currentLibrary;~%  \
                       SgObject h = SG_NIL, t = SG_NIL; /* for exports */ ~%" 
		    initfun-name))
	(parameterize ((cgen-current-unit u))
	  (for-each cgen-define predef-syms)
	  (cgen-include "<sagittarius.h>")))))
  
  (define-cgen-literal <cgen-scheme-code> <code-builder>
    ((code-name :init-keyword :code-name)
     (code-vector-c-name :init-keyword :code-vector-c-name)
     (literals  :init-keyword :literals))
    (make (value)
      (define (fixup cb-name)
	(if (pair? cb-name)
	    (string->symbol (format "~a" cb-name))
	    cb-name))
      (let* ((cv (code-builder->vector value))
	     (lv (extract-literals cv))
	     (cvn (allocate-code-vector cv lv (code-builder-name value)))
	     (code-name (cgen-literal (fixup (code-builder-name value)))))
	(define (init-thunk) 
	  (format #t "    SG_STATIC_CODE_BUILDER( /* ~a */~%"
		  (cgen-safe-comment (code-builder-name value)))
	  (format #t "      (SgWord *)~a, ~a, ~a, ~a, ~a, ~a,"
		  cvn (if (cgen-literal-static? code-name)
			  (cgen-cexpr code-name)
			  "SG_FALSE")
		  (code-builder-argc value) (code-builder-optional value)
		  (code-builder-freec value) (code-builder-maxstack value))
	  (format #t " ~a)" (vector-length cv)))
	(let1 c-name (cgen-allocate-static-datum 'runtime 'SgCodeBuilder
						 init-thunk)
	  (make <cgen-scheme-code> :value value
		:c-name c-name
		:code-vector-c-name cvn
		:code-name code-name
		:literals lv))))
    (init (self)
      (unless (cgen-literal-static? (~ self 'code-name))
	(print "  SG_CODE_BUILDER(" (~ self 'c-name) ")->name = "
	       (cgen-cexpr (~ self 'code-name)) ";"
	       "/* " (cgen-safe-comment (code-builder-name (~ self 'value)))
	       " */"))
      (fill-code self))
    (static (self) #t))

  (define (extract-literals code)
    (let* ((len (vector-length code))
	   (lits (make-vector len #f)))
      (do ((i 0 (+ i 1)))
	  ((= i len) lits)
	(let1 insn (vector-ref code i)
	  (let-values (((name&insn iv argc src? label?) (insn-info insn)))
	    (unless (zero? argc) ;; always 0 or 1
	      (vector-set! lits (+ i 1)
			   (cgen-literal (vector-ref code (+ i 1)))))
	    (set! i (+ i argc)))))))
  
  (define (allocate-code-vector cv lv full-name)
    (define (alloc-word initval)
      (cgen-allocate-static-datum 'runtime 'SgWord initval))
    (define (loop cv lv count first-cexpr)
      (if (= count (vector-length cv))
	  first-cexpr
	  (let1 insn (vector-ref cv count)
	    (let-values (((name&insn iv argc src? label?) (insn-info insn)))
	      (let* ((insn-name (car name&insn))
		     (name-info (if first-cexpr
				    ""
				    (format "/* ~a */"
					    (cgen-safe-comment full-name))))
		     (insn-cexpr
		      (alloc-word
		       (if (> insn #x80000000)
			   (format "~a-0x~8,'0x   /* ~3d ~a */"
				   name-info (- #x100000000 insn) count
				   (cgen-safe-comment insn-name))
			   (format "~a0x~8,'0x    /* ~3d ~a */"
				   name-info insn count
				   (cgen-safe-comment insn-name)))))
		     (first-cexpr (or first-cexpr insn-cexpr)))
		(cond (label? 
		       (alloc-word (format "SG_WORD(~a)" 
					   (vector-ref cv (+ count 1))))
		       (loop cv lv (+ count 2) first-cexpr))
		      ((not (zero? argc))
		       (let ((v (vector-ref lv (+ count 1)))
			     (c (cgen-safe-comment 
				 (format "~a" (vector-ref cv (+ count 1))))))
			 (alloc-word
			  (if (cgen-literal-static? v)
			      (format "SG_WORD(~a) /* ~a */" (cgen-cexpr v) c)
			      (format "SG_WORD(SG_UNDEF) /* ~a */" c)))
			 (loop cv lv (+ count 2) first-cexpr)))
		      (else 
		       (loop cv lv (+ count 1) first-cexpr))))))))
    (loop cv lv 0 #f))
			     
  (define (fill-code cb)
    (let ((cvn (~ cb 'code-vector-c-name))
	  (lv  (~ cb 'literals)))
      (do ((len (vector-length lv)) (i 0 (+ i 1)))
	  ((= len i))
	(let1 lit (vector-ref lv i)
	  (when (and lit (not (cgen-literal-static? lit)))
	    (format #t "  ((SgWord*)~a)[~a] = SG_WORD(~a);~%"
		    cvn i (cgen-cexpr lit)))))))

  (define (library-name->string lib)
    (let ((s (string-map (lambda (c)
			   (cond ((char=? c #\space) #\_)
				 ((or (char=? c #\()
				      (char=? c #\))) #\space)
				 ((char=? c #\-) #\_)
				 (else c))) (format "~a" lib))))
      (string-trim-both s)))

  (define-cgen-literal <cgen-scheme-identifier> <identifier>
    ((id-name :init-keyword :id-name)
     (library :init-keyword :library)) ;; name
    (make (value)
      (unless (null? (id-envs value))
	(format (current-error-port) 
		"*WARNING* identifier '~a' in ~a contains environment~%    \
                 assume the identifier can be resolved in the \
                 target library.~%"
		(id-name value) (library-name (id-library value))))
      (let1 libname (symbol->string (library-name (id-library value)))
	(make <cgen-scheme-identifier> :value value
	      :c-name (cgen-allocate-static-datum)
	      :id-name (cgen-literal (id-name value))
	      :library (cgen-literal 
			(if (and (null? (id-envs value))
				 (not (replace-pattern libname)))
			    (id-library value)
			    (find-library (~ (cgen-current-unit) 'library)
					  #f))))))
    (init (self)
      (let ((name (cgen-cexpr (~ self 'id-name)))
	    (cname (~ self 'c-name)))
	(format #t "  ~a = Sg_MakeIdentifier(SG_SYMBOL(~a), SG_NIL, (~a));~%"
		cname name (cgen-cexpr (~ self 'library)))))
    (static (self) #f))

  (define-cgen-literal <cgen-scheme-library> <library>
    ((name :init-keyword :name))
    (make (value)
      (make <cgen-scheme-library> :value value
	    :c-name (cgen-allocate-static-datum)
	    :name (cgen-literal 
		   (string->symbol 
		    (format "~a" (decode-library-name (library-name value)))))))
    (init (self)
      (let ((name (cgen-cexpr (~ self 'name)))
	    (cname (~ self 'c-name)))
	(format #t "  ~a = Sg_FindLibrary(SG_SYMBOL(~a), TRUE);~%"
		cname name)))
    (static (self) #f))

  (define (last-pair* l)
    (let loop ((l l) (r '()))
      (cond ((null? l) (values (reverse! r) '()))
	    ((not (pair? l)) (values (reverse! r) l))
	    (else (loop (cdr l) (cons (car l) r))))))

  (define-cgen-literal <cgen-scheme-pair> <pair>
    ((values-list :init-keyword :values-list))
    (make (value)
      (make <cgen-scheme-pair> :value value
	    :c-name (cgen-allocate-static-datum)
	    :values-list (let-values (((l last) (last-pair* value)))
			   (if (null? last)
			       (map cgen-literal l)
			       (append! (map cgen-literal l)
					(cgen-literal last))))))
    (init (self)
      (let ((values (~ self 'values-list))
	    (cname (~ self 'c-name))
	    (h     (gensym))
	    (t     (gensym)))
	(print "  do {")
	(format #t "    SgObject ~a = SG_NIL, ~a = SG_NIL;~%" h t)
	(let-values (((l last) (last-pair* values))
		     ((v vlast) (last-pair* (~ self 'value))))
	  (for-each (lambda (v o)
		      (format #t "    SG_APPEND1(~a, ~a, ~a); /* ~a */ ~%"
			      h t (cgen-cexpr v) (cgen-safe-comment o))) l v)
	  (unless (null? last)
	    (format #t "    SG_SET_CDR(~a, ~a); /* ~a */~%"
		    t (cgen-cexpr last) (cgen-safe-comment vlast))))
	(format #t "    ~a = ~a;~%" cname h)
	(print "  } while (0);")))
    (static (self) #f))
)