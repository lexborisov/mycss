#!/usr/bin/perl -w

use bytes;
use strict;
use Encode;

my $grammar = "
<selector-list> = <complex-selector-list>

<complex-selector-list> = <complex-selector>#

<compound-selector-list> = <compound-selector>#

<simple-selector-list> = <simple-selector>#

<relative-selector-list> = <relative-selector>#

<complex-selector> = <compound-selector> [ <combinator>? <compound-selector> ]*

<relative-selector> = <combinator>? <complex-selector>

<combinator> = '>>' | '>' | '+' | '~' | <column-token>

<compound-selector> = <simple-selector>+

<simple-selector> = <type-selector> | <id-selector> |
                    <class-selector> | <attribute-selector> |
                    <pseudo-class-selector> | <pseudo-element-selector>

<type-selector> = <wq-name> | <ns-prefix> '*'

<ns-prefix> = <ident-token>? '|'

<wq-name> = <ns-prefix>? <ident-token>

<id-selector> = <hash-token>

<class-selector> '.' <ident-token>

<attribute-selector> = '[' <wq-name> ']' |
                       '[' <wq-name> <attr-matcher> [ <string-token> | <ident-token> ] <attr-modifier>? ']'

<attr-matcher> = '=' | <include-match-token> | <dash-match-token> | <prefix-match-token> |
                 <suffix-match-token> | <substring-match-token>

<attr-modifier> = i

<pseudo-class-selector> = ':' <ident-token> |
                          ':' <function-token> <any-value> ')'

<pseudo-element-selector> = ':' <pseudo-class-selector>
";

parser_grammar($grammar);

sub parser_grammar {
	my $result = {};
	my ($key, $value);
	
	my @current;
	foreach my $line (split /\n/, $_[0])
	{
		$line =~ s/^\s+//;
		$line =~ s/\s+$//;
		next unless $line;
		
		if ($line =~ /^[^'"]+\s*=/) {
			if(@current) {
				$result->{ $key } = [];
				parser_grammar_value(\@current, 0, $result->{ $key }, [], []);
				
				@current = ();
			}
			
			($key, $value) = split /\s*=\s*/, $line, 2;
			push @current, split /\s+/, $value;
		}
		else {
			push @current, split /\s+/, $line;
		}
	}
	
	if(@current) {
		$result->{ $key } = [];
		parser_grammar_value(\@current, 0, $result->{ $key }, [], []);
	}
	
	foreach my $key (sort {$a cmp $b} keys %$result) {
		my $start = [];
		my $end = [];
		
		print_res($result->{$key}, $start, $end);
		print $key, "\n";
		print_result($start, 1);
	}
	
	print "";
}

sub parser_grammar_value {
	my ($rules, $idx, $entries, $prev, $next) = @_;
	
	my $base_entry = $entries;
	my $base_prev  = $prev;
	my $last_entry = $entries;
	
	while($idx < @$rules)
	{
		my $name = $rules->[$idx];
		my $mod = parser_delete_postmod($name);
		
		if ($name eq "|") {
			foreach my $entry (@$last_entry) {
				$entry->{next} = $next;
			}
			
			$last_entry = $entries;
			$entries = $base_entry;
			$prev    = $base_prev;
		}
		elsif ($name eq "[") {
			my $new_entries = [];
			
			$idx = parser_grammar_value($rules, ++$idx, $entries, $prev, $new_entries);
			
			$name = $rules->[$idx];
			$mod = parser_delete_postmod($name);
			
			if($mod) {
				foreach my $entry (@$entries) {
					$entry->{mod} = $mod;
				}
			}
			
			$base_entry = $entries;
			$entries = $new_entries;
		}
		elsif($name eq "]") {
			foreach my $entry (@$last_entry) {
				$entry->{next} = $next;
			}
			
			return $idx;
		}
		else {
			push @$entries, {name => $name, next => [], prev => $prev, mod => ""};
			
			$last_entry = $entries;
			$prev       = $entries->[-1];
			$entries    = $entries->[-1]->{next};
		}
		
		$idx++;
	}
	
	$idx;
}

sub print_res {
	my ($entry, $start, $end) = @_;
	return unless ref $entry && @$entry;
	
	push @$start, $end if @$end == 0;
	
	if(@$entry > 1) {
		foreach my $idx (1..$#$entry) {
			my @new_end = @$end;
			
			push @$start, \@new_end;
			push @new_end, $entry->[$idx];
			
			print_res($entry->[$idx]->{next}, $start, \@new_end);
		}
	}
	
	push @$end, $entry->[0];
	print_res($entry->[0]->{next}, $start, $end);
}

sub parse_convert_link {
	my ($result) = @_;
	
	foreach my $key (keys %$result) {
		parse_convert_link_end($result->{$key});
	}
}

sub parse_convert_link_end {
	my ($result, $key) = @_;
	return unless exists $result->{$key};
	
	foreach my $key (keys %$result) {
		#$result->{$key};
	}
}

sub print_result {
	my ($result, $tab_count) = @_;
	
	foreach my $res (@$result) {
		print "\t" for 1..$tab_count || 0;
		
		print join " -> ", map {$_->{name}} @$res;
		
		print "\n";
	}
}

sub parser_delete_postmod {
	if($_[0] =~ s/(\>|'|\])([^']+)$/$1/) {
		return $2;
	}
	
	"";
}


