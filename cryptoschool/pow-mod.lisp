(defun pow-mod-1 (num exp modulus)
  (if (> exp 0)
      (mod (if (= 1 (mod exp 2))
	       (* num (expt (pow-mod-1 num (floor (- exp 1) 2) modulus) 2))
	       (expt (pow-mod-1 num (floor exp 2) modulus) 2))
	   modulus)
      1))
(pow-mod-1 4 13 497) ; 445

(defun pow-mod-2 (base exp modulus &optional (acc 1))
  (if (> exp 0)
      (pow-mod-2 (mod (* base base) modulus)
	       (ash exp -1)
	       modulus
	       (if (= 1 (mod exp 2)) (mod (* base acc) modulus) acc))
      acc))
(pow-mod-2 4 13 497) ; 445
