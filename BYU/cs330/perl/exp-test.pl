#!/usr/bin/perl

$username = "fred";
$password = "foo";

print "Enter user:pass: ";
$logon = <>;
if($logon =~ /([^:]+):(.+)/) {
	print "User $1 good.\n" if ($1 eq $username);
	print "Pass $2 good.\n" if ($2 eq $password);
}
