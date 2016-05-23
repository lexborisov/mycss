#!/usr/bin/perl -w

use bytes;
use strict;
use Encode;

use MyCSS::Grammar;

my $filename = "data/selectors_grammar.txt";

my $grammar = MyCSS::Grammar->new();
my ($parse_res, $index_res) = $grammar->parser_file($filename);

my $index_list = {};

foreach my $key (@$index_res) {
	my $tree = $grammar->create_tree($parse_res->{$key}, $key);
	my $list = $grammar->parse_tree($tree);
	
	$index_list->{$key} = $list;
	#
	#print "$key:\n";
	#$grammar->print_list($list, 1);
}

my $index = {};

foreach my $key (@$index_res) {
	my $work = $grammar->decomposite_list($index_list->{$key}, $index_list, $index);
	
	print "Work data for $key:\n";
	$grammar->print_list($work, 1);	
}



