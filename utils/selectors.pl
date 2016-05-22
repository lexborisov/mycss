#!/usr/bin/perl -w

use bytes;
use strict;
use Encode;

use MyCSS::Grammar;

my $filename = "data/selectors_grammar.txt";

my $grammar = MyCSS::Grammar->new();
my ($parse_res, $index_res) = $grammar->parser_file($filename);

foreach my $key (@$index_res) {
	my $tree = $grammar->create_tree($parse_res->{$key}, $key);
	my $list = $grammar->parse_tree($tree);
	
	print "$key:\n";
	$grammar->print_list($list, 1);
}

