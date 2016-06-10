#!/usr/bin/perl -w

use utf8;
use strict;

my $static_list_index_length = 57;
my $func_map = {
        not => "",
        matches => "",
        has => "",
        dir => "",
        lang => "",
        current => "",
        drop => "",
        'nth-child' => "",
        'nth-last-child' => "",
        'nth-of-type' => "",
        'nth-last-of-type' => "",
        'nth-column' => "",
        'nth-last-column' => ""
};

my $INDEX_OF_NAMES = print_functions();

my $result = create_result($static_list_index_length);
my $static_list = create_static_list_index($result);

print $static_list, "\n";

sub create_result {
        my ($static_list_index_length) = @_;
        my $result = {};
        
        foreach my $ns (sort {$a cmp $b} keys %$func_map)
        {
                my $id = get_index_id($ns, $static_list_index_length);
                
                push @{$result->{$id}}, [$ns, length($ns)];
        }
        
        my $count = 1;
        print "MyCSS_SELECTORS_SUB_TYPE_UNKNOWN = 0x001,\n";
        foreach my $name (sort {$a cmp $b} keys %$func_map) {
                print "MyCSS_SELECTORS_SUB_TYPE_FUNCTION_", uc(name_to_correct_name($name)), " = ", sprintf("0x%03x", ++$count), ",\n";
        }
        
        print "\n";
        
        $result;
}

sub test_result {
        my $op = [0, undef];
        
        foreach my $idx (1..1024) {
                my $result = create_result($idx);
                my $res_max = test_result_max_value($result, 0);
                
                if(!defined($op->[1]) || $op->[1] > $res_max) {
                        $op->[0] = $idx;
                        $op->[1] = $res_max;
                }
        }
        
        print "Best:\n";
        print $op->[0], ": ", $op->[1], "\n";
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
        my ($result, $struct, $offset) = @_;
        
        my @list_sorted = sort {$a->[1] <=> $b->[1]} @$result[0..$#$result];
        
        foreach my $i (1..$#list_sorted) {
                my $cur = $offset;
                $offset++;
                push @$struct, "\t{".
                '"'. $list_sorted[$i]->[0] .'", '. $list_sorted[$i]->[1] .', '.
                $INDEX_OF_NAMES->{$list_sorted[$i]->[0]}, ', '.
                ($i < $#list_sorted ? $offset : 0) .", $cur},\n";
        }
        
        $offset;
}

sub create_static_list_index {
        my ($result) = @_;
        
        my @res;
        my $struct = [];
        my $offset = $static_list_index_length + 1;
        
        foreach my $i (0..$static_list_index_length)
        {
                if(exists $result->{$i}) {
                        my $id = 0;
                        
                        if(scalar @{$result->{$i}} > 1) {
                                $offset = create_sub_static_list_index($result->{$i}, $struct, $offset);
                                
                                $id = $offset - (@{$result->{$i}} - 1);
                        }
                        
                        my @list_sorted = sort {$a->[1] <=> $b->[1]} @{$result->{$i}}[0..$#{$result->{$i}}];
                        
                        push @res, "\t{".
                        '"'. $list_sorted[0]->[0] .'", '. $list_sorted[0]->[1] .', '.
                        $INDEX_OF_NAMES->{$list_sorted[0]->[0]}, ', '.
                        "$id, $i},\n";
				}
				else {
                        push @res, "\t{NULL, 0, NULL, 0, 0},\n";
                }
        }
        
        "static const mycss_selectots_function_begin_entry_t mycss_selectors_function_begin_map_index[] = \n{\n". join("", @res, @$struct) ."};\n"
}

sub print_functions {
        my $headers;
        foreach my $name (keys %$func_map) {
                my $corect_name = name_to_correct_name($name);
                $headers->{$name} = "mycss_selectors_function_begin_$corect_name";
        }
        
        print join( "\n", map {"void ". $headers->{$_} ."(mycss_result_t* result, mycss_selectors_entry_t* selector);"} keys %$headers), "\n\n";
        
        foreach my $key (keys %$headers) {
                print "void ". $headers->{$key} ."(mycss_result_t* result, mycss_selectors_entry_t* selector)\n{\n\t\n}\n\n";
        }
        
        $headers;
}

sub name_to_correct_name {
        my ($name) = @_;
        $name =~ s/[-]+/_/g;
        $name;
}

