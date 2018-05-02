#!/usr/bin/perl

print "Hello, world!\n";

my $food = "everything"; # my not required, makes it local instead of global
my $amount = 100;

print "I like $amount portions of ", $food, ".\n";

my @data = ("food", "games", $food, 42);
if(@data == 4) { print "cool, data == $#data\n";
		 print "data contains: @data\n"; }
print "\$data[2] = $data[2]\n";
print "\$data[1..3] = $data[1..3]\n"; #not desired
print "\@data[1..3] = @data[1..3]\n";
print "\@data[2,0] = @data[2,1]\n";

if(@ARGV) {
	print "Args: @ARGV\n"; 
}
# is == to
print "Args: @ARGV\n" if @ARGV;

# also unless (!if) and until (!while)

print "\nFor1:\n";
for(my $i = 0; $i < @data; $i++) {
	print $data[$i], "\n";
}

print "\nFor2:\n";
foreach(@data) {
	print "$_\n"; # print $_; = print;
}

print "\nFor3:\n";
foreach $dat (@data) {
	print "$dat\n";
}

#email breaker
my $email = <>;
if($email =~ /([^@]+)@(.+)/) {
	print "Username: $1\n";
	print "Host: $2\n";
}

# read from stdin, exit on exit, print non-blanks
while (<>) {
	next if /^$/;
	last if /^exit/; #break ! work?
	print;
}

