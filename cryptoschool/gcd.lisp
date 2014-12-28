(defun gcd1 (a b) (if (= b 0) a (gcd1 b (mod a b))))
(defun gcd2 (a b) (if (= b 0) (list 1 0) (let ((ret (gcd2 b (mod a b)))) (list (cadr ret) (- (car ret) (* (floor a b) (cadr ret)))))))
