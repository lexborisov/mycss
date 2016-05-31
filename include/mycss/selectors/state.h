/*
 Copyright (C) 2016 Alexander Borisov
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 
 Author: lex.borisov@gmail.com (Alexander Borisov)
*/

#ifndef MyHTML_MyCSS_SELECTORS_STATE_H
#define MyHTML_MyCSS_SELECTORS_STATE_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mycss/myosi.h"
#include "mycss/mycss.h"
#include "mycss/selectors/myosi.h"
#include "mycss/selectors/parser.h"
#include "myhtml/utils.h"

bool mycss_selectors_state_token_all(mycss_result_t* result, mycss_token_t* token);
bool mycss_selectors_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token);

/* <combinator> */
bool mycss_selectors_state_combinator(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_combinator_greater_than(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);

/* <simple-selector> */
bool mycss_selectors_state_simple_selector(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_dash_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_dash_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_dash_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_dash_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_dash_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_equals(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_equals_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_equals_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_equals_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_equals_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_include_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_include_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_include_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_include_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_include_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_prefix_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_prefix_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_prefix_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_prefix_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_prefix_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_substring_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_substring_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_substring_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_substring_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_substring_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_suffix_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_suffix_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_suffix_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_suffix_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar_ident_suffix_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_dash_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_dash_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_dash_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_dash_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_dash_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_equals(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_equals_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_equals_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_equals_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_equals_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_dash_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_dash_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_dash_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_dash_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_dash_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_equals(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_equals_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_equals_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_equals_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_equals_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_include_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_include_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_include_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_include_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_include_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_prefix_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_prefix_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_prefix_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_prefix_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_prefix_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_substring_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_substring_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_substring_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_substring_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_substring_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_suffix_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_suffix_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_suffix_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_suffix_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar_ident_suffix_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_include_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_include_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_include_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_include_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_include_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_prefix_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_prefix_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_prefix_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_prefix_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_prefix_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_substring_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_substring_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_substring_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_substring_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_substring_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_suffix_match(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_suffix_match_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_suffix_match_ident_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_suffix_match_string(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_suffix_match_string_i(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon_colon(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon_colon_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_full_stop(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_ident_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* sel_entry, mycss_token_t* token);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_SELECTORS_STATE_H */
