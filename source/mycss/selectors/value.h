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

#ifndef MyHTML_MyCSS_SELECTORS_VALUE_H
#define MyHTML_MyCSS_SELECTORS_VALUE_H
#pragma once

#define mycss_selector_value_attribute(obj) ((mycss_selectors_object_attribute_t*)(obj))
#define mycss_selector_value_string(obj) ((myhtml_string_t*)(obj))
#define mycss_selector_value_function(obj) ((mycss_result_entry_t*)(obj))
#define mycss_selector_value_destroy(result, type, value, self_destroy) (mycss_selectors_value_destroy_map[type](result, type, value, self_destroy))

#ifdef __cplusplus
//extern "C" {
#endif

#include "mycss/result.h"
#include "mycss/entry.h"
#include "mycss/selectors/myosi.h"
#include "myhtml/utils/mchar_async.h"

typedef void * (*mycss_selectors_value_destroy_f)(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy);

struct mycss_selectors_value_attribute {
    myhtml_string_t* value;
    
    mycss_selectors_match_t match;
    mycss_selectors_mod_t mod;
}
typedef mycss_selectors_object_attribute_t;

mycss_result_entry_t * mycss_selectors_value_function_create(mycss_result_t* result, bool set_clean);
void * mycss_selectors_value_function_destroy(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy);

mycss_selectors_object_attribute_t * mycss_selectors_value_attribute_create(mycss_result_t* result, bool set_clean);
void * mycss_selectors_value_attribute_destroy(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy);

void * mycss_selectors_value_undef_create(mycss_result_t* result, bool set_clean);
void * mycss_selectors_value_undef_destroy(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_SELECTORS_VALUE_H */
