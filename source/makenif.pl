# perl -w

my $version = shift;
my $fname = shift;
open IN, "<$fname" or die "Cant open $fname";
open OUT, ">$fname.tmp" or die "Cant open $fname.tmp";
while (<IN>)
{
	if (/Version\s+=\s+(\S+)/)
	{
		s/$1/$version/;
	}
	print OUT;
}
close IN;
close OUT;
system "mv $fname.tmp $fname";
