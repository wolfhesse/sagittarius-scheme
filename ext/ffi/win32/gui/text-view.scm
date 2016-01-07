;;; -*- mode: scheme; coding: utf-8; -*-
;;;
;;; win32/gui/text-view - Win32 GUI text view
;;;  
;;;   Copyright (c) 2015  Takashi Kato  <ktakashi@ymail.com>
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

;; this is *not* a wrapper of predefined Windows control.
;; the text-view is a type of component which can show and
;; edit given text value such as multiline edit control.
;; the difference between those 2 would be this has more
;; control than the other. a text-view would have underlying
;; buffer to do undo/redo and it can be controlled by users.

;; based on http://www.catch22.net/tuts/neatpad tutorial
(library (win32 gui text-view)
    (export <win32-text-view> win32-text-view? make-win32-text-view
	    ;; TODO more
	    )
    (import (rnrs)
	    (sagittarius)
	    (sagittarius ffi)
	    (win32 user)
	    (win32 defs)
	    (win32 gdi)
	    (win32 kernel)
	    (win32 gui api)
	    (win32 gui edit)
	    (clos user)
	    (sagittarius object))

(define-constant *win32-default-text-view-class-name* 
  "sagittarius-default-text-view-class")
;; so that users can send WM_SETFONT message or
;; others via the same interface.
(define-class <win32-text-view> (<win32-edit>) 
  ((window-lines :init-value 0)
   (window-columns :init-value 0)
   (horizontal-scroll-position :init-value 0)
   (vertical-scroll-position :init-value 0)
   (horizontal-scroll-max :init-value 0)
   (vertical-scroll-max :init-value 0)
   (tab-width :init-keyword :tab-width :init-value 8)
   (mouse-down :init-value #f)
   (selection-start :init-value 0)
   (selection-end :init-value 0)
   (cursor-offset :init-value 0)
   (filename :init-keyword :filename :init-value #f)))
(define (win32-text-view? o) (is-a? o <win32-text-view>))
(define (make-win32-text-view . opt) (apply make <win32-text-view> opt))

(define-method initialize ((t <win32-text-view>) initargs)
  (call-next-method)
  ;; it's not a pre-defined Windows component, so we just 
  ;; overwrite the value unlike the other components.
  (set! (~ t 'class-name) *win32-default-text-view-class-name*)
  (set! (~ t 'style) (bitwise-ior (~ t 'style) WS_VSCROLL WS_HSCROLL))
  (set! (~ t 'window-style) WS_EX_CLIENTEDGE)
  ;; TODO do we need name?
  (set! (~ t 'name) "")
  (set! (~ t 'value) (make <win32-text-view-buffer>))
  (win32-set-event-handler! t 'file-open handle-open-file)
  (and-let* ((file (~ t 'filename)))
    (load-file t file))
  t)

;; 'file-open event
;; wparam: ignore
;; lparam: pointer of filepath
(define (load-file text-view file)
  ;; TODO better implementation
  (call-with-input-file file
    (lambda (in)
      (let loop ((lines '()) (longest 0) (size 0) (total 0))
	(let ((line (read-line in)))
	  (if (eof-object? line)
	      (let ((buffer (~ text-view 'value)))
		(set! (~ buffer 'size) total)
		(set! (~ buffer 'count) size)
		(set! (~ buffer 'lines) (reverse! lines))
		(set! (~ buffer 'longest-line) longest)
		;; :filename can be passed so we need to check this
		(when (~ text-view 'hwnd) (update-metrics text-view)))
	      (let ((len (string-length line)))
		(loop (cons line lines)
		      (max len longest)
		      (+ size 1)
		      (+ len total)))))))))
(define (handle-open-file text-view message wparam lparam)
  (when lparam
    (let ((s (pointer->object lparam)))
      (load-file text-view s))))

(define (refresh-window text-view)
  (invalidate-rect (~ text-view 'hwnd) null-pointer #f))
(define (update-metrics text-view)
  (let ((r (allocate-c-struct RECT)))
    (get-client-rect (~ text-view 'hwnd) r)
    (handle-size text-view 0 (c-struct-ref r RECT 'right) 
		 (c-struct-ref r RECT 'bottom))
    (refresh-window text-view)))

(define (get-color text-view type)
  (case type
    ((background) (get-sys-color COLOR_WINDOW))
    ((text) (get-sys-color COLOR_WINDOWTEXT))
    (else null-pointer)))

(define (handle-paint text-view) 
  (define (text-out text-view hdc rect chunk attr)
    (define font-width (~ text-view 'font-width))
    ;; set colours
    (set-text-color hdc (get-color text-view 'text))
    (set-bk-color hdc (get-color text-view 'background))
    ;; should we do it here?
    (select-object hdc (~ text-view 'font))
    (let ((tab (integer->pointer (* font-width (~ text-view 'tab-width))))
	  (left (c-struct-ref rect RECT 'left)))
      ;; draw line and expand tabs
      (let ((w (tabbed-text-out hdc
				left
				(c-struct-ref rect RECT 'top)
				chunk
				(string-length chunk)
				1
				(address tab)
				left)))
	(+ left (win32-loword w)))))
  ;; Draw a line of text into the view
  (define (plain-text text-view hdc line-no rect)
    ;; TODO chop off the line according to the regex or something
    ;;      to do syntax highlighting
    (let ((line (win32-text-view-buffer-line (~ text-view 'value) line-no)))
      (let ((left (text-out text-view hdc rect line #f))
	    (fill (allocate-c-struct RECT)))
	;; erase the rest of the line with background color
	(c-memcpy fill 0 rect 0 (size-of-c-struct RECT))
	(c-struct-set! fill RECT 'left left)
	(ext-text-out hdc 0 0 ETO_OPAQUE fill null-pointer 0 0))))

  (define (paint-line text-view hdc line-no)
    (define hwnd (~ text-view 'hwnd))
    (define font-height (~ text-view 'font-height))
    (define font-width (~ text-view 'font-width))

    (let ((rect (allocate-c-struct RECT)))
      (get-client-rect hwnd rect)
      (let* ((hpos (~ text-view 'horizontal-scroll-position))
	     (vpos (~ text-view 'vertical-scroll-position))
	     (top (* (- line-no vpos) font-height)))
	(c-struct-set! rect RECT 'left (* (- hpos) font-width))
	(c-struct-set! rect RECT 'top top)
	(c-struct-set! rect RECT 'bottom (+ top font-height)))
      (if (>= line-no (win32-text-view-buffer-line-count (~ text-view 'value)))
	  (begin
	    ;; for now
	    (set-bk-color hdc (get-color text-view 'background))
	    (ext-text-out hdc 0 0 ETO_OPAQUE rect null-pointer 0 0))
	  (plain-text text-view hdc line-no rect))))

  (let ((ps (allocate-c-struct PAINTSTRUCT))
	(hwnd (~ text-view 'hwnd)))
    (begin-paint hwnd ps)
    (let* ((height (~ text-view 'font-height))
	   (hdc (c-struct-ref ps PAINTSTRUCT 'hdc))
	   (vpos (~ text-view 'vertical-scroll-position))
	   (first (+ vpos (div (c-struct-ref ps PAINTSTRUCT 'rcPaint.top)
			       height)))
	   (last (+ vpos (div (c-struct-ref ps PAINTSTRUCT 'rcPaint.bottom)
			      height))))
      (when (<= first last)
	(do ((i first (+ i 1)))
	    ((>= i last))
	  (paint-line text-view hdc i))))
    (end-paint hwnd ps)))

(define (setup-scroll-bars text-view)
  (let ((si (allocate-c-struct SCROLLINFO)))
    ;; vertical scroll bar
    (c-struct-set! si SCROLLINFO 'cbSize (size-of-c-struct SCROLLINFO))
    (c-struct-set! si SCROLLINFO 'fMask 
		   (bitwise-ior SIF_PAGE SIF_POS SIF_RANGE SIF_DISABLENOSCROLL))
    (c-struct-set! si SCROLLINFO 'nPos (~ text-view 'vertical-scroll-position))
    (c-struct-set! si SCROLLINFO 'nPage (~ text-view 'window-lines))
    (c-struct-set! si SCROLLINFO 'nMin 0)
    (c-struct-set! si SCROLLINFO 'nMax 
		   (- (win32-text-view-buffer-line-count (~ text-view 'value)) 1))
    (set-scroll-info (~ text-view 'hwnd) SB_VERT si #t)
    ;; horizontal scroll bar
    (c-struct-set! si SCROLLINFO 'nPos 
		   (~ text-view 'horizontal-scroll-position))
    (c-struct-set! si SCROLLINFO 'nPage (~ text-view 'window-columns))
    (c-struct-set! si SCROLLINFO 'nMin 0)
    (c-struct-set! si SCROLLINFO 'nMax 
      (- (win32-text-view-buffer-longest-line (~ text-view 'value)) 1))
    (set-scroll-info (~ text-view 'hwnd) SB_HORZ si #t)
    
    ;; adjust
    (set! (~ text-view 'vertical-scroll-max)
	  (- (win32-text-view-buffer-line-count (~ text-view 'value)) 
	     (~ text-view 'window-lines)))
    (set! (~ text-view 'horizontal-scroll-max)
	  (- (win32-text-view-buffer-longest-line (~ text-view 'value)) 
	     (~ text-view 'window-columns)))
    ))
(define (handle-size text-view flags width height)
  (define (pin-to-bottom-corner text-view)
    (cond ((> (+ (~ text-view 'horizontal-scroll-position)
		 (~ text-view 'window-columns))
	      (win32-text-view-buffer-longest-line (~ text-view 'value))))
	  ((> (+ (~ text-view 'vertical-scroll-position)
		 (~ text-view 'window-lines))
	      (win32-text-view-buffer-line-count (~ text-view 'value))))
	  (else #f)))
  ;; TODO left margin?
  (let ((lines (min (div height (~ text-view 'font-height))
		    (win32-text-view-buffer-line-count (~ text-view 'value))))
	(columns (min (div width (~ text-view 'font-width))
		      (win32-text-view-buffer-longest-line 
		       (~ text-view 'value)))))
    (set! (~ text-view 'window-lines) lines)
    (set! (~ text-view 'window-columns) columns)
    (when (pin-to-bottom-corner text-view) (refresh-window text-view))
    (setup-scroll-bars text-view)
    0))

(define (handle-set-font text-view hfont) 
  (set! (~ text-view 'font) hfont)
  (win32-edit-update-font text-view)
  (update-metrics text-view))

(define (scroll text-view dx dy)
  (define (ensure-amount a pos max)
    (cond ((negative? a) (- (min (abs a) (~ text-view pos))))
	  ((positive? a) (min a (- (~ text-view max) (~ text-view pos))))
	  (else 0)))
  (let ((dy (ensure-amount dy 'vertical-scroll-position 'vertical-scroll-max))
	(dx (ensure-amount dx 'horizontal-scroll-position 
			   'horizontal-scroll-max))
	(hpos (~ text-view 'horizontal-scroll-position))
	(vpos (~ text-view 'vertical-scroll-position)))
    (set! (~ text-view 'horizontal-scroll-position) (+ hpos dx))
    (set! (~ text-view 'vertical-scroll-position) (+ vpos dy))

    (unless (and (zero? dx) (zero? dy))
      (scroll-window-ex (~ text-view 'hwnd)
			(* (- dx) (~ text-view 'font-width))
			(* (- dy) (~ text-view 'font-height))
			null-pointer
			null-pointer
			null-pointer
			null-pointer
			SW_INVALIDATE)
      (setup-scroll-bars text-view))))

(define (get-track-pos text-view bar)
  (let ((si (allocate-c-struct SCROLLINFO)))
    (c-struct-set! si SCROLLINFO 'cbSize (size-of-c-struct SCROLLINFO))
    (c-struct-set! si SCROLLINFO 'fMask SIF_TRACKPOS)
    (get-scroll-info (~ text-view 'hwnd) bar si)
    (c-struct-ref si SCROLLINFO 'nTrackPos)))
(define (handle-vscroll text-view code pos)
  (let ((old (~ text-view 'vertical-scroll-position)))
    (cond ((= code SB_TOP)
	   (set! (~ text-view 'vertical-scroll-position) 0)
	   (refresh-window text-view))
	  ((= code SB_BOTTOM)
	   (set! (~ text-view 'vertical-scroll-position) 
		 (~ text-view 'vertical-scroll-max))
	   (refresh-window text-view))
	  ((= code SB_LINEUP) (scroll text-view 0 -1))
	  ((= code SB_LINEDOWN) (scroll text-view 0 1))
	  ((= code SB_PAGEDOWN)
	   (scroll text-view 0 (~ text-view 'window-lines)))
	  ((= code SB_PAGEUP) 
	   (scroll text-view 0 (- (~ text-view 'window-lines))))
	  ((or (= code SB_THUMBPOSITION)
	       (= code SB_THUMBTRACK))
	   (set! (~ text-view 'vertical-scroll-position) 
		 (get-track-pos text-view SB_VERT))
	   (refresh-window text-view)))
    (unless (= old (~ text-view 'vertical-scroll-position))
      (setup-scroll-bars text-view))
    0))
(define (handle-hscroll text-view code pos)
  (let ((old (~ text-view 'horizontal-scroll-position)))
    (cond ((= code SB_LEFT)
	   (set! (~ text-view 'horizontal-scroll-position) 0)
	   (refresh-window text-view))
	  ((= code SB_RIGHT)
	   (set! (~ text-view 'horizontal-scroll-position) 
		 (~ text-view 'horizontal-scroll-max))
	   (refresh-window text-view))
	  ((= code SB_LINELEFT) (scroll text-view -1 0))
	  ((= code SB_LINERIGHT) (scroll text-view 1 0))
	  ((= code SB_PAGELEFT)
	   (scroll text-view (- (~ text-view 'window-columns)) 0))
	  ((= code SB_PAGERIGHT) 
	   (scroll text-view (~ text-view 'window-columns) 0))
	  ((or (= code SB_THUMBPOSITION)
	       (= code SB_THUMBTRACK))
	   (set! (~ text-view 'horizontal-scroll-position)
		 (get-track-pos text-view SB_HORZ))
	   (refresh-window text-view)))
    (unless (= old (~ text-view 'horizontal-scroll-position))
      (setup-scroll-bars text-view))
    0))

(define (handle-mouse-wheel text-view delta)
  (define wheel-delta 120) ;; from MSDN
  (let ((scroll-lines (empty-pointer)))
    (system-parameters-info SPI_GETWHEELSCROLLLINES 0 (address scroll-lines) 0)
    (let* ((lines (pointer->integer scroll-lines))
	   (n (if (<= lines 1) 3 lines)))
      (scroll text-view 0 (* (div (- delta) wheel-delta) n))
      0)))

(define (->short n)
  (if (> n #x7FFF)
      (- n #x10000)
      n))

(define (handle-mouse-activate text-view hwnd hit-test message)
  (set-focus (~ text-view 'hwnd))
  MA_ACTIVATE)

;; FIXME
;; For now we assume the font has the same width (mono font)
;; so that we can easily calculate tab width as well
(define (mouse-coord-to-file-pos text-view mx my)
  (define font-height (~ text-view 'font-height))
  (define font-width (~ text-view 'font-width))

  (let ((rect (allocate-c-struct RECT)))
    (get-client-rect (~ text-view 'hwnd) rect)
    (let* ((rb (c-struct-ref rect RECT 'bottom))
	   (bottom (- rb (mod rb font-height)))
	   (right (c-struct-ref rect RECT 'right)))
      ;;(c-struct-set! rect RECT 'bottom bottom)
      (let ((mx (cond ((> mx right) (- right 1))
		      ((< mx 0) 0)
		      (else mx)))
	    (my (cond ((> my bottom) (- bottom 1))
		      ((< my 0) 0)
		      (else my)))
	    (buf (~ text-view 'value)))
	(let-values (((line-no end?)
		      (let ((n (+ (div my font-height) 
				  (~ text-view 'vertical-scroll-position)))
			    (size (win32-text-view-buffer-line-count buf)))
			(if (>= n size)
			    (values (if (zero? size) 0 (- size 1)) #t)
			    (values n #f)))))
	  (define (tab-count line len)
	    (define (tab i) (if (eqv? #\tab (string-ref line i)) 1 0))
	    (do ((i 0 (+ i 1)) (c 0 (+ c (tab i))))
		((= i len) c)))
	  (let* ((line (win32-text-view-buffer-line buf line-no))
		 (len (string-length line))
		 (tabs (tab-count line len))
		 (cx (+ (* len font-width)
			(* tabs (- (~ text-view 'tab-width) 1) font-width))))
	    (if (> mx cx)
		(values line-no len cx) ;; easy
		;; need to calculate tab otherwise caret would be
		;; in the middle of the tab...
		;; TODO
		(let ((off (mod mx font-width))
		      (half (div font-width 2)))
		  (values line-no 0 
			  (if (> off half)
			      (+ mx (- font-width off))
			      (- mx off))))))
	  ;; TODO in the end we may need this type of thing
	  #;
	  (let* ((line (win32-text-view-buffer-line buf line-no))
		 (size (allocate-c-struct SIZE))
		 (hdc (get-dc (~ text-view 'hwnd)))
		 (old (select-object hdc (~ text-view 'font))))
	    (get-text-extend-point-32 hdc line (string-length line) size)
	    (select-object hdc old)
	    (let ((cx (c-struct-ref size SIZE 'cx))
		  (cy (c-struct-ref size SIZE 'cy)))
	      (if (> mx cx)
		  (begin
		    (display mx) (display ":") (display cx) (newline)
		    (values line-no 0 cx)
		    )
		  (let ((off (mod mx font-width))
			(half (div font-width 2)))
		    (values line-no 0 
			    (if (> off half)
				(+ mx (- font-width off))
				(- mx off))))))))))))
	  
    

(define (handle-lbutton-down text-view flags mx my)
  (let-values (((line-no file-off px) 
		(mouse-coord-to-file-pos text-view mx my)))
    (set-caret-pos px (* (- line-no (~ text-view 'vertical-scroll-position))
			 (~ text-view 'font-height)))
    (set! (~ text-view 'mouse-down) #t)
    (set! (~ text-view 'selection-start) file-off)
    (set! (~ text-view 'selection-end) file-off)
    (set! (~ text-view 'cursor-offset) file-off)
    (set-capture (~ text-view 'hwnd))
    (refresh-window text-view)
    0))

(define (handle-lbutton-up text-view flags mx my)
  (when (~ text-view 'mouse-down)
    (set! (~ text-view 'mouse-down) #f)
    (release-capture)
    (refresh-window text-view))
  0)

(define (handle-mouse-move text-view flags mx my)
  (when (~ text-view 'mouse-down)
    (let-values (((line-no file-off px) 
		  (mouse-coord-to-file-pos text-view mx my)))
      (unless (= (~ text-view 'selection-end) file-off)
	;; (invalidate-range text-view (~ text-view 'selection-end) file-off)
	(set-caret-pos px (* (- line-no (~ text-view 'vertical-scroll-position))
			     (~ text-view 'font-height)))
	(set! (~ text-view 'selection-end) file-off)
	(set! (~ text-view 'cursor-offset) file-off))))
  0)

(define (handle-set-focus text-view hwnd-old)
  (define hwnd (~ text-view 'hwnd))
  (create-caret hwnd null-pointer 2 (~ text-view 'font-height))
  ;; (reposition-caret text-view) ;; TODO
  (show-caret hwnd)
  (refresh-window text-view)
  0)

(define (handle-kill-focus text-view hwnd-old)
  (hide-caret (~ text-view 'hwnd))
  (destroy-caret)
  (refresh-window text-view)
  0)

(define (default-text-view-proc hwnd imsg wparam lparam)
  (cond ((= imsg WM_NCCREATE)
	 ;; save the lpCreateParams of CREATESTRUCT
	 (let ((w (c-struct-ref lparam CREATESTRUCT 'lpCreateParams)))
	   (set-window-long-ptr hwnd GWLP_USERDATA w)
	   ;; setup what we required
	   (let ((tw (pointer->object w)))
	     (set! (~ tw 'hwnd) hwnd)
	     (unless (slot-bound? tw 'font-height) 
	       (win32-edit-update-font tw))
	     (setup-scroll-bars tw))
	   1))
	((= imsg WM_SIZE)
	 (let ((s (pointer->integer lparam))
	       (w (win32-get-component hwnd)))
	   (handle-size w wparam (win32-loword s) (win32-hiword s))))
	((= imsg WM_PAINT) (handle-paint (win32-get-component hwnd)) 1)
	((= imsg WM_SETFONT)
	 (handle-set-font (win32-get-component hwnd) wparam) 1)
	((= imsg WM_VSCROLL) 
	 (handle-vscroll (win32-get-component hwnd)
			 (win32-loword wparam)
			 (win32-hiword wparam)))
	((= imsg WM_HSCROLL)
	 (handle-hscroll (win32-get-component hwnd)
			 (win32-loword wparam)
			 (win32-hiword wparam)))
	((= imsg WM_MOUSEWHEEL)
	 (handle-mouse-wheel (win32-get-component hwnd)
			     ;; we need to consider negative value
			     ;; of wparam 
			     (->short (win32-hiword wparam))))
	((= imsg WM_MOUSEACTIVATE)
	 (let ((l (pointer->integer lparam)))
	   (handle-mouse-activate (win32-get-component hwnd)
				  (integer->pointer wparam)
				  (win32-loword l)
				  (win32-hiword l))))
	((= imsg WM_SETFOCUS)
	 (handle-set-focus (win32-get-component hwnd)
			   (integer->pointer wparam)))
	((= imsg WM_KILLFOCUS)
	 (handle-kill-focus (win32-get-component hwnd)
			   (integer->pointer wparam)))
	((= imsg WM_LBUTTONDOWN)
	 (let ((l (pointer->integer lparam)))
	   (handle-lbutton-down (win32-get-component hwnd)
				wparam
				(->short (win32-loword l))
				(->short (win32-hiword l)))))
	((= imsg WM_LBUTTONUP)
	 (let ((l (pointer->integer lparam)))
	   (handle-lbutton-up (win32-get-component hwnd)
			      wparam
			      (->short (win32-loword l))
			      (->short (win32-hiword l)))))
	((= imsg WM_MOUSEMOVE)
	 (let ((l (pointer->integer lparam)))
	   (handle-mouse-move (win32-get-component hwnd)
			      wparam
			      (->short (win32-loword l))
			      (->short (win32-hiword l)))))
	((win32-common-dispatch hwnd imsg wparam lparam) 1)
	(else (def-window-proc hwnd imsg wparam lparam))))

(define *text-view-proc*
  (c-callback void* (HWND unsigned-int WPARAM LPARAM) default-text-view-proc))
(define win32-default-text-view-class
  (let ((c (make <win32-window-class>
	     :name *win32-default-text-view-class-name*
	     :window-proc *text-view-proc*
	     :background null-pointer ;; no flickering for us
	     :cursor (load-cursor null-pointer IDC_IBEAM))))
    (win32-register-class c)))

;; text-view buffer
;; TODO proper implementation
(define-class <win32-text-view-buffer> ()
  ((count :init-keyword :count :init-value 0)
   (longest-line :init-keyword :longest-line :init-value 0)
   (lines :init-keyword :lines :init-value '())
   (size :init-keyword :size :init-value 0)))
(define (win32-text-view-buffer-line-count b) (~ b 'count))
(define (win32-text-view-buffer-longest-line b) (~ b 'longest-line))
(define (win32-text-view-buffer-line b n) (list-ref (~ b 'lines) n))
(define (win32-text-view-buffer-size b) (~ b 'size))
)
    