print "Hello, World!\n";

my $ten = 10;
my @foo = (1 .. $ten);
print "Foo: @foo\n";

my @bar = (1 .. 10);
foreach $tmp (@bar) {
	print "$tmp ";
}
print "\n";
