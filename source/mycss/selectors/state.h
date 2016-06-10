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

#include "mycss/result.h"
#include "mycss/entry.h"
#include "mycss/selectors/myosi.h"
#include "mycss/selectors/parser.h"
#include "myhtml/utils.h"

bool mycss_selectors_state_token_all(mycss_result_t* result, mycss_token_t* token);
bool mycss_selectors_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token);
void mycss_selectors_state_end(mycss_result_t* result);

/* <combinator> */
bool mycss_selectors_state_combinator(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_combinator_greater_than(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);

/* <simple-selector> */
bool mycss_selectors_state_simple_selector(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_left_bracket_ident_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon_colon(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon_colon_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_colon_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_full_stop(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_simple_selector_ident_vertical_bar(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_shared_after_attr_modifier(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_shared_after_attribute_value(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_shared_after_wq_name_attr(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
bool mycss_selectors_state_shared_after_attr_matcher(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_SELECTORS_STATE_H */
