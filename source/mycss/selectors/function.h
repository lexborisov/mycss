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

#ifndef MyHTML_MyCSS_SELECTORS_FUNCTION_H
#define MyHTML_MyCSS_SELECTORS_FUNCTION_H
#pragma once

#define MyCSS_SELECTORS_FUNCTION_NAME_STATIC_SIZE 57

#ifdef __cplusplus
extern "C" {
#endif
    
#include "mycss/result.h"
#include "mycss/entry.h"
#include "mycss/selectors/myosi.h"

typedef void (*mycss_selectors_function_begin_f)(mycss_result_t* result, mycss_selectors_entry_t* selector);

struct mycss_selectors_function_index {
    mycss_parser_token_f parser;
    mycss_parser_token_f switch_parser;
}
typedef mycss_selectors_function_index_t;

struct mycss_selectots_function_begin_entry {
    const char* name;
    size_t length;
    
    mycss_selectors_function_begin_f func;
    
    size_t next;
    size_t curr;
}
typedef mycss_selectots_function_begin_entry_t;

mycss_selectors_function_begin_f mycss_function_begin_by_name(const char *name, size_t length);
const mycss_selectots_function_begin_entry_t * mycss_function_begin_entry_by_name(const char* name, size_t length);

void mycss_selectors_function_begin_nth_child(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_not(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_dir(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_matches(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_lang(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_drop(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_nth_of_type(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_nth_last_column(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_current(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_nth_last_child(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_nth_last_of_type(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_has(mycss_result_t* result, mycss_selectors_entry_t* selector);
void mycss_selectors_function_begin_nth_column(mycss_result_t* result, mycss_selectors_entry_t* selector);

void mycss_selectors_begin_unknown(mycss_result_t* result, mycss_selectors_entry_t* selector);
bool mycss_selectors_unknown_parser(mycss_result_t* result, mycss_token_t* token);

bool mycss_selectors_function_not_or_matches_parser(mycss_result_t* result, mycss_token_t* token);
bool mycss_selectors_function_has_parser(mycss_result_t* result, mycss_token_t* token);
bool mycss_selectors_function_nth_parser(mycss_result_t* result, mycss_token_t* token);
bool mycss_selectors_function_nth_of_selectors_parser(mycss_result_t* result, mycss_token_t* token);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_SELECTORS_FUNCTION_H */
