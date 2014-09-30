#|
Michael Clark
HW-26
ged@byu.net
|#

;;================================================================================
;;==                         Lecture 25 Interpreter                             ==
;;================================================================================

;;================================================================================
;;==                               Top Level                                    ==
;;================================================================================

(define run
  (lambda (string)
    (begin (execute-program (scan&parse string))
           'program_done)))

;;================================================================================
;;==                       Grammatical Specification                            ==
;;================================================================================

(define the-lexical-spec
  '((whitespace (whitespace)                                   skip)
    (comment    ("%" (arbno (not #\newline)))                  skip)
    (identifier (letter (arbno (or letter digit "_" "-" "?"))) symbol)
    (number     (digit (arbno digit))                          number)))

(define the-grammar
  '((program (statement) a-program)

    (statement (identifier "=" expression)                           assign-statement)
    (statement ("print" "(" expression ")")                          print-statement)
    (statement ("{" (separated-list statement ";") "}")              compound-statement)
    (statement ("if" expression statement statement)                 if-statement)
    (statement ("while" expression "do" statement)                   while-statement)
    (statement ("var" (separated-list identifier "=" expression ",") ";" statement) block-statement)
    (statement ("setall" "{" (separated-list identifier ",") "}" "="
                         "{" (separated-list expression ",") "}")
               multassign-statement)
 
      (expression (number)                                                    lit-exp)
      (expression (identifier)                                                var-exp)
      (expression (primitive "(" (separated-list expression ",") ")")         primapp-exp)
      (expression ("if" expression "then" expression "else" expression)       if-exp)
      (expression ("let" (arbno identifier "=" expression) "in" expression)   let-exp)
      (expression ("proc" "(" (separated-list identifier ",") ")" expression) proc-exp)
      (expression ("(" expression (arbno expression) ")")                     app-exp)
      (expression ("letrec" (arbno identifier "(" (separated-list identifier ",") ")" "=" expression)
                   "in" expression)                                           letrec-exp)
      (expression ("set" identifier "=" expression)                           varassign-exp)
      (expression ("begin" expression (arbno ";" expression) "end")           begin-exp)
 
      (primitive ("+")        add-prim)
      (primitive ("-")        subtract-prim)
      (primitive ("*")        mult-prim)
      (primitive ("add1")     incr-prim)
      (primitive ("sub1")     decr-prim)
      (primitive ("equal?")   equal-prim)
      (primitive ("zero?")    zero-prim)
      (primitive ("greater?") greater-prim)
      (primitive ("less?")    less-prim)
      ))
 
  (sllgen:make-define-datatypes the-lexical-spec the-grammar)
 
  (define show-the-datatypes
    (lambda ()
      (sllgen:list-define-datatypes the-lexical-spec the-grammar)))
 
  (define scan&parse
    (sllgen:make-string-parser the-lexical-spec the-grammar))
 
  (define just-scan
    (sllgen:make-string-scanner the-lexical-spec the-grammar))
 
  ;;================================================================================
  ;;==                          Internal Datatypes                                ==
  ;;================================================================================
 
  (define-datatype reference reference?
    (a-ref (position integer?)
           (vec vector?)))
 
  ;;================================================================================
  ;;==                            The Interpreter                                 ==
  ;;================================================================================
 
  (define execute-program
    (lambda (pgm)
      (cases program pgm
             (a-program (statement) (execute-statement statement (init-env))))))
 
  (define execute-statement
    (lambda (stmt env)
      (cases statement stmt
             (assign-statement   (id exp)                   (setref! (apply-env-ref env id)
                                                                     (eval-expression exp env)))
             (print-statement    (exp)                      (begin (write (eval-expression exp env))
                                                                   (newline)))
             (compound-statement (statements)               (for-each (lambda (statement)
                                                                        (execute-statement statement env))
                                                                      statements))
             (if-statement       (exp true-stat false-stat) (if (true-value? (eval-expression exp env))
                                                                (execute-statement true-stat env)
                                                                (execute-statement false-stat env)))
             (while-statement    (exp statement)            (let loop ()
                                                              (if (true-value? (eval-expression exp env))
                                                                  (begin (execute-statement statement env)
                                                                         (loop)))))
             (block-statement    (ids exps statement) (execute-statement statement
                                                        (extend-env ids
                                                            (map (lambda (x) (eval-expression x env)) exps) env)))
             (multassign-statement (loi loe) (map (lambda (x y) (setref! (apply-env-ref env x)
                                                                         y))
                                                  loi
                                                  (map (lambda (x) (eval-expression x env))
                                                       loe)))
             )))
 
  (define eval-expression
    (lambda (exp env)
      (cases expression exp
             (lit-exp       (datum)                       datum)
             (var-exp       (id)                          (apply-env env id))
             (primapp-exp   (prim rands)                  (let ((args (eval-rands rands env)))
                                                            (apply-primitive prim args)))
             (if-exp        (test-exp true-exp false-exp) (if (true-value? (eval-expression test-exp env))
                                                              (eval-expression true-exp env)
                                                              (eval-expression false-exp env)))
             (let-exp       (ids rands body)              (let ((args (eval-rands rands env)))
                                                            (eval-expression body (extend-env ids args env))))
             (proc-exp      (ids body)                    (closure ids body env))
             (app-exp       (rator rands)                 (let ((proc (eval-expression rator env))
                                                                (args (eval-rands rands env)))
                                                            (if (procval? proc)
                                                                (apply-procval proc args)
                                                                (eopl:error 'eval-expression "Attempt to apply non-procedure ~s" proc))))
             (letrec-exp    (proc-names idss bodies letrec-body)
                            (eval-expression letrec-body
                                             (extend-env-recursively proc-names idss bodies env)))
             (varassign-exp (id rhs-exp)                  (begin (setref! (apply-env-ref env id)
                                                                          (eval-expression rhs-exp env))
                                                                 1))
             (begin-exp     (exp1 exps)                   (letrec ((loop (lambda (acc exps)
                                                                           (if (null? exps) acc
                                                                               (loop (eval-expression (car exps) env)
                                                                                     (cdr exps))))))
                                                            (loop (eval-expression exp1 env) exps)))
             (else                                        (eopl:error 'eval-expression "Not here:~s" exp))
             )))
 
  (define eval-rands
    (lambda (rands env)
      (map (lambda (x) (eval-rand x env)) rands)))
 
  (define eval-rand
    (lambda (rand env)
      (eval-expression rand env)))
 
  (define apply-primitive
    (lambda (prim args)
      (let ((check (lambda (arity)
                     (if (not (= arity (length args)))
                         (eopl:error 'apply-primitive "wrong number of arguments to primitive ~s" prim)))))
        (cases primitive prim
               (add-prim      () (begin (check 2)
                                        (+ (car args) (cadr args))))
               (subtract-prim () (begin (check 2)
                                        (- (car args) (cadr args))))
               (mult-prim     () (begin (check 2)
                                        (* (car args) (cadr args))))
               (incr-prim     () (begin (check 1)
                                        (+ (car args) 1)))
               (decr-prim     () (begin (check 1)
                                        (- (car args) 1)))
               (equal-prim    () (begin (check 2)
                                        (if (= (car args) (cadr args)) truth falsity)))
               (zero-prim     () (begin (check 1)
                                        (if (zero? (car args)) truth falsity)))
               (greater-prim  () (begin (check 2)
                                        (if (  (car args) (cadr args)) truth falsity)))
               (less-prim     () (begin (check 2)
                                        (if (< (car args) (cadr args)) truth falsity)))
             ))))
 
  ;;================================================================================
  ;;==                               Booleans                                     ==
  ;;================================================================================
 
  (define truth 1)
  (define falsity 0)
 
  (define true-value?
    (lambda (x)
      (not (zero? x))))
 
  ;;================================================================================
  ;;==                              Procedures                                    ==
  ;;================================================================================
 
  (define-datatype procval procval?
    (closure (ids (list-of symbol?))
             (body expression?)
             (env environment?)))
 
  (define apply-procval
    (lambda (proc args)
      (cases procval proc
             (closure (ids body env) (if (= (length ids) (length args))
                                         (eval-expression body (extend-env ids args env))
                                         (eopl:error 'apply-procval "Wrong number of arguments to procedure ~s" ids))))))
 
  ;;================================================================================
  ;;==                              References                                    ==
  ;;================================================================================
 
  (define primitive-deref
    (lambda (ref)
      (cases reference ref
             (a-ref (pos vec) (vector-ref vec pos)))))
 
  (define primitive-setref!
    (lambda (ref val)
      (cases reference ref
             (a-ref (pos vec) (vector-set! vec pos val)))))
 
  (define deref
    (lambda (ref)
      (primitive-deref ref)))
 
  (define setref!
    (lambda (ref val)
      (primitive-setref! ref val)))
 
  ;;================================================================================
  ;;==                             Environments                                   ==
  ;;================================================================================

(define init-env
  (lambda ()
    (extend-env '(stdout)
                '(*uninitialized*)
                (empty-env))))

(define-datatype environment environment?
  (empty-env-record)
  (extended-env-record (syms (list-of symbol?))
                       (vals vector?)
                       (env environment?)))

(define empty-env
  (lambda ()
    (empty-env-record)))

(define extend-env
  (lambda (syms vals env)
    (extended-env-record syms (list->vector vals) env)))

(define extend-env-recursively
  (lambda (proc-names idss bodies old-env)
    (let ((len (length proc-names)))
      (let ((vec (make-vector len)))
        (let ((env (extended-env-record proc-names vec old-env)))
          (for-each (lambda (pos ids body)
                      (vector-set! vec pos (closure ids body env)))
                    (iota len)
                    idss
                    bodies)
          env)))))

(define iota
  (lambda (end)
    (let loop ((next 0))
      (if (= next end)
          '()
          (cons next (loop (+ 1 next)))))))

(define apply-env
  (lambda (env sym)
    (deref (apply-env-ref env sym))))

(define apply-env-ref
  (lambda (env sym)
    (cases environment env
           (empty-env-record    ()              (eopl:error 'apply-env-ref "No binding for ~s" sym))
           (extended-env-record (syms vals env) (let ((position (rib-find-position sym syms)))
                                                  (if (number? position)
                                                      (a-ref position vals)
                                                      (apply-env-ref env sym)))))))

(define rib-find-position
  (lambda (sym los)
    (list-find-position sym los)))

(define list-find-position
  (lambda (sym los)
    (list-index (lambda (sym1) (eqv? sym1 sym)) los)))

(define list-index
  (lambda (pred ls)
    (cond ((null? ls)      #f)
          ((pred (car ls)) 0)
          (else            (let ((list-index-r (list-index pred (cdr ls))))
                             (if (number? list-index-r)
                                 (+ list-index-r 1)
                                 #f))))))


;;; Tests
;#|
(run "var x = 1, y = *( 3, 4 ), z = 4;
    {
        setall {x, y, z} = {y, x, 3};
        print( x );
        print( y );
        print( z )
    }")
;|#

;;; Q/A
;; No, they aren't in scope for the others (at least not the way I wrote it)