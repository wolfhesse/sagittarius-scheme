;; for #[...] read
(library (tmp-reader)
    (export :export-reader-macro)
    (import (rnrs)
	    (sagittarius reader)
	    (srfi :14 char-sets))

  (define-dispatch-macro #\# #\[ (char-set-reader port subc param)
    (let loop ((cs (char-set)))
      (let ((c (get-char port)))
	(if (char=? c #\])
	    cs
	    (let ((nc (lookahead-char port)))
	      (cond ((char=? c #\\)
		     (let ((c2 (get-char port)))
		       ;; support minimal chars s and w
		       (cond ((char=? c2 #\s)
			      (loop (char-set-union! cs char-set:whitespace)))
			     ((char=? c2 #\S)
			      (loop (char-set-union! 
				     cs 
				     (char-set-complement char-set:whitespace))))
			     ((char=? c2 #\w)
			      (loop (char-set-union! cs char-set:letter)))
			     (else
			      (loop (char-set-adjoin! cs c2))))))
		    ((char=? nc #\-)
		     (get-char port)
		     (let ((c2 (get-char port)))
		       (if (char=? c2 #\])
			   (char-set-adjoin! cs c nc)
			   (loop (ucs-range->char-set!
				  (char->integer c)
				  (+ (char->integer c2) 1)
				  #f
				  cs)))))
		    
		    (else (loop (char-set-adjoin! cs c)))))))))
)

#!read-macro=tmp-reader
(import (rnrs)
	(srfi :64)
	(binary parse)
	(util bytevector)
	(sagittarius control))

(define (call-with-input-bytevector bv proc)
  (call-with-port (open-bytevector-input-port bv) proc))

(define (test-find-bytevector input pattern . max-chars)
  (call-with-input-bytevector (string->utf8 input)
    (lambda (p)
      (let* ((n (apply find-bytevector-from-port? (string->utf8 pattern) 
		       p max-chars))
             (c (get-u8 p)))
        (list n (if (eof-object? c) 'eof (integer->char c)))))))

(test-begin "Binary parse")

(test-equal "find-bytevector-from-port?(1)" '(7 #\d)
	    (test-find-bytevector "bacacabd" "acab"))
(test-equal "find-bytevector-from-port?(2)" '(7 #\d)
	    (test-find-bytevector "bacacabd" "acab" 100))
(test-equal "find-bytevector-from-port?(3)" '(#f eof)
	    (test-find-bytevector "bacacabd" "acad"))
(test-equal "find-bytevector-from-port?(4)" '(#f eof)
	    (test-find-bytevector "bacacabd" "acad" 100))
(test-equal "find-bytevector-from-port?(5)" '(#f #\a)
	    (test-find-bytevector "bacacabd" "bd" 5))
(test-equal "find-bytevector-from-port?(6)" '(8 eof)
	    (test-find-bytevector "bacacabd" "bd" 9))
(test-equal "find-bytevector-from-port?(7)" '(8 eof)
	    (test-find-bytevector "bacacabd" "bd"))
(test-equal "find-bytevector-from-port?(8)" '(8 eof)
	    (test-find-bytevector "bacacabd" "bd" 8))
(test-equal "find-bytevector-from-port?(9)" '(#f eof)
	    (test-find-bytevector "bacacabd" "be" 20))

(define (test-parseutil proc input . args)
  (define (->u8-set set)
    (map (lambda (v)
	   (cond ((eqv? '*eof* v) '*eof*)
		 ((char? v) (char->integer v))
		 ((char-set? v) (char-set->u8-set v))
		 (else v)))
	 set))
  (define (convert args)
    (map (lambda (arg) 
	   (cond ((string? arg) (string->utf8 arg))
		 ((list? arg) (->u8-set arg))
		 ((char-set? arg) (char-set->u8-set arg))
		 (else arg))) args))
  (call-with-input-bytevector (string->utf8 input)
    (lambda (p)
      (let* ((c (apply proc (append (convert args) (list p))))
             (n (get-u8 p)))
        (list (cond ((eof-object? c) 'eof) 
		    ((number? c) (integer->char c))
		    ((bytevector? c) (utf8->string c))
		    (else c))
              (if (eof-object? n) 'eof (integer->char n)))))))

(define (test-assert-curr-u8 str clist)
  (test-parseutil assert-curr-u8 str clist "zz"))

(test-equal "assert-curr-u8(1)" '(#\space #\a)
	    (test-assert-curr-u8 " abcd" '(#\a #\space)))
(test-equal "assert-curr-u8(2)" '(#\space #\a)
	    (test-assert-curr-u8 " abcd" #[a ]))
(test-equal "assert-curr-u8(3)" '(#\space #\a)
	    (test-assert-curr-u8 " abcd" #[a\s]))
(test-equal "assert-curr-u8(4)" '(#\space #\a)
	    (test-assert-curr-u8 " abcd" '(#\a #[\s])))
(test-equal "assert-curr-u8(5)" '(#\a #\space)
	    (test-assert-curr-u8 "a bcd" '(#\a #\space)))
(test-equal "assert-curr-u8(6)" '(#\a #\space)
	    (test-assert-curr-u8 "a bcd" #[a ]))
(test-error "assert-curr-u8(7)" condition?
	    (test-assert-curr-u8 "bcd" #[a ]))
(test-error "assert-curr-u8(8)" condition?
	    (test-assert-curr-u8 "" #[a ]))
(test-equal "assert-curr-u8(9)" '(eof eof)
	    (test-assert-curr-u8 "" '(#\a #\space *eof*)))


(test-equal "skip-until number" '(#f #\a)
	    (test-parseutil skip-until " abcd" 1))
(test-error "skip-until number" condition?
	    (test-parseutil skip-until " abcd" 10))
(test-equal "skip-until number" '(#f eof)
	    (test-parseutil skip-until " abcd" 5))

(test-equal "skip-until cset" '(#\space #\a)
	    (test-parseutil skip-until " abcd" '(#\a #\space)))
(test-equal "skip-until cset" '(#\space #\a)
	    (test-parseutil skip-until " abcd" #[a ]))
(test-equal "skip-until cset" '(#\c #\space)
	    (test-parseutil skip-until "xxxc bcd" #[abc ]))
(test-equal "skip-until cset" '(#\c eof)
	    (test-parseutil skip-until "xxxc" #[abc ]))
(test-error "skip-until cset" condition?
	    (test-parseutil skip-until "xxxc" #[def]))
(test-equal "skip-until cset" '(eof eof)
	    (test-parseutil skip-until "xxxc" '(#[def] *eof*)))
(test-equal "skip-until cset" '(#\c eof)
	    (test-parseutil skip-until "xxxc" '(#[c-f] *eof*)))

(test-equal "skip-until proc(1)" '(#\c #\space)
	    (test-parseutil skip-until "xxxc bcd"
			    (^x (not (eqv? x (char->integer #\x))))))
(test-equal "skip-until proc(2)" '(eof eof)
	    (test-parseutil skip-until "xxx"
			    (^x (not (eqv? x (char->integer #\x))))))
(test-error "skip-until proc(3)" condition?
	    (test-parseutil skip-until "yyyy"
			    (^x (eqv? x (char->integer #\x)))))
(test-equal "skip-while(1)" '(#\d #\d)
	    (test-parseutil skip-while "xxxd" '(#\a #\space #\x)))
(test-equal "skip-while(2)" '(#\d #\d)
	    (test-parseutil skip-while "xxxd" #[ax ]))
(test-equal "skip-while(3)" '(#\y #\y)
	    (test-parseutil skip-while "yxxxd" #[ax ]))
(test-equal "skip-while(4)" '(eof eof)
	    (test-parseutil skip-while "xxxa" #[ax ]))
(test-equal "skip-while(5)" '(#\d #\d)
	    (test-parseutil skip-while "xxxd"
			    (^x (eqv? x (char->integer #\x)))))
(test-equal "skip-while(6)" '(#\y #\y)
	    (test-parseutil skip-while "yxxxd"
			    (^x (eqv? x #\x))))
(test-equal "skip-while(7)" '(eof eof)
	    (test-parseutil skip-while "yxxxd"
			    (^x (and (u8? x)
				     (char-alphabetic? (integer->char x))))))

(test-equal "next-token(1)" '("" #\d)
	    (test-parseutil next-token "xxxd" #[ax ] #[d] "next token"))
(test-equal "next-token(2)" '("bc" #\d)
	    (test-parseutil next-token "xxxabcd" #[ax ] #[d] "next token"))
(test-equal "next-token(3)" '("aeio" #\tab)
	    (test-parseutil next-token "   aeio\tnjj" #[\s] #[\s] "next token"))
(test-error "next-token(4)" condition?
	    (test-parseutil next-token "   aeio" #[\s] #[\s] "next token"))
(test-equal "next-token(5)" '("aeio" eof)
	    (test-parseutil next-token "   aeio" #[\s] '(#[\s] *eof*)
			    "next token"))
(test-equal "next-token(6)" '("aeio" #\tab)
	    (test-parseutil next-token "   aeio\tnjj"
			    (^x (and (u8? x)
				     (char-whitespace? (integer->char x))))
			    (^x (or (eof-object? x)
				    (char-whitespace? (integer->char x))))
			    "next token"
			    ))

(test-equal "next-token-of" '("" #\x)
	    (test-parseutil next-token-of "xxxd" #[a-c]))
(test-equal "next-token-of" '("" #\x)
	    (test-parseutil next-token-of "xxxd" #[a-d]))
(test-equal "next-token-of" '("xxx" #\d)
	    (test-parseutil next-token-of "xxxd" #[ax]))
(test-equal "next-token-of" '("anmb" #\-)
	    (test-parseutil next-token-of "anmb-runge" #[\w]))
(test-equal "next-token-of" '("rnge!rg0#$@" #\space)
	    (test-parseutil next-token-of "rnge!rg0#$@ bag" #[\S]))
(test-equal "next-token-of" '("xxx" #\d)
	    (test-parseutil next-token-of "xxxd"
			    (^x (eqv? x (char->integer #\x)))))
(test-equal "next-token-of" '("xxxx" eof)
	    (test-parseutil next-token-of "xxxx"
			    (^x (eqv? x (char->integer #\x)))))

(test-end)