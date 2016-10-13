#!/usr/bin/perl -w

use utf8;
use strict;

use MyHTML::Base;

# extra module
use HTML::MyHTML;

# The Code
#my $SAVE_TO_DIR = "../source/mycss";
my $SAVE_TO_DIR = "/new/C-git/Modest/source/mycss";

my $STATIC_DECLARATION_NAME_INDEX_LENGTH = 263;
my $STATIC_DECLARATION_VALUE_INDEX_LENGTH = 227;

my $PREFIX_PROPERTY_PARSER = "mycss_property_parser_";

my $PREFIX_PROPERTY = "MyCSS_PROPERTY_TYPE_";
my $PREFIX_PROPERTY_UNDEF = "MyCSS_PROPERTY_TYPE_UNDEF";

my $PREFIX_PROPERTY_VALUE = "MyCSS_PROPERTY_VALUE_";
my $PREFIX_PROPERTY_VALUE_UNDEF = "MyCSS_PROPERTY_VALUE_UNDEF";

my $ENUM_NAME = "mycss_property_type";
my $ENUM_VALUE = "mycss_property";

# The Code
my ($tree, $node) = load_properties("data/property.txt");

#$STATIC_DECLARATION_NAME_INDEX_LENGTH = test_name_result($tree, $node);
#$STATIC_DECLARATION_VALUE_INDEX_LENGTH = test_value_result($tree, $node);

my ($index_name, $index_value) = create_const_file($tree, $node, 1);
create_resource_file($tree, $node, $index_name, $index_value);

sub create_const_file {
        my ($tree, $node, $save_result) = @_;
        
        my $utils = MyHTML::Base->new(dirs => {source => $SAVE_TO_DIR, template => "tmpl"});
        my $data_const = $utils->read_tmpl("property_const.h");
        
        my ($enum_name, $index_name) = create_name_enum($tree, $node);
        my ($enum_value, $index_value) = create_value_enum($tree, $node);
        
        if ($save_result){
                my @res;
                push @res, @$enum_name;
                push @res, @$enum_value;
                
                $utils->save_src("property/const.h", $data_const, {BODY => join("", @res)});
		}
        
        ($index_name, $index_value);
}

sub create_resource_file {
        my ($tree, $node, $index_name, $index_value) = @_;
        
        my $utils = MyHTML::Base->new(dirs => {source => $SAVE_TO_DIR, template => "tmpl"});
        my $data_const = $utils->read_tmpl("property_resources.h");
        
        my ($result_name, $index_by_name) = create_name_result($tree, $node, $STATIC_DECLARATION_NAME_INDEX_LENGTH);
        my $static_list_name = create_static_list_index_for_name($result_name, $STATIC_DECLARATION_NAME_INDEX_LENGTH);
        
        my $result_value = create_value_result($tree, $node, $STATIC_DECLARATION_VALUE_INDEX_LENGTH);
        my $static_list_value = create_static_list_index_for_value($result_value, $STATIC_DECLARATION_VALUE_INDEX_LENGTH);
        
        $utils->save_src("property/resources.h", $data_const, {
                BODY => join("\n",
                        $static_list_name, 
                        $static_list_value
                ),
                DEFINED => "#define MyCSS_PROPERTY_STATIC_INDEX_FOR_SEARCH_SIZE $STATIC_DECLARATION_NAME_INDEX_LENGTH\n".
                "#define MyCSS_PROPERTY_VALUE_STATIC_INDEX_FOR_SEARCH_SIZE $STATIC_DECLARATION_VALUE_INDEX_LENGTH"
        });
        
        create_resource_name_file($index_name, $index_value);
        create_serialization_file($index_name, $index_by_name);
        #print_mycss_functions($index_name);
        #print_modest_style_map($index_name);
}

sub create_resource_name_file {
        my ($index_name, $index_value) = @_;
        
        my $utils = MyHTML::Base->new(dirs => {source => $SAVE_TO_DIR, template => "tmpl"});
        my $data_const = $utils->read_tmpl("property_resources_name.h");
        
        $utils->save_src("property/resources_name.h", $data_const, {
                BODY => join("\n",
                        create_index($index_name, "mycss_property_index_type_name"),
                        create_index($index_value, "mycss_property_index_type_value")
                ),
        });
}

sub create_serialization_file {
        my ($index_name, $index_by_name) = @_;
        
        my $utils = MyHTML::Base->new(dirs => {source => $SAVE_TO_DIR, template => "tmpl"});
        my $data_const = $utils->read_tmpl("declaration_serialization_resources.h");
        
        $utils->save_src("declaration/serialization_resources.h", $data_const, {
                BODY => create_serialization_index($index_name, $index_by_name, "mycss_declaration_serialization_map_by_type")
        });
}

sub create_serialization_index {
        my ($index_name, $index_by_name, $var_name) = @_;
        
        my @res;
        foreach my $name (@$index_name) {
                $name = "undef" unless $name;
                $name = lc(name_to_norm($name));
                
                if (exists $index_by_name->{$name}->{serialize} && $index_by_name->{$name}->{serialize}) {
                        push @res, $index_by_name->{$name}->{serialize};
                }
                else {
                        push @res, "undef";
                }
        }
        
        return
        "static mycss_declaration_serialization_f $var_name\[] = \n{\n\tmycss_declaration_serialization_".
        join(",\n\tmycss_declaration_serialization_", @res).
        "\n};\n";
}

sub create_index {
        my ($index, $var_name) = @_;
        
        my @res;
        foreach my $name (@$index) {
                push @res, qq~\t"$name"~;
        }
        
        return
        "static const char * $var_name\[] = \n{\n".
        join(",\n", @res).
        "\n};\n";
}

sub print_mycss_functions {
        my ($index) = @_;
        
        my @res;
        foreach my $name (@$index) {
                $name = "undef" unless $name;
                $name = name_to_norm($name);
                
                push @res, qq~mycss_property_parser_$name~;
        }
        
        pop @res;
        
        foreach my $name (@res) {
                print qq~bool $name(mycss_entry_t* entry, mycss_token_t* token, bool last_response);\n~;
        }
        
        print "\n\n";
        
        foreach my $name (@res) {
                print qq~bool $name(mycss_entry_t* entry, mycss_token_t* token, bool last_response)\n{\n~;
                print "\treturn mycss_property_shared_switch_to_parse_error(entry);\n";
                print "}\n\n";
        }
}

sub print_modest_style_map {
        my ($index) = @_;
        
        my @res;
        foreach my $name (@$index) {
                $name = "undef" unless $name;
                $name = name_to_norm($name);
                
                push @res, qq~modest_style_map_collate_declaration_$name~;
        }
        
        pop @res;
        
        print
        "static const modest_style_map_collate_f modest_style_map_static_collate_declaration\[] = \n{\n\t".
        join(",\n\t", @res).
        "\n};\n\n";
        
        foreach my $name (@res) {
                print qq~void $name(myhtml_tree_node_t* node, modest_finder_thread_declaration_t* thr_dec);\n~;
        }
        
        print "\n\n";
        
        foreach my $name (@res) {
                print qq~void $name(myhtml_tree_node_t* node, modest_finder_thread_declaration_t* thr_dec)\n{\n~;
                print "";
                print "}\n\n";
        }
}

sub create_name_result {
        my ($tree, $node, $index_length) = @_;
        my $result = {};
        my $index_by_name = {};
        
        while($node) {
                my $info = $node->info($tree);
                
                if($info->{tag} ne "property") {
                        $node = $node->next;
                        next;
                }
                
                my $name = lc($info->{attr}->{name});
                my $parser = $info->{attr}->{parser};
                
                unless(defined $parser) {
					$parser = "$PREFIX_PROPERTY_PARSER". lc(name_to_norm($name));
				}
                
                my $id = get_index_id($name, $index_length);
                push @{$result->{$id}}, [$name, length($name), $parser];
                
                $index_by_name->{ name_to_norm($name) }->{serialize} = $info->{attr}->{serialize};
                
                $node = $node->next;
        }
        
        ($result, $index_by_name);
}

sub create_name_enum {
        my ($tree, $node) = @_;
        
        my $idx = 1;
        my $res = [];
        my @for_sort;
        my @index = ("");
        
        while($node) {
                my $info = $node->info($tree);
                
                if($info->{tag} ne "property") {
                        $node = $node->next;
                        next;
                }
                
                push @for_sort, $info->{attr}->{name};
                
                $node = $node->next;
        }
        
        my $norm_prefix = uc($ENUM_NAME);
        $norm_prefix =~ s/^my/My/i;
        
        push @$res, [$norm_prefix ."_UNDEF", "0x0000"];
        foreach my $name (sort {$a cmp $b} @for_sort) {
                push @$res, [$norm_prefix ."_". uc(name_to_norm($name)), sprintf("0x%04x", $idx++)];
                
                push @index, $name;
        }
        push @$res, [$norm_prefix ."_LAST_ENTRY", sprintf("0x%04x", $idx++)];
        push @index, "";
        
        my @return;
        push @return, "enum $ENUM_NAME {\n\t";
        push @return, join(",\n\t", @{format_list_text($res, "= ")}), "\n";
        push @return, "}\ntypedef $ENUM_NAME\_t;\n\n";
        
        (\@return, \@index);
}

sub create_value_result {
        my ($tree, $node, $index_length) = @_;
        my $result = {};
        my $exists = {};
        
        while($node) {
                my $info = $node->info($tree);
                
                if($info->{tag} ne "property") {
                        $node = $node->next;
                        next;
                }
                
                if($info->{attr}->{value}) {
                        my $val_list = split_value_only_ident(lc($info->{attr}->{value}));
                        
                        foreach my $val (@$val_list) {
                                next if exists $exists->{$val};
                                
                                my $id = get_index_id($val, $index_length);
                                push @{$result->{$id}}, [$val, length($val)];
                                
                                $exists->{$val} = 1;
                        }
                }

                $node = $node->next;
        }
        
        $result;
}

sub create_value_enum {
        my ($tree, $node) = @_;
        
        my $idx = 1;
        my $res = [];
        
        my $by_prop = {};
        my $index_val = {};
        my @index = ("");
        
        my %all_prop;
        
        while($node) {
                my $info = $node->info($tree);
                
                if($info->{tag} ne "property") {
                        $node = $node->next;
                        next;
                }
                
                if ($info->{attr}->{value}) {
                        my $val_list = split_value(lc($info->{attr}->{value}));
                        
                        foreach my $val (@$val_list) {
                                $all_prop{$val} = 1;
                                push @{$by_prop->{ $info->{attr}->{name} }}, $val;
                        }
                }
                
                $node = $node->next;
        }
        
        my $norm_prefix = uc($ENUM_VALUE);
        $norm_prefix =~ s/^my/My/i;
        
        push @$res, [$norm_prefix ."_VALUE_UNDEF", "0x0000"];
        foreach my $name (sort {$a cmp $b} keys %all_prop) {
                my $id = sprintf("0x%04x", $idx++);
                my $norm_name = $norm_prefix ."_VALUE_". uc(name_to_norm($name));
                
                push @$res, [$norm_name, $id];
                push @index, $name;
                
                $index_val->{$norm_name} = $id;
        }
        push @$res, [$norm_prefix ."_VALUE_LAST_ENTRY", sprintf("0x%04x", $idx++)];
        push @index, "";
        
        my @return;
        push @return, "enum $ENUM_VALUE\_value {\n\t";
        push @return, join(",\n\t", @{format_list_text($res, "= ")}), "\n";
        push @return, "}\ntypedef $ENUM_VALUE\_value\_t;\n\n";
        
        foreach my $key (sort {$a cmp $b} keys %$by_prop)
        {
                my $norm_key = name_to_norm($key);
                $res = [];
                
                foreach my $name (sort {$a cmp $b} @{$by_prop->{$key}}) {
                        my $norm_name = $norm_prefix ."_VALUE_". uc(name_to_norm($name));
                        
                        push @$res, [$norm_prefix ."_". uc($norm_key) ."_". uc(name_to_norm($name)), $index_val->{$norm_name}];
                }
                
                push @return, "enum $ENUM_VALUE\_$norm_key {\n\t";
                push @return, join(",\n\t", @{format_list_text($res, "= ")}), "\n";
                push @return, "}\ntypedef $ENUM_VALUE\_$norm_key\_t;\n\n";
        }
        
        (\@return, \@index);
}

sub split_value {
        my ($value) = @_;
        my @res;
        
        $value =~ s/^\s+//;
        $value =~ s/\s+$//;
        
        $value =~ s/[\|\[\]]+//g;
        $value =~ s/\{[0-9,\s]+\}//g;
        $value =~ s/\#\d+?//g;
                
        foreach my $val (split /\s+/, $value) {
                if($val =~ s/^<([^>]+)>$/$1/) {
                        $val = "$val";
                }
                
                push @res, $val;
        }
        
        \@res;
}

sub split_value_only_ident {
        my ($value) = @_;
        my @res;
        
        $value =~ s/^\s+//;
        $value =~ s/\s+$//;
        
        $value =~ s/[\|\[\]]+//g;
        $value =~ s/\{[0-9,\s]+\}//g;
        $value =~ s/\#\d+?//g;
                
        foreach my $val (split /\s+/, $value) {
                next if $val =~ /^<([^>]+)>$/;
                
                push @res, $val;
        }
        
        \@res;
}

sub test_name_result {
        my ($tree, $node) = @_;
        my $op = [0, undef];
        
        foreach my $idx (1..2048) {
                my ($result, $index_by_name) = create_name_result($tree, $node, $idx);
                my $res_max = test_result_max_value($result, 0);
                
                if(!defined($op->[1]) || $op->[1] > $res_max) {
                        $op->[0] = $idx;
                        $op->[1] = $res_max;
                }
        }
        
        print "Best:\n";
        print $op->[0], ": ", $op->[1], "\n";
        
        $op->[0];
}

sub test_value_result {
        my ($tree, $node) = @_;
        my $op = [0, undef];
        
        foreach my $idx (1..2048) {
                my $result = create_value_result($tree, $node, $idx);
                my $res_max = test_result_max_value($result, 0);
                
                if(!defined($op->[1]) || $op->[1] > $res_max) {
                        $op->[0] = $idx;
                        $op->[1] = $res_max;
                }
        }
        
        print "Best:\n";
        print $op->[0], ": ", $op->[1], "\n";
        
        $op->[0];
}

sub test_result_max_value {
        my ($res, $is_print) = @_;
        
        my $max = 0;
        foreach my $id (sort {scalar(@{$res->{$a}}) <=> scalar(@{$res->{$b}})} keys %$res) {
                print "$id: ", scalar(@{$res->{$id}}), "\n" if $is_print;
                
                $max = scalar(@{$res->{$id}}) if $max < scalar(@{$res->{$id}});
        }
        
        $max;
}

sub get_index_id {
        use bytes;
        
        my @chars = split //, $_[0];
        my $f = ord(lc($chars[0]));
        my $l = ord(lc($chars[-1]));
        
        ((($f * $l * scalar(@chars))) % $_[1]) + 1;
}

sub create_sub_static_list_index {
        my ($result, $struct, $offset, $sub, $index_len) = @_;
        
        my @list_sorted = sort {$a->[1] <=> $b->[1]} @$result[0..$#$result];
        
        foreach my $i (1..$#list_sorted) {
                my $cur = $offset;
                $offset++;
                push @$struct, "\t{".
                '"'. $list_sorted[$i]->[0] .'", '. $list_sorted[$i]->[1] .', '.
                $sub->($list_sorted[$i]), ', '.
                ($i < $#list_sorted ? $offset : 0) .", $cur},\n";
        }
        
        $offset;
}

sub create_static_list_index {
        my ($result, $sub, $sub_null, $index_len) = @_;
        
        my @res;
        my $struct = [];
        my $offset = $index_len + 1;
        
        foreach my $i (0..$index_len)
        {
                if(exists $result->{$i}) {
                        my $id = 0;
                        
                        if(scalar @{$result->{$i}} > 1) {
                                $offset = create_sub_static_list_index($result->{$i}, $struct, $offset, $sub, $index_len);
                                
                                $id = $offset - (@{$result->{$i}} - 1);
                        }
                        
                        my @list_sorted = sort {$a->[1] <=> $b->[1]} @{$result->{$i}}[0..$#{$result->{$i}}];
                        
                        push @res, "\t{".
                        '"'. $list_sorted[0]->[0] .'", '. $list_sorted[0]->[1] .', '.
                        $sub->($list_sorted[0]), ', '.
                        "$id, $i},\n";
				}
				else {
                        push @res, "\t{NULL, 0, ". $sub_null->() .", 0, 0},\n";
                }
        }
        
        return [@res, @$struct];
}

sub create_static_list_index_for_name {
        my ($result, $index_len) = @_;
        
        my $struct = create_static_list_index($result,
        sub {
                return $PREFIX_PROPERTY . uc(name_to_norm($_[0][0])) .", ". $_[0][2];
        },
        sub {
                return $PREFIX_PROPERTY_UNDEF. ", NULL";
        },
        $index_len);
        
        "static const mycss_property_index_static_entry_t mycss_property_index_static_for_search[] = \n{\n". join("", @$struct) ."};\n"
}

sub create_static_list_index_for_value {
        my ($result, $index_len) = @_;
        
        my $struct = create_static_list_index($result,
        sub {
                return $PREFIX_PROPERTY_VALUE . uc(name_to_norm($_[0][0]));
        },
        sub {
                return $PREFIX_PROPERTY_VALUE_UNDEF;
        },
        $index_len);
        
        "static const mycss_property_value_index_static_entry_t mycss_property_value_index_static_for_search[] = \n{\n". join("", @$struct) ."};\n"
}

sub name_to_norm {
        my ($name) = @_;
        $name =~ s/[-]+/_/g;
        $name;
}

sub load_properties {
        my ($filename) = @_;
        
        open my $fh, "<", $filename or die "Can't load $filename: $!";
        binmode $fh, ":utf8";
        
        my @lines = <$fh>;
        
        my $myhtml = HTML::MyHTML->new(MyHTML_OPTIONS_PARSE_MODE_SINGLE, 1);
        my $tree = $myhtml->new_tree();
        
        $tree->parse_flags_set(MyHTML_TREE_PARSE_FLAGS_SKIP_WHITESPACE_TOKEN);
        
        $myhtml->parse($tree, MyHTML_ENCODING_UTF_8, join("", @lines));
		
        close $fh;
        
        ($tree, $tree->body->child);
}

sub format_list_text {
        my ($list, $join_val) = @_;
        
        my ($max, $len) = (0, 0);
        foreach my $struct (@$list) {
                $len = length($struct->[0]);
                $max = $len if $len > $max;
        }
        
        my @res;
        foreach my $struct (@$list) {
                $len = $max - length($struct->[0]);
                push @res, sprintf("%s%$len"."s %s%s", $struct->[0], ($len ? " " : ""), $join_val, $struct->[1]);
        }
        
        \@res;
}


