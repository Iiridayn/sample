(defun euler-totient-coprime (num a b)
	(if (/= (gcd a b) 1) 'error
		(* num (- 1 (/ 1 a)) (- 1 (/ 1 b)))))
(euler-totient-coprime 36 2 3) ; 12 - 2*2*3*3 = 36.
(euler-totient-coprime 437 19 23) ; 396 - http://primefan.tripod.com/Phi500.html

(defun euler-totient-coprime (num &rest factors)
	(reduce #'* (cons num
		(mapcar #'(lambda (x) (- 1 (/ 1 x)))
		factors))))
(euler-totient-coprime 435 3 5 29) ; 224
