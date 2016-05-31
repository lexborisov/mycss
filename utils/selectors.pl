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
my $cfunc = MyCSS::CFunction->new(
	token               => $token,
	grammar             => $grammar,
	func_def            => \&My::Functions::Basic::function_default,
	func_else           => \&My::Functions::Basic::function_else,
	func_last           => \&My::Functions::Basic::function_last,
	func_prefix         => "mycss_selectors_state_",
	func_whitespace     => \&My::Functions::Basic::function_whitespace,
	func_not_whitespace => \&My::Functions::Basic::function_not_whitespace,
	func_string_before  => \&My::Functions::Basic::function_string_before,
	func_string_after   => \&My::Functions::Basic::function_string_after,
	func_proto_args     => "mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token",
	func_list           => {
		selector_combinator   => \&My::Functions::selector_combinator,
		selector_ident        => \&My::Functions::selector_ident,
		selector_namespace    => \&My::Functions::selector_namespace,
		selector_key          => \&My::Functions::selector_key,
		selector_id           => \&My::Functions::selector_id,
		selector_class        => \&My::Functions::selector_class,
		selector_matcher      => \&My::Functions::selector_matcher,
		selector_value        => \&My::Functions::selector_value,
		selector_modifier     => \&My::Functions::selector_modifier,
		selector_end          => \&My::Functions::selector_end,
		selector_pseudo_class => \&My::Functions::selector_pseudo_class,
		selector_function     => \&My::Functions::selector_function,
		selector_function_end => \&My::Functions::selector_function_end
	}
);

my ($parse_res, $index_res) = $grammar->parser_file($filename);
#my ($parse_res, $index_res) = $grammar->parser('<combinator> = <ident-token func=selector_ident>? <delim-token value="|" func=selector_namespace>');

my $index_list = {};
foreach my $key (@$index_res) {
	my $tree = $grammar->create_tree($parse_res->{$key}, $key);
	my $list = $grammar->parse_tree($tree);
	
	$index_list->{$key} = $list;
}

#my $key = "<combinator>";
my $key = "<simple-selector>";

my $work = $grammar->decomposite($index_list, [$key], sub {
	return MyCSS::Token->new($_[1], $_[2]);
});

$grammar->print_list($work->{$key});

print "Work data for $key:\n";
my $hash = $grammar->make_combine_hash_from_decomposing_list($work->{$key}, sub{ $_[1]->entry->name });

my $func_result = $cfunc->create($key, $hash, "mycss_selectors_state_");
$cfunc->print_result_names($func_result);
$cfunc->print_result_data($func_result);



package My::Functions;

sub selector_combinator {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_combinator(result, selectors, selector, token);"]
}

sub selector_ident {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_ident(result, selectors, selector, token);"]
}

sub selector_namespace {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_namespace(result, selectors, selector, token);"]
}

sub selector_key {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_key(result, selectors, selector, token);"]
}

sub selector_id {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_id(result, selectors, selector, token);"]
}

sub selector_class {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_class(result, selectors, selector, token);"]
}

sub selector_matcher {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_matcher(result, selectors, selector, token);"]
}

sub selector_value {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_value(result, selectors, selector, token);"]
}

sub selector_modifier {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_modifier(result, selectors, selector, token);"]
}

sub selector_end {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_end(result, selectors, selector, token);"]
}

sub selector_pseudo_class {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_pseudo_class(result, selectors, selector, token);"]
}

sub selector_function {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_function(result, selectors, selector, token);"]
}

sub selector_function_end {
	my ($creater, $cfunc, $fname, $type) = @_;
	["mycss_selectors_parser_selector_function_end(result, selectors, selector, token);"]
}


package My::Functions::Basic;

sub function_whitespace {
	my ($creater, $cfunc, $exists_delim) = @_;
	
	[
		"if(result->parser != mycss_selectors_state_token_skip_whitespace)",
		"\t"."result->parser = mycss_selectors_state_token_skip_whitespace;"
	];
}

sub function_not_whitespace {
	my ($creater, $cfunc, $exists_delim) = @_;
	
	[
		"if(result->parser != mycss_selectors_state_token_all)",
		"\t"."result->parser = mycss_selectors_state_token_all;"
	];
}

sub function_string_before {
	my ($creater, $cfunc, $exists_delim) = @_;
	
	[
		"myhtml_string_t str;",
		"mycss_token_data_to_string(result->entry, token, &str);"
	];
}

sub function_string_after {
	my ($creater, $cfunc, $exists_delim) = @_;
	
	["myhtml_string_destroy(&str, false);"];
}

sub function_default {
	my ($creater, $cfunc, $fname, $type) = @_;
	
	["selectors->state = $fname;"];
}

sub function_else {
	my ($creater, $cfunc, $fname) = @_;
	
	# exception for begin selectors <ident-token>
	# if next token is whitespace or something else do not call expectations error
	# it is normally situation
	if($fname eq "mycss_selectors_state_simple_selector_ident") {
		return [
			"result->parser = mycss_parser_token;",
			"return false;"
		];
	}
	
	[
		"mycss_selectors_parser_expectations_error(result, selectors, selector, token);",
		"result->parser = mycss_parser_token;",
		"return false;"
	];
}

sub function_last {
	my ($creater, $cfunc, $fname, $type, $find_next) = @_;
	
	if($find_next) {
		return [
			"printf(\"$fname\\n\");  /* End of selector */"
		];
	}
	
	[
		"printf(\"$fname\\n\");  /* End of selector */",
		"result->parser = mycss_parser_token;"
	];
}




