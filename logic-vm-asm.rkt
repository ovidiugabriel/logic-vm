#lang racket

;; 🇪🇪
;; logic-vm assembler utility

(require racket/function)
(require data/enumerate/lib)
(require binaryio)

(define (for-each-line callback out)
  (thunk (for ([line (in-lines)])
     (callback line out) ) ) )

(define (assemble-file file-path binary-path)
  (let ([out (open-output-file binary-path #:exists 'replace)])
    (with-input-from-file file-path
      (for-each-line assemble-line out) )
    (close-output-port out) ) )

(define (filter-spaces lst)
  (filter (λ (x) (not (equal? "" x))) lst) )

;; enum of operations
(define operations
  (fin/e "init" "mov" "not" "and" "or" "orpos") )

;; enum of hexadecimal digits
(define hex
  (fin/e #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9 #\a #\b #\c #\d #\e #\f))

(define (hexstring->integer value)
  (let ([chars (string->list value)])
    (+ (* (to-nat hex (first chars)) 16) (to-nat hex (second chars))) ) )

(define (assemble-line line out)
  (let ([parts (filter-spaces (string-split line " "))])    
    (write-line
     (append (list (to-nat operations (first parts)))
             (map (λ (x) (hexstring->integer (substring x 2)))
                  (rest parts) ) ) out) ) )

(define (write-line integer-list out)
  (write-bytes (list->bytes integer-list) out))

(assemble-file "logic.asm" "logic.bin")
