#!/usr/bin/perl

$string = $ARGV[0];
$key = $ARGV[1];

#print $string . "\n";

for($i = 0; $i < length $string; $i++) {
	print substr($string,$i,1);
	for($j = 1; $j < $key; $j++) {
		#$c = int (rand(52));
		$c = int (rand(26));

		#$c += 64;
		$c += 97;

		#$c += 6 if ($c > 90);

		printf("%c", $c);
	}
}
