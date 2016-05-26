package MyCSS::CFunction;

use utf8;
use strict;
use Encode;

use MyCSS::Token;

my $MyCSS_CFUNCTION_CHAR_NAME = {
	'0' => "Null",
	'1' => "Start of Heading",
	'2' => "Start of Text",
	'3' => "End of Text",
	'4' => "End of Transmission",
	'5' => "Enquiry",
	'6' => "Acknowledgment",
	'7' => "Bell",
	'8' => "Back Space",
	'9' => "Horizontal Tab",
	'10' => "Line Feed",
	'11' => "Vertical Tab",
	'12' => "Form Feed",
	'13' => "Carriage Return",
	'14' => "Shift Out",
	'15' => "Shift In",
	'16' => "Data Line Escape",
	'17' => "Device Control 1",
	'18' => "Device Control 2",
	'19' => "Device Control 3",
	'20' => "Device Control 4",
	'21' => "Negative Acknowledgement",
	'22' => "Synchronous Idle",
	'23' => "End of Transmit Block",
	'24' => "Cancel",
	'25' => "End of Medium",
	'26' => "Substitute",
	'27' => "Escape",
	'28' => "File Separator",
	'29' => "Group Separator",
	'30' => "Record Separator",
	'31' => "Unit Separator",
	'32' => "Space",
	'33' => "Exclamation mark",
	'34' => "Double quotes",
	'35' => "Number",
	'36' => "Dollar",
	'37' => "Procent",
	'38' => "Ampersand",
	'39' => "Single quote",
	'40' => "Left parenthesis",
	'41' => "Right parenthesis",
	'42' => "Asterisk",
	'43' => "Plus",
	'44' => "Comma",
	'45' => "Hyphen",
	'46' => "Full stop",
	'47' => "Slash or divide",
	'48' => "Zero",
	'49' => "One",
	'50' => "Two",
	'51' => "Three",
	'52' => "Four",
	'53' => "Five",
	'54' => "Six",
	'55' => "Seven",
	'56' => "Eight",
	'57' => "Nine",
	'58' => "Colon",
	'59' => "Semicolon",
	'60' => "Less than",
	'61' => "Equals",
	'62' => "Greater than",
	'63' => "Question mark",
	'64' => "At symbol",
	'65' => "A",
	'66' => "B",
	'67' => "C",
	'68' => "D",
	'69' => "E",
	'70' => "F",
	'71' => "G",
	'72' => "H",
	'73' => "I",
	'74' => "J",
	'75' => "K",
	'76' => "L",
	'77' => "M",
	'78' => "N",
	'79' => "O",
	'80' => "P",
	'81' => "Q",
	'82' => "R",
	'83' => "S",
	'84' => "T",
	'85' => "U",
	'86' => "V",
	'87' => "W",
	'88' => "X",
	'89' => "Y",
	'90' => "Z",
	'91' => "Left bracket",
	'92' => "Backslash",
	'93' => "Right bracket",
	'94' => "CircumFlex",
	'95' => "Underscore",
	'96' => "Grave accent",
	'97' => "a",
	'98' => "b",
	'99' => "c",
	'100' => "d",
	'101' => "e",
	'102' => "f",
	'103' => "g",
	'104' => "h",
	'105' => "i",
	'106' => "j",
	'107' => "k",
	'108' => "l",
	'109' => "m",
	'110' => "n",
	'111' => "o",
	'112' => "p",
	'113' => "q",
	'114' => "r",
	'115' => "s",
	'116' => "t",
	'117' => "u",
	'118' => "v",
	'119' => "w",
	'120' => "x",
	'121' => "y",
	'122' => "z",
	'123' => "Opening brace",
	'124' => "Vertical bar",
	'125' => "Closing brace",
	'126' => "Tilde",
	'127' => "Delete"
};

# temp
sub temp_delim_data_state {
	"data";
}

sub temp_token_type_state {
	"token->type";
}

sub temp_get_data_state {
	("myhtml_incoming_buffer_t *buffer = mycss_token_buffer_first(selectors->entry, token);",
    "const char data = buffer->data[ (token->begin - buffer->offset) ];");
}

sub temp_set_state {
	"selectors->state = $_[0];";
}

sub temp_function_state {
	"bool $_[0](mycss_selectors_t* selectors, mycss_token_t* token)";
}

sub temp_function_return_state {
	"return true;";
}

sub temp_check_parser_state {
	("if(selectors->parser != $_[0])",
	"\tselectors->parser = $_[0];");
}

sub new {
	bless {grammar => $_[1], token => $_[2]}, $_[0];
}

sub create {
	my ($cfunc, $func_name, $hash, $prefix) = @_;
	
	$prefix = $prefix . $cfunc->create_func_name_by_name($func_name, "");
	
	my $result = {names => [], data => []};
	$cfunc->_create($prefix, $hash, "$prefix\_", $result);
	
	$result;
}

sub _create {
	my ($cfunc, $func_name, $hash, $prefix, $result) = @_;
	
	my @res;
	my @inside;
	my @other;
	my @delim;
	
	foreach my $key (sort {$a cmp $b} keys %$hash)
	{
		my $type = $cfunc->token_type_by_name($key);
		
		if ($type == MyCSS_TOKEN_TYPE_DELIM) {
			push @delim, $key;
		}
		else {
			push @other, $key;
		}
	}
	
	my $res_delim = $cfunc->_create_condition_delim($func_name, \@delim, $hash, $prefix, \@res);
	my $res_oyher = $cfunc->_create_condition_other($func_name, \@other, $hash, $prefix, \@res);
	
	push @inside, temp_function_state($func_name), "{";
	push @inside, "\t". "switch(token->type)", "\t{";
	
	if(@$res_delim) {
		push @inside, @$res_delim;
	}
	
	push @inside, @$res_oyher;
	$cfunc->_create_condition_default(\@inside, 2);
	
	push @inside, "\t}";
	push @inside, "", "\t". temp_function_return_state();
	push @inside, "}\n";
	
	push @{$result->{data}}, \@inside;
	push @{$result->{names}}, \@res;
	
	foreach my $entry (@res) {
		$cfunc->_create($entry->[0], $entry->[1], $entry->[0]."_", $result);
	}
}

sub _create_condition_else {
	my ($cfunc, $inside, $tab_count) = @_;
	
	my $kw = join("\t", map {""} 1..$tab_count) || "";
	
	push @$inside, "\t$kw"."else {";
	push @$inside, "\t\t$kw". temp_set_state("mycss_selectors_state_simple_selector");
	push @$inside, "", "\t\t$kw". join("\n\t\t$kw", temp_check_parser_state("mycss_parser_token_skip_whitespace"));
	push @$inside, "", "\t\t$kw"."return false;";
	push @$inside, "\t$kw}";
}

sub _create_condition_default {
	my ($cfunc, $inside, $tab_count) = @_;
	
	my $kw = join("\t", map {""} 1..$tab_count) || "";
	
	push @$inside, "\t$kw"."default: {";
	push @$inside, "\t\t$kw". temp_set_state("mycss_selectors_state_simple_selector");
	push @$inside, "", "\t\t$kw". join("\n\t\t$kw", temp_check_parser_state("mycss_parser_token_skip_whitespace"));
	push @$inside, "", "\t\t$kw"."return false;";
	push @$inside, "\t$kw}";
}

sub _create_condition_delim {
	my ($cfunc, $func_name, $keys, $hash, $prefix, $res) = @_;
	return [] unless @$keys;
	
	my @inside = ("\t\t". $cfunc->create_condition_type( $keys->[0], temp_token_type_state() ). " {");
	
	push @inside, "\t\t\t". join("\n\t\t\t", temp_get_data_state()). "\n";
	
	my $inside_len = @inside;
	
	foreach my $key (@$keys) {
		my $fn = $cfunc->create_func_name_by_name($key, $prefix);
		
		push @inside, "\t\t\t". (@inside > $inside_len ? "else ": "").
		$cfunc->create_condition( $key, temp_delim_data_state() ) ." {";
		
		if(%{$hash->{$key}->{next}})
		{
			push @$res, [$fn, $hash->{$key}->{next}, $key];
			push @inside, "\t\t\t\t". temp_set_state($fn);
			push @inside, "", "\t\t\t\t". _create_check_whitespace($key, $hash, 3);
		}
		else {
			push @inside,
			"\t\t\t\t". "printf(\"$fn\\n\");  /* End of selector */",
			"\t\t\t\t". temp_set_state("mycss_selectors_state_simple_selector"), "",
			"\t\t\t\t". join("\n\t\t\t\t", temp_check_parser_state("mycss_parser_token_skip_whitespace"));
		}
		
		push @inside, "\t\t\t}";
	}
	
	$cfunc->_create_condition_else(\@inside, 3);
	
	push @inside, "", "\t\t\t". "break;";
	push @inside, "\t\t}";
	
	\@inside;
}

sub _create_condition_other {
	my ($cfunc, $func_name, $keys, $hash, $prefix, $res) = @_;
	return [] unless @$keys;
	
	my @inside;
	
	foreach my $key (@$keys) {
		my $fn = $cfunc->create_func_name_by_name($key, $prefix);
		
		push @inside, "\t\t". $cfunc->create_condition_type( $key, temp_token_type_state() ) ." {";
		
		if(%{$hash->{$key}->{next}})
		{
			push @$res, [$fn, $hash->{$key}->{next}, $key];
			push @inside, "\t\t\t". temp_set_state($fn);
			push @inside, "", "\t\t\t". _create_check_whitespace($key, $hash, 2);
		}
		else {
			push @inside, "\t\t\t". "printf(\"$fn\\n\");  /* End of selector */",
			"\t\t\t". temp_set_state("mycss_selectors_state_simple_selector"), "",
			"\t\t\t". join("\n\t\t", temp_check_parser_state("mycss_parser_token_skip_whitespace"));
		}
		
		push @inside, "", "\t\t\t". "break;";
		push @inside, "\t\t}";
	}
	
	\@inside;
}

sub _create_check_whitespace {
	my ($key, $hash, $tab_count) = @_;
	my $val = $hash->{$key}->{val};
	
	my $kw = join "\t", map {""} 0..$tab_count;
	
	if (exists $val->[0]->{entry}->{attr}->{ws}) {
		return join("\n$kw", temp_check_parser_state("mycss_parser_token_skip_whitespace"));
	}
	
	join("\n$kw", temp_check_parser_state("mycss_parser_token_all"));
}

sub create_condition {
	my ($self, $name, $data) = @_;
	
	my $type = $self->token_type_by_name($name);
	
	my $text = "";
	
	if($type == MyCSS_TOKEN_TYPE_DELIM()) {
		my $clean_name = MyCSS::Token->entry_clean_name($name);
		$text = qq~if($data == '$clean_name')~;
	}
	else {
		$text = "if($data == ". $self->token_type_name_by_name($name) .")";
	}
	
	$text;
}

#sub create_condition_type {
#	my ($self, $name, $data) = @_;
#	
#	"if($data == ". $self->token_type_name_by_name($name) .")";
#}

sub create_condition_type {
	my ($self, $name) = @_;
	
	"case ". $self->token_type_name_by_name($name) .":";
}

sub print_result_names {
	my ($cfunc, $result) = @_;
	
	foreach my $entries (@{$result->{names}}) {
		foreach my $entry (@$entries) {
			print temp_function_state($entry->[0]), ";\n";
		}
	}
}

sub print_result_data {
	my ($cfunc, $result) = @_;
	
	foreach my $entries (@{$result->{data}}) {
		print join("\n", @$entries), "\n";
	}
}

sub token { $_[0]->{token} }
sub grammar { $_[0]->{token} }

sub token_type_by_name {
	my $type_name = MyCSS::Token->convert_name_to_type_like($_[1]) || MyCSS::Token->get_type_undef();
	MyCSS::Token->type_to_num($type_name);
}

sub token_type_name_by_name {
	my $type_name = MyCSS::Token->convert_name_to_type_like($_[1]) || MyCSS::Token->get_type_undef();
	$type_name;
}

sub create_func_name_by_name {
	my ($cfunc, $name, $prefix) = @_;
	
	my $func_name;
	my $type = $cfunc->token_type_by_name($name);
	
	my $c_name = MyCSS::Token->entry_clean_name($name);
	
	if($type == MyCSS_TOKEN_TYPE_DELIM) {
		$func_name = lc($MyCSS_CFUNCTION_CHAR_NAME->{ ord(lc($c_name)) });
	}
	else {
		$func_name = lc($c_name);
		
		$func_name =~ s/\-token$//;
		
		if (length($func_name) == 1 && exists $MyCSS_CFUNCTION_CHAR_NAME->{ ord(lc($func_name)) }) {
			$func_name = lc($MyCSS_CFUNCTION_CHAR_NAME->{ ord(lc($func_name)) });
		}
	}
	
	$func_name =~ s/\s+|\||-/_/g;
	$func_name =~ s/_+/_/g;
	
	$prefix.$func_name;
}

1;
