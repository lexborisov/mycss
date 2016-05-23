package MyCSS::Grammar;

use utf8;
use strict;
use Encode;

my $GRAMMAR_SUB_INDEX = {
	'['  => \&GRAMMAR_LEFT_SQUARE_BRACKET,
	']'  => \&GRAMMAR_RIGHT_SQUARE_BRACKET,
	'\'' => \&GRAMMAR_BEGIN_APOSTROPHE,
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
			if(@current) {
				$result->{ $key } = [@current];
				push @$index, $key;
				
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
		$result->{ $key } = [@current];
		push @$index, $key;
	}
	
	($result, $index);
}

sub parser_file {
	my ($grammar, $filename) = @_;
	
	open my $fh, "<", $filename or die "Can't open file: $!";
	my @data = <$fh>;
	close $fh;
	
	$grammar->parser(join("", @data));
}

sub GRAMMAR_LEFT_SQUARE_BRACKET { # ([)
	my ($name, $rules, $idx, $tree, $open, $mod) = @_;
	
	my $entry = $tree->new({name => $name, mod => "["});
	$open->[-1]->append_child($entry);
	
	push @$open, $entry;
}

sub GRAMMAR_RIGHT_SQUARE_BRACKET { # (])
	my ($name, $rules, $idx, $tree, $open, $mod) = @_;
	
	if(@$open <= 1) {
		warn "RIGHT SQUARE BRACKET: try delete ROOT of tree! WRONG!\n";
		exit(1);
	}
	
	my $entry = grammar_open_delete_vertical_line($open, 1);
	
	$entry->mod($mod) if $mod;
}

sub GRAMMAR_BEGIN_APOSTROPHE { # (')
	my ($name, $rules, $idx, $tree, $open, $mod) = @_;
	
	my $entry = $tree->new({name => $name, mod => $mod});
	$open->[-1]->append_child($entry);
}

sub GRAMMAR_BEGIN_LESS_THAN_SIGN { # (<)
	my ($name, $rules, $idx, $tree, $open, $mod) = @_;
	
	my $entry = $tree->new({name => $name, mod => $mod});
	$open->[-1]->append_child($entry);
}

sub GRAMMAR_VERTICAL_LINE { # (|)
	my ($name, $rules, $idx, $tree, $open, $mod) = @_;
	
	my $idx_find = grammar_open_find_in_scope_entry_by_name($open, "|");
	
	if ($idx_find) {
		grammar_open_delete_remove_from_end($open, $open->[$idx_find], 1);
	}
	
	my $entry = $tree->new({name => $name, mod => "|"});
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
		
		if($name =~ /^(.)/)
		{
			unless(exists $GRAMMAR_SUB_INDEX->{$1}) {
				warn "Not find in index: $name";
				next;
			}
			
			$GRAMMAR_SUB_INDEX->{$1}->($name, $rules, $idx, $root, $open, $mod);
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
		
		my @tmp = @{shift @$curent_ends};
		
		foreach my $cend (@$curent_ends) {
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
		push @{$list}, [$entry];
	}
	
	$list;
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
	if($_[0] =~ s/(\>|'|\])([^']+)$/$1/) {
		return $2;
	}
	
	"";
}

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


