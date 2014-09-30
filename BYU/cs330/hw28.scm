#|
Michael Clark
HW-28
ged@byu.net
|#

#|
;; '+' type signatures
(boolean * boolean -> illegal)
(boolean * char -> illegal)
(boolean * byte -> illegal)
(boolean * short -> illegal)
(boolean * int -> illegal)
(boolean * long -> illegal)
(boolean * float -> illegal)
(boolean * double -> illegal)
(char * boolean -> illegal)
(char * char -> int)
(char * byte -> int)
(char * short -> int)
(char * int -> int)
(char * long -> long)
(char * float -> float)
(char * double -> double)
(byte * boolean -> illegal)
(byte * char -> int)
(byte * byte -> int)
(byte * short -> int)
(byte * int -> int)
(byte * long -> long)
(byte * float -> float)
(byte * double -> double)
(short * boolean -> illegal)
(short * char -> int)
(short * byte -> int)
(short * short -> int)
(short * int -> int)
(short * long -> long)
(short * float -> float)
(short * double -> double)
(int * boolean -> illegal)
(int * char -> int)
(int * byte -> int)
(int * short -> int)
(int * int -> int)
(int * long -> long)
(int * float -> float)
(int * double -> double)
(long * boolean -> illegal)
(long * char -> long)
(long * byte -> long)
(long * short -> long)
(long * int -> long)
(long * long -> long)
(long * float -> float)
(long * double -> double)
(float * boolean -> illegal)
(float * char -> float)
(float * byte -> float)
(float * short -> float)
(float * int -> float)
(float * long -> float)
(float * float -> float)
(float * double -> double)
(double * boolean -> illegal)
(double * char -> double)
(double * byte -> double)
(double * short -> double)
(double * int -> double)
(double * long -> double)
(double * float -> double)
(double * double -> double)

Ennumeration of all the primitives.  The strings are all (string * thing -> string)

All the primitives I suspect are special cases.  The classes probably have something like operator overloading built into them, but have language support for it as well (since a user can't overload operators afaik in java).

Any others, I would either need friendly operator overloading, or the source code to a java compiler (and a lot of free time).
|#

#|
(define lot '(boolean char byte short int long float double))
(define var '(b c by s i l f d))

(define cart (lambda (cart1 cart2)
            (if (null? cart1) '() (cons (ch (car cart1) cart2) (cart (cdr cart1) cart2)))))

(define ch (lambda (val cart)
             (if (null? cart) '() (cons (list val (car cart)) (ch val (cdr cart))))))

(define all (cart lot lot))
(define varall (cart var var))

(define niceprint1
  (lambda (3list)
    (if (null? 3list) '() (begin (niceprint2 (car 3list)) (niceprint1 (cdr 3list))))))
(define niceprint2
  (lambda (2list)
    (if (null? 2list) '() 
        ;(begin (niceprint3 (car 2list)) (niceprint2 (cdr 2list)))
        (begin (niceprint4 (caar 2list) (cadar 2list)) (niceprint2 (cdr 2list)))
        )))
(define niceprint3
  (lambda (1list)
    (begin (display (list (car 1list) '* (cadr 1list) '-> '?)) (newline))))
(define niceprint4
  (lambda (v1 v2)
    (begin (write (list 'System.out.println (list
                        (string->symbol (list->string (append (string->list (symbol->string v1)) '(#\1))))
                        `+ 
                        (string->symbol (list->string (append (string->list (symbol->string v2)) '(#\2)))))
                          `semicolon)) (newline))))
|#
