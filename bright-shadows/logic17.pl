#!/usr/bin/perl

my @number = (1,3,4,6);
my @op = ('+', '-', '/', '*');
my @found;

# i,j,k,l - op permutation (1-4th op)
for(my $i = 0; $i < 4; $i++) {
for(my $j = 0; $j < 4; $j++) {
for(my $k = 0; $k < 4; $k++) {
#for(my $l = 0; $l < 4; $l++) {
	my $number = 0;
	
	# m,n,o,p number combinations
	for(my $m = 0; $m < 4; $m++) {
	for(my $n = 0; $n < 4; $n++) { next if ($n == $m);
	for(my $o = 0; $o < 4; $o++) { next if (($o == $m) || ($o == $n));
	for(my $p = 0; $p < 4; $p++)
	{
		next if (($p == $m) || ($p == $n) || ($p == $o));
		for($a = 0; $a < 2; $a++) {
			my $math;
			if($a == 0) {
				$math = "(" . $number[$m] . $op[$i] . $number[$n] . ")" . $op[$j] . "(" . $number[$o] . $op[$k] . $number[$p] . ")" ;#. $op[$l];
			}
			else {
				$math = "((" . $number[$m] . $op[$i] . $number[$n] . ")" . $op[$j] . $number[$o] . ")" . $op[$k] . $number[$p] ;#. $op[$l];
			}
		#my $math = $number[$m] . $op[$i] . $number[$n] . $op[$j] . $number[$o] . $op[$k] . $number[$p] ;#. $op[$l];
			$number = eval($math);
			push(@found, $number);
		#print $number . "\n";
			print $math . "=" . $number . "\n" if (abs($number - 24) <= 2);
		}
	}}}}
}}}#}

@found = sort {$a <=> $b} @found;

@results;
for(my $i = 0; $i < @found; $i++) {
	push(@results, $found[$i]) if($found[$i] != $found[$i+1]);
}

print join("\n", @results);
