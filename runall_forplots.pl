#!/usr/bin/perl -w

my $exec = "main_1";
system("cp ./main $exec");

my $file = "ini/Top_Run06_Co_1.17_newMC.ini";
print "Now $file\n";
system("./$exec $file");

$file = "ini/Top_Run06_Co_1.33_newMC.ini";
print "Now $file\n";
system("./$exec $file");

$file = "ini/Top_Run06_Th_0.58_newMC.ini";
print "Now $file\n";
system("./$exec $file");

$file = "ini/Top_Run06_Th_2.61_newMC.ini";
print "Now $file\n";
system("./$exec $file");

$file = "ini/Side_Run07_Th_0.58_newMC.ini";
print "Now $file\n";
system("./$exec $file");

$file = "ini/Side_Run07_Th_2.61_newMC.ini";
print "Now $file\n";
system("./$exec $file");
