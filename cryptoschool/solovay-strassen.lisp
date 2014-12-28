(defun pow-mod (num exp modulus) (if (> exp 0) (mod (if (= 1 (mod exp 2)) (* num (expt (pow-mod num (floor (- exp 1) 2) modulus) 2)) (expt (pow-mod num (floor exp 2) modulus) 2)) modulus) 1))

(defun jacobi (a n) (cond ((= a 1) 1) ((= a 2) (if (member (mod n 8) '(1 7)) 1 -1)) ((/= 1 (gcd a n)) 0) (t (setf a (mod a n)) (let ((twos (log (max 1 (boole boole-andc2 a (- a 1))) 2))) (setf a (ash a (- twos))) (* (if (or (member (mod n 8) '(1 7)) (= 0 (mod twos 2))) 1 -1) (if (= 3 (mod a 4) (mod n 4)) -1 1) (jacobi n a))))))

(defun ss-prime (n k) (if (= k 0) 'probably-prime (let* ((a (+ 2 (random (- n 2)))) (x (jacobi a n))) (if (or (= x 0) (/= (pow-mod a (/ (- n 1) 2) n) (if (= x -1) (+ n x) x))) 'composite (ss-prime n (- k 1))))))

(let ((primes '()) (prime (+ (expt 10 12) 1))) (loop while (< (list-length primes) 1000) do (if (eq (ss-prime prime 50) 'probably-prime) (push prime primes)) (incf prime 2)) (reverse primes))
