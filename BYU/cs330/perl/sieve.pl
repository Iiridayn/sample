#!/usr/bin/perl

#check if there is exactly one argument
if(@ARGV != 1) {
	print "Usage: $0 <max-number>\n";
	exit;
}

my $size = $ARGV[0]; #new scalar, = to the input max value
my $bound = sqrt($size); #scalar to bound the loop
#print "Using size $size, and bound $bound.\n";

# create an array of scalars, initialized to 2, 3, ..., size
my @primes = (2 .. $size);

#for each array index < the boundary point
for($i = 0; $i < $bound; $i++) {
	next if ($primes[$i] == 0); #pass if set to 0 - means not a prime
	#print "$i ";
	#for all multiples of $i, starting at $i squared, up to the max (index $size-2)
	for($j = ($primes[$i]*$primes[$i]); $j <= $size; $j+=$primes[$i]) {
		$primes[$j-2] = 0; # set the non-prime to 0
		#print "setting $j to 0: $primes[$j-2]\n";
	}
}

#print "primes: @primes\n";

#for each element in @primes, store the value in $prime temporarly
foreach $prime (@primes) {
	print "$prime  " unless ($prime == 0); #only print the value if it is non-zero
}
print "\n"; # for the shell
