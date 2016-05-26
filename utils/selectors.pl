#!/usr/bin/perl -w

use bytes;
use strict;
use Encode;

use MyCSS::Grammar;
use MyCSS::Token;
use MyCSS::CFunction;

# !WARNING! Work! Draft version!

my $filename = "data/selectors_grammar_work.txt";

my $token = MyCSS::Token->new();
my $grammar = MyCSS::Grammar->new();
my $cfunc = MyCSS::CFunction->new();

my ($parse_res, $index_res) = $grammar->parser_file($filename);
#my ($parse_res, $index_res) = $grammar->parser("<simple-selector> = <'>' wc func=lalal> <'>'> | <'>'> | <'+'> | <'~'> | <column-token>");

my $index_list = {};
foreach my $key (@$index_res) {
	my $tree = $grammar->create_tree($parse_res->{$key}, $key);
	my $list = $grammar->parse_tree($tree);
	
	$index_list->{$key} = $list;
	
	#print "$key:\n";
	#$grammar->print_list($list, 1);
}

#my $key = "<attribute-selector>";
my $key = "<simple-selector>";
#
my $workd = $grammar->decomposite($index_list, [$key]);
##$grammar->print_list($index_list->{$key}, 1);
$grammar->print_list($workd->{$key}, 1);

print "\n\n";

#my $key = "<compound-selector>";

my $work = $grammar->decomposite($index_list, [$key], sub {
	return MyCSS::Token->new($_[1]);
});

print "Work data for $key:\n";
my $hash = $grammar->make_combine_hash_from_decomposing_list($work->{$key}, sub{ $_[1]->entry->name });
print "Combine hash count: ", scalar(keys %$hash), "\n";

my $func_result = $cfunc->create($key, $hash, "mycss_selectors_state_");
#$cfunc->print_result_names($func_result);
$cfunc->print_result_data($func_result);


