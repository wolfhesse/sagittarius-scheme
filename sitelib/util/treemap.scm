;;; -*- mode: scheme; coding: utf-8 -*-
;;;
;;; treemap.scm - treemap utilities
;;;  
;;;   Copyright (c) 2010-2014  Takashi Kato  <ktakashi@ymail.com>
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

(library (util treemap)
    (export make-rb-treemap
	    treemap-ref
	    treemap-set!
	    treemap-delete!
	    treemap-update!
	    treemap-clear!
	    treemap-copy
	    treemap-contains?
	    treemap-size
	    treemap-entries treemap-entries-list
	    treemap-keys    treemap-keys-list
	    treemap-values  treemap-values-list

	    treemap-for-each treemap-map
	    treemap->alist
	    )
    (import (rnrs)
	    (clos user)
	    (sagittarius)
	    (sagittarius object))

  ;; TODO don't traverse twice
  (define (treemap-update! tm key proc default)
    (treemap-set! tm key (proc (treemap-ref tm key default))))

  ;; sort of consistency for hashtable
  (define (treemap-entries tm) 
    (let-values (((ks vs) (treemap-entries-list tm)))
      (values (list->vector ks) (list->vector vs))))
  (define (treemap-keys tm) (list->vector (treemap-keys-list tm)))
  (define (treemap-values tm) (list->vector (treemap-values-list tm)))
  
  (define (treemap-for-each proc tm)
    (for-each proc (treemap-keys-list tm) (treemap-values-list tm)))
  (define (treemap-map proc tm)
    (map proc (treemap-keys-list tm) (treemap-values-list tm)))
  (define (treemap->alist tm) (treemap-map cons tm))

  ;; for generic ref
  (define-method ref ((tm <tree-map>) key)
    (treemap-ref tm key #f))
  (define-method ref ((tm <tree-map>) key fallback)
    (treemap-ref tm key fallback))
  (define-method (setter ref) ((tm <tree-map>) key value)
    (treemap-set! tm key value))

)