#!/usr/bin/perl -w

my @files=<ini/*.ini>;
foreach $file (@files)
{
  print "Now $file\n";
  system("./main $file");
}
exit 0;
