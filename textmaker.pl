#!/usr/bin/perl

for($i = 0; $i < 20; $i++) {
	$c = int (rand(62));
	
	$c += 48;
	
	$c += 7 if ($c > 57);
	$c += 6 if ($c > 90);
	
	printf("%c", $c);
}
