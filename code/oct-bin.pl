#!/usr/bin/perl

while (($a = getc) || ($a >= 0 && $a <= 7))  {
	last if($a eq "\n");
	print ((($a & 4) == 0)?0:1);
	print ((($a & 2) == 0)?0:1);
	print ((($a & 1) == 0)?0:1);
}

