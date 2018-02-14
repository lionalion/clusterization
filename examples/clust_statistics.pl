#! /usr/bin/perl

##! perl -w
use strict;
use warnings;
use utf8;
##########################################################
sub usage {
  print "Use: $0 file log_file out\n";
  print "P,R,F for clusterization\n";
  exit 1;
};
#format log_file
#id: {num_cluster_num_doc weight};
#10: 506 0.123; 905 0.124; 502 0.125; 501 0.126; 504 0.121; 505 0.123;
usage if ( $#ARGV != 2);
##########################################################
my $f = $ARGV[0];
my $fl = $ARGV[1];
my $out = $ARGV[2];

open( F_OUT, ">$out" ) || die "Can't open file: $!";
binmode(F_OUT, ":encoding(UTF-8)");
open( F_IN,  $f ) or die "Can't open file: $!";
binmode(F_IN, ":encoding(UTF-8)");
my %h=();
while (my $lin =<F_IN>) { 
	my @ar = split  "\t", $lin;
	my @t = split  "", $ar[0];
	if ($#t == 2){
		if (!exists $h{$t[0]}) {
			$h{$t[0]}=1;
		}
		else {
			$h{$t[0]}=$h{$t[0]}+1;#number files in dir
		}
	}
	elsif ($#t == 3){
		if (!exists $h{$t[0].$t[1]}) {
			$h{$t[0].$t[1]}=1;
		}
		else {
			$h{$t[0].$t[1]}=$h{$t[0].$t[1]}+1;#number files in dir
		}
	}	
}
 
#foreach my $k ( sort keys  %h){
#	print  "$k\t$h{$k}\n";
#}
open( F_LOG,  $fl ) or die "Can't open file: $!";
binmode(F_LOG, ":encoding(UTF-8)");

my @clst ;
my %unic=();
while (my $lin =<F_LOG>) { 
	my %t=();
	my $sz=0;
	if ($lin=~/\S/){
		my @t = split ":", $lin;
		my @fls = split ";", $t[1];		
		foreach my $i (@fls){
			$i=~ m/(\d+) [\d\.]+/;
			my @sp = split "", $1;
			if ($#sp == 2){
				if (!exists $t{$sp[0]}) {
					$t{$sp[0]}=1;
				}
				else {
					$t{$sp[0]}=$t{$sp[0]}+1;
				}
				$sz++;#cluster's size 
			}
			elsif($#sp == 3){
				if (!exists $t{$sp[0].$sp[1]}) {
					$t{$sp[0].$sp[1]}=1;
				}
				else {
					$t{$sp[0].$sp[1]}=$t{$sp[0].$sp[1]}+1;
				}
				$sz++;#cluster's size 
			}
		}
	} 
	my $flg=0;
	my %w=();
	foreach my $k ( sort { $a <=> $b } 	keys  %t){#костыль на случай, если колво док в кластерах равны
		if (exists $w{$t{$k}}){#отсортировали по кол-ву док в кластере, если кол-во док одинаково - оставили один вариант 
			$w{$t{$k}}=$k;#зависит от сортировки - тут добавляем последний
		}
		else{$w{$t{$k}}=$k;}
	}
	foreach my $k ( reverse sort keys  %w){#от макс. кол-ва док в одном кластере
		if ($flg==0){#берем только верхний			
			push @clst, "$w{$k}\t$k\t$sz";#number claster - doc_frq - cluster size 
			if (!exists $unic{$w{$k}} ){
				$unic{$w{$k}}=1;#кол-во уникальных док в кластере
			}
			$flg=1;
		}
		else { last }
	} 
	#print F_OUT "\n";	
}
#print F_OUT "\n";
#foreach my $k (keys %unic){
#print  "$k\n";
#}
##### calculation ##########
#print "size of hash:  " . $#clst."\t".keys( %unic )."\t".keys( %h )."\n";
### macroPrecision ### 
my $Pmac=keys( %unic ) /keys( %h ) ;
### macroRecall ### if keys( %h )<($#clst+1)-????
my $Rmac;
if (keys( %h )>=($#clst+1)){
	$Rmac= ($#clst+1)/keys( %h );
}
else{
	$Rmac= keys( %h )/($#clst+1);#если получилось больше, чем д.б.
}
##############################
my $sumP=0;
my $sumR=0;
my $sum=0;
foreach my $a ( @clst){
	my @c=split "\t", $a;
	if (exists $h{$c[0]}){
		$sumP=$sumP+$c[1]/$h{$c[0]};
		$sum++;
		if ($h{$c[0]}>$c[2]){
			$sumR=$sumR+$c[2]/$h{$c[0]};
		}
		else{
			$sumR=$sumR+$h{$c[0]}/$c[2];
		}
	}
	else{print "something strange...\n"}	
}
### microPrecision ###
my $Pmic = $sumP/$sum;
### microRecall ###
my $Rmic = $sumR/$sum;

my $F1=2*$Pmac*$Rmac/($Rmac+$Pmac);
my $F2=2*$Pmic*$Rmic/($Rmic+$Pmic);
my $F=2*$F1*$F2/($F1+$F2);
print F_OUT "Pmac=$Pmac\nRmac=$Rmac\nPmic=$Pmic\nRmic=$Rmic\nF1=$F1\nF2=$F2\nF=$F\n"; 
print "$out\nPmac\t$Pmac\nRmac\t$Rmac\nPmic\t$Pmic\nRmic\t$Rmic\nF1\t$F1\nF2\t$F2\nF\t$F\n"; 