package MyCSS::Grammar;

use utf8;
use strict;
use Encode;

my $GRAMMAR_SUB_INDEX = {
	'['  => \&GRAMMAR_LEFT_SQUARE_BRACKET,
	']'  => \&GRAMMAR_RIGHT_SQUARE_BRACKET,
	'<'  => \&GRAMMAR_BEGIN_LESS_THAN_SIGN,
	'|'  => \&GRAMMAR_VERTICAL_LINE,
};

my $GRAMMAR_MOD_INDEX = {
	'*'  => \&GRAMMAR_PARSE_MOD_ASTERISK,
	'?'  => \&GRAMMAR_PARSE_MOD_QUESTION_MARK,
	'|'  => \&GRAMMAR_PARSE_MOD_VERTICAL_LINE, # fake
	'['  => \&GRAMMAR_PARSE_MOD_SQUARE_BRACKET,
	'#'  => \&GRAMMAR_PARSE_MOD_DEFAULT,
	'+'  => \&GRAMMAR_PARSE_MOD_DEFAULT,
	''   => \&GRAMMAR_PARSE_MOD_DEFAULT
};

my $GRAMMAR_SYS_MAP = {
	'['  => 1,
	'|'  => 1
};

sub new {
	bless {}, $_[0];
}

sub parser {
	my $grammar = shift;
	
	my ($result, $index) = ({}, []);
	my ($key, $value);
	
	my @current;
	foreach my $line (split /\n/, $_[0])
	{
		$line =~ s/^\s+//;
		$line =~ s/\s+$//;
		next unless $line;
		
		if ($line =~ /^[^'"]+\s*=/) {
			($key, $value) = split /\s*=\s*/, $line, 2;
			
			push @$index, $key;
			
			$result->{ $key } = [] unless $result->{ $key };
			$grammar->parse_line($result->{ $key }, $value);
		}
		else {
			$grammar->parse_line($result->{ $key }, $line);
		}
	}
	
	($result, $index);
}

sub parse_line {
	my $grammar = shift;
	my $current = shift;
	
	my @line = split //, $_[0];
	my $tmp  = 0;
	my $i    = 0;
	
	for ($i = 0; $i < @line; ++$i) {
		if ($line[$i] eq '<') {
			my $res;
			($res, $i) = $grammar->parse_line_entry(\@line, $i);
			push @$current, join "", @$res;
			
			$tmp = $i;
		}
		elsif($line[$i] =~ /\s/) {
			push @$current, join "", @line[($tmp + 1)..($i - 1)] if ($tmp + 1) < $i;
			$tmp = $i;
		}
	}
	
	push @$current, join "", @line[($tmp + 1)..($i - 1)] if ($tmp + 1) < $i;
}

sub parse_line_entry {
	my ($grammar, $line, $i) = @_;
	
	my $is_the_end = 0;
	my $is = 0;
	my @res;
	
	for ($i = $i; $i < @$line; ++$i) {
		if ($line->[$i] eq '\\') {
			$is++;
			
			if (($is % 2) == 0) {
				push @res, $line->[$i];
			}
		}
		elsif($line->[$i] eq '\'') {
			my $cur_res;
			($cur_res, $i) = $grammar->parse_line_text($line, ($i + 1), "'");
			push @res, join "", "'", @$cur_res;
		}
		elsif($line->[$i] eq '"') {
			my $cur_res;
			($cur_res, $i) = $grammar->parse_line_text($line, ($i + 1), '"');
			push @res, join "", '"', @$cur_res;
		}
		elsif ($line->[$i] eq '>') {
			push @res, $line->[$i];
			
			if (($is % 2) == 0)
			{
				while(1) {
					++$i;
					
					if($i < @$line) {
						if($line->[$i] =~ /\s/) {
							--$i;
							last;
						}
						
						push @res, $line->[$i];
					}
					else {
						last;
					}
				}
				
				last;
			}
		}
		else {
			$is = 0 if $is;
			
			push @res, $line->[$i];
		}
	}
	
	(\@res, $i);
}

sub parse_line_text {
	my ($grammar, $line, $i, $qo) = @_;
	
	my $is_the_end = 0;
	my $is = 0;
	my @res;
	
	for ($i = $i; $i < @$line; $i++) {
		if ($line->[$i] eq '\\') {
			$is++;
			
			if (($is % 2) == 0) {
				push @res, $line->[$i];
			}
		}
		elsif ($line->[$i] eq $qo) {
			push @res, $line->[$i];
			
			if (($is % 2) == 0)
			{
				last;
			}
		}
		else {
			$is = 0 if $is;
			
			push @res, $line->[$i];
		}
	}
	
	(\@res, $i);
}

sub parse_line_other {
	my ($grammar, $line, $i) = @_;
	
	for ($i = $i; $i < @$line; ++$i) {
		if ($line->[$i] eq '>' && ($i < @$line || $line->[$i] =~ /\s/)) {
			return $i;
		}
	}
}


sub parser_file {
	my ($grammar, $filename) = @_;
	
	open my $fh, "<", $filename or die "Can't open file: $!";
	my @data = <$fh>;
	close $fh;
	
	$grammar->parser(join("", @data));
}

sub GRAMMAR_LEFT_SQUARE_BRACKET { # ([)
	my ($name, $rules, $idx, $tree, $open, $mod, $attr) = @_;
	
	my $entry = $tree->new({name => $name, mod => "[", attr => $attr});
	$open->[-1]->append_child($entry);
	
	push @$open, $entry;
}

sub GRAMMAR_RIGHT_SQUARE_BRACKET { # (])
	my ($name, $rules, $idx, $tree, $open, $mod, $attr) = @_;
	
	if(@$open <= 1) {
		warn "RIGHT SQUARE BRACKET: try delete ROOT of tree! WRONG!\n";
		exit(1);
	}
	
	my $entry = grammar_open_delete_vertical_line($open, 1);
	
	$entry->mod($mod) if $mod;
}

sub GRAMMAR_BEGIN_LESS_THAN_SIGN { # (<)
	my ($name, $rules, $idx, $tree, $open, $mod, $attr) = @_;
	
	my $entry = $tree->new({name => $name, mod => $mod, attr => $attr});
	$open->[-1]->append_child($entry);
}

sub GRAMMAR_VERTICAL_LINE { # (|)
	my ($name, $rules, $idx, $tree, $open, $mod, $attr) = @_;
	
	my $idx_find = grammar_open_find_in_scope_entry_by_name($open, "|");
	
	if ($idx_find) {
		grammar_open_delete_remove_from_end($open, $open->[$idx_find], 1);
	}
	
	my $entry = $tree->new({name => $name, mod => "|", attr => $attr});
	$open->[-1]->append_child($entry);
	
	push @$open, $entry;
}

sub grammar_open_delete_vertical_line {
	my ($open, $include) = @_;
	
	my $i = @$open;
	while ($i) {
		--$i;
		
		if($open->[$i]->name eq "[") {
			if($include) {
				return delete $open->[$i];
			}
			
			last;
		}
		
		delete $open->[$i];
	}
}

sub grammar_open_delete_remove_from_end {
	my ($open, $entry, $include) = @_;
	
	my $i = @$open;
	while ($i) {
		--$i;
		
		if($open->[$i] eq $entry) {
			delete $open->[$i] if $include;
			last;
		}
		
		delete $open->[$i];
	}
}

sub grammar_open_find_in_scope_entry_by_name {
	my ($open, $name) = @_;
	
	my $i = @$open;
	while ($i) {
		--$i;
		
		if($open->[$i]->{name} eq "[") {
			last;
		}
		elsif ($open->[$i]->{name} eq $name) {
			return $i;
		}
	}
	
	undef;
}

sub create_tree {
	my ($grammar, $rules, $name) = @_;
	
	my $root = MyCSS::Grammar::MyTree->new({name => $name});
	
	my $open = [$root];
	my $idx = 0;
	
	while($idx < @$rules)
	{
		my $name = $rules->[$idx];
		my $mod = parser_delete_postmod($name);
		my $attr = parser_delete_attr($name);
		
		if($name =~ /^(.)/)
		{
			unless(exists $GRAMMAR_SUB_INDEX->{$1}) {
				warn "Not find in index: $name";
				$idx++;
				next;
			}
			
			$GRAMMAR_SUB_INDEX->{$1}->($name, $rules, $idx, $root, $open, $mod, $attr);
		}
		
		$idx++;
	}
	
	$root;
}

sub grammar_delete_empty {
	my ($list) = @_;
	
	my $i = @$list;
	while ($i) {
		--$i;
		delete $list->[$i] unless @{$list->[$i]};
	}
}

sub parse_append {
	my ($grammar, $entry) = @_;
	
	my $end = [[]];
	if ($entry->name eq "[" || $entry->name eq "|")
	{
		my $new_start = [];
		
		if($entry->child) {
			_grammar_parse_tree($grammar, $entry->child, $new_start, $end);
		}
		
		grammar_delete_empty($end);
	}
	else
	{
		$end->[-1] = [$entry];
	}
	
	$end;
}

sub GRAMMAR_PARSE_MOD_ASTERISK { # (*)
	my ($entry, $start, $ends, $curent_ends) = @_;
	
	my $end_len = $#$ends;
	
	foreach my $cend (@$curent_ends) {
		foreach my $i (0..$end_len) {
			my $end = $ends->[$i];
			
			my @new_end = @$end;
			push @$start, \@new_end;
			
			push @new_end, @$cend;
			push @$ends, \@new_end;
		}
	}
}

sub GRAMMAR_PARSE_MOD_QUESTION_MARK { # (?)
	my ($entry, $start, $ends, $curent_ends) = @_;
	
	my $end_len = $#$ends;
	
	foreach my $cend (@$curent_ends) {
		foreach my $i (0..$end_len) {
			my $end = $ends->[$i];
			
			my @new_end = @$end;
			push @$start, \@new_end;
			
			push @new_end, @$cend;
			push @$ends, \@new_end;
		}
	}
}

sub GRAMMAR_PARSE_MOD_VERTICAL_LINE { # (|)
	my ($entry, $start, $ends, $curent_ends) = @_;
	
	foreach my $cend (@$curent_ends) {
		push @$ends, \@$cend;
		push @$start, $ends->[-1];
	}
}

sub GRAMMAR_PARSE_MOD_SQUARE_BRACKET { # ([)
	my ($entry, $start, $ends, $curent_ends) = @_;
	
	my $end_len = $#$ends;
	
	foreach my $i (0..$end_len) {
		my $end = $ends->[$i];
		
		my @tmp = @{$curent_ends->[0]};
		
		foreach my $t (1..$#$curent_ends) {
			my $cend = $curent_ends->[$t];
			
			my @new_end = (@$end, @$cend);
			push @$start, \@new_end;
			
			push @$ends, \@new_end;
		}
		
		push @$end, @tmp;
	}
}

sub GRAMMAR_PARSE_MOD_DEFAULT {
	my ($entry, $start, $ends, $curent_ends) = @_;
	
	my $end_len = $#$ends;
	
	foreach my $cend (@$curent_ends) {
		foreach my $i (0..$end_len) {
			push @{$ends->[$i]}, @$cend;
		}
	}
}

# <compound-selector> [ <t1> | <t2> ] <lalalal>
sub _grammar_parse_tree {
	my ($grammar, $entry, $start, $ends) = @_;
	
	push @$start, $ends->[-1] unless exists $ends->[-1] && @{$ends->[-1]};
	
	while ($entry) {
		my $curent_ends = parse_append($grammar, $entry);
		
		$GRAMMAR_MOD_INDEX->{ ($entry->mod || "") }->($entry, $start, $ends, $curent_ends);
		
		$entry = $entry->next;
	}
}

sub parse_tree {
	my ($grammar, $entry) = @_;
	
	my $ends  = [[]];
	my $start = [];
	
	_grammar_parse_tree($grammar, $entry->child, $start, $ends);
	grammar_delete_empty($start);
	
	$start;
}

sub decomposite {
	my ($grammar, $index_of_list, $keys, $sub_for_bless_entry) = @_;
	
	my $save_index = {};
	my $work = {};
	
	$grammar->{sub_for_bless_entry} = $sub_for_bless_entry;
	
	foreach my $key (@$keys) {
		$work->{$key} = $grammar->decomposite_list($index_of_list->{$key}, $index_of_list, $save_index);
	}
	
	$work;
}

sub decomposite_list {
	my ($grammar, $list, $index_of_list, $save_index) = @_;
	
	my $work = [];
	foreach my $entries (@$list) {
		push @{$work}, @{$grammar->decomposite_list_entries($entries, $index_of_list, $save_index)};
	}
	
	$work;
}

sub decomposite_list_entries {
	my ($grammar, $entries, $index_of_list, $save_index) = @_;
	my $work = [[]];
	
	foreach my $entry (@$entries)
	{
		my $res = $grammar->decomposite_list_entry($entry, $index_of_list, $save_index);
		my $new_work;
		
		foreach my $res_entries (@$res) {
			foreach my $work_entry (@$work) {
				push @{$new_work}, [@$work_entry, @$res_entries];
			}
		}
		
		$work = $new_work;
	}
	
	$work;
}

sub decomposite_list_entry {
	my ($grammar, $entry, $index_of_list, $save_index) = @_;
	
	my $list = [];
	if ($index_of_list->{ $entry->name }) {
		if ($save_index->{$entry->name}) {
			$list = $save_index->{$entry->name};
		}
		else {
			$list = $grammar->decomposite_list($index_of_list->{ $entry->name }, $index_of_list, $save_index);
			$save_index->{$entry->name} = $list;
		}
	}
	else {
		unless(ref $grammar->{sub_for_bless_entry}) {
			push @{$list}, [$entry];
		}
		else {
			push @{$list}, [
				$grammar->{sub_for_bless_entry}->($grammar, $entry)
			];
		}
	}
	
	$list;
}

sub make_combine_hash_from_decomposing_list {
	my ($grammar, $list, $sub) = @_;
	
	$sub ||= sub{ $_[1]->name };
	
	my $hash = {};
	my $tmp_hash = $hash;
	
	foreach my $entries (@$list)
	{
		foreach my $entry (@$entries)
		{
			my $key = $sub->($grammar, $entry);
			$tmp_hash->{$key} = {} unless exists $tmp_hash->{$key};
			
			my $nr = $tmp_hash->{$key};
			
			push @{$nr->{val}}, $entry;
			
			$nr->{next} = {} unless exists $nr->{next};
			
			$tmp_hash = $nr->{next};
		}
		
		undef $tmp_hash;
		$tmp_hash = $hash;
	}
	
	$hash;
}

sub print_list {
	my ($grammar, $result, $tab_count) = @_;
	
	foreach my $res (@$result) {
		print "\t" for 1..$tab_count || 0;
		
		print join " -> ", map {$_->{name}} @$res;
		
		print "\n";
	}
}

sub parser_delete_postmod {
	if($_[0] =~ s/^(<.*?>)([^>]+)$/$1/) {
		return $2;
	}
	elsif($_[0] =~ s/^(.*?\])([^\]+])$/$1/) {
		return $2;
	}
	
	"";
}

sub parser_delete_attr {
	my $attr = {};
	
	if($_[0] =~ s/^(<.*?)\s+([^>]*)>$/$1>/) {
		my @data = split /\s+/, $2;
		
		foreach my $dt (@data) {
			my ($key, $value) = split /=/, $dt, 2;
			$attr->{$key} = $value;
		}
	}
	
	$attr;
}


1;

package MyCSS::Grammar::MyTree;

sub new {
	my ($class, $args) = @_;
	
	$class = ref $class if ref $class;
	
	bless $args, $class;
}

sub append_child {
	my ($to, $entry) = @_;
	
	if ($to->{last_child}) {
		$to->{last_child}->{next} = $entry;
		$entry->{prev} = $to->{last_child};
	}
	else {
		$to->{child} = $entry;
	}
	
	$entry->{next} = undef;
	$entry->{parent} = $to;
	
	$to->{last_child} = $entry;
}

sub parent {
	$_[0]->{parent} = $_[1] if @_ > 1;
	$_[0]->{parent};
}

sub child {
	$_[0]->{child} = $_[1] if @_ > 1;
	$_[0]->{child};
}

sub next {
	$_[0]->{next} = $_[1] if @_ > 1;
	$_[0]->{next};
}

sub prev {
	$_[0]->{prev} = $_[1] if @_ > 1;
	$_[0]->{prev};
}

sub name {
	$_[0]->{name} = $_[1] if @_ > 1;
	$_[0]->{name};
}

sub mod {
	$_[0]->{mod} = $_[1] if @_ > 1;
	$_[0]->{mod};
}

1;
