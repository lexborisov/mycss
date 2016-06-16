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

#ifndef MyHTML_MyCSS_RESULT_H
#define MyHTML_MyCSS_RESULT_H
#pragma once

#include "mycss/myosi.h"
#include "mycss/an_plus_b.h"
#include "mycss/namespace/myosi.h"
#include "mycss/namespace/init.h"
#include "mycss/selectors/myosi.h"
#include "mycss/selectors/init.h"
#include "mycss/rules/myosi.h"
#include "mycss/rules/init.h"
#include "mycss/media/myosi.h"
#include "mycss/media/init.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mycss_result {
    /* refs */
    mycss_entry_t* entry;
    size_t mchar_value_node_id;
    size_t mchar_selector_list_node_id;
    size_t mcobject_result_entries_node_id;
    
    mycss_result_entry_t* result_entry_first; /* first */
    mycss_result_entry_t* result_entry;       /* current */
    mycss_namespace_t* ns;
    mycss_selectors_t* selectors;
    mycss_rules_t* rules;
    mycss_media_t* media;
    mycss_an_plus_b_t* anb;
    
    mycss_parser_token_f parser;
    mycss_parser_token_f switch_parser;
    void* state;
    
    size_t selectors_entries_id;
    size_t string_node_id;
    size_t namespace_entries_id;
};

struct mycss_result_entry {
    mycss_selectors_entry_t** selector_list;
    size_t selector_list_length;
    
    mycss_selectors_entry_t* selector; /* current selector/last entry */
    
    mycss_result_entry_t* parent;
    mycss_result_entry_t* next;
    mycss_result_entry_t* prev;
};

mycss_result_t * mycss_result_create(void);
mycss_status_t mycss_result_init(mycss_entry_t* entry, mycss_result_t* result);
mycss_status_t mycss_result_clean_all(mycss_result_t* result);
mycss_result_t * mycss_result_destroy(mycss_result_t* result, bool self_destroy);
void mycss_result_end(mycss_result_t* result);

mycss_result_entry_t * mycss_result_entry_create(mycss_result_t* result);
void mycss_result_entry_clean(mycss_result_entry_t* result_entry);
mycss_result_entry_t * mycss_result_entry_destroy(mycss_result_t* result, mycss_result_entry_t* result_entry, bool self_destroy);
mycss_result_entry_t * mycss_result_entry_create_and_push(mycss_result_t* result);
mycss_result_entry_t * mycss_result_entry_append_selector(mycss_result_t* result, mycss_result_entry_t* res_entry, mycss_selectors_entry_t* selector);
mycss_result_entry_t * mycss_result_get_parent_set_parser(mycss_result_t* result, mycss_result_entry_t* res_entry);
mycss_result_entry_t * mycss_result_entry_create_next_level_of_selectors(mycss_result_t* result, mycss_result_entry_t* current_res_entry);

void mycss_result_entry_print(mycss_result_t* result, mycss_result_entry_t* res_entry, FILE* fh);

size_t mycss_result_detect_namespace_by_name(mycss_result_t* result, const char* ns, size_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_RESULT_H */
