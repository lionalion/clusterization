#!/usr/bin/perl

#script integrates the results of clustering and source text (input_file)
# output_log input_file out
use warnings;
use strict;
use Encode;

sub usage {
	print "\nUse: $0 claster_log  file out_file\n";
	exit 1;
}
usage if ( $#ARGV != 2);
my $log = shift @ARGV;
my $fls = shift @ARGV;
my $f_out = shift @ARGV;

###########################################################

open( F_LOG,  $log ) or die "Can't open file: $!";
binmode(F_LOG, ":encoding(UTF-8)");
open( F_IN,  $fls ) or die "Can't open file: $!";
binmode(F_IN, ":encoding(UTF-8)");
open( F_OUT, '>', $f_out) or die "Can't open file: $!";
binmode(F_OUT, ":encoding(UTF-8)");
my %tmp=();
while (my $lin =<F_IN>) { 
	my @t = split "\t", $lin;
	if ($#t==1){
		my @sym = split(//, $t[1]);
		my $str;
		if ($#sym > 170){
			for(my $x=0; $x<170; $x++) {
				$str.= $sym[$x];
			}
			$str.="...";
		}
		else{
			$t[1]=~s/[\r\n\"]+//;
			$str=$t[1];
		}
		$tmp{$t[0]} = $str;	
	}
	else{
		print "error format...\n$lin\n";
	}
}
print F_OUT "cluster_number\tweigth\tIDoc\ttext(limit is 170 symbols)\n";
while (my $line =<F_LOG>) { 
	if ($line =~ /\S/){
		my @str = split ":", $line;	
		my %h=();
		my @cls = split ";", $str[1];	
		foreach my $cls (@cls){	
			if ($cls =~ /\d/){
				my @one = split " ", $cls;
				if(!exists( $h{$one[0]}) ) {
					$h{$one[0]} = $one[1];
				}
			}
		}
		foreach my $h (reverse sort { $h{$a} <=> $h{$b} } keys %h){
			if(exists( $tmp{$h})){
				my $cnt=$str[0]+1;
				my $wgt=int ($h{$h}*1000);
				print F_OUT "$cnt\t$h\t$tmp{$h}\n";
			}
		}
	}
}

