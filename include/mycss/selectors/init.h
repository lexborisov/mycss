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

#ifndef MyHTML_MyCSS_SELECTORS_INIT_H
#define MyHTML_MyCSS_SELECTORS_INIT_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <mycss/result.h>
#include <mycss/entry.h>
#include <mycss/selectors/myosi.h>
#include <mycss/namespace/init.h>

mycss_selectors_t * mycss_selectors_create(void);
mycss_status_t mycss_selectors_init(mycss_entry_t* entry, mycss_selectors_t* selectors);
mycss_status_t mycss_selectors_clean_all(mycss_selectors_t* selectors);
mycss_selectors_t * mycss_selectors_destroy(mycss_selectors_t* selectors, bool self_destroy);
void mycss_selectors_end(mycss_result_entry_t* res_entry, mycss_selectors_t* selectors, bool self_destroy);

void mycss_selectors_entry_clean(mycss_selectors_entry_t* sel_entry);
mycss_selectors_entry_t * mycss_selectors_entry_destroy(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, bool self_destroy);

void * mycss_selectors_entry_value_destroy(mycss_result_t* result, mycss_selectors_entry_t* entry, bool destroy_self);

mycss_selectors_entry_t * mycss_selectors_entry_find_first(mycss_selectors_entry_t* selector);
void mycss_selectors_print_selector(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, FILE* fh);
void mycss_selectors_print_chain(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, FILE* fh);

mycss_selectors_entry_t * mycss_selectors_entry_create(mycss_selectors_t* selectors);
    
mycss_selectors_entry_t ** mycss_selectors_entry_list_create(mycss_selectors_t* selectors);
mycss_selectors_entry_t ** mycss_selectors_entry_list_add_one(mycss_selectors_t* selectors, mycss_selectors_entry_t** list, size_t current_size);
mycss_selectors_entry_t ** mycss_selectors_entry_list_destroy(mycss_selectors_t* selectors, mycss_selectors_entry_t** list);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_SELECTORS_INIT_H */
