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

#include "mycss/selectors/value.h"

mycss_result_entry_t * mycss_selectors_value_function_create(mycss_result_t* result, bool set_clean)
{
    return mycss_result_entry_create(result);
}

void * mycss_selectors_value_function_destroy(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy)
{
    return mycss_result_entry_destroy(result, value, self_destroy);
}

mycss_selectors_object_attribute_t * mycss_selectors_value_attribute_create(mycss_result_t* result, bool set_clean)
{
    mycss_selectors_object_attribute_t* attr = (mycss_selectors_object_attribute_t*)
        mchar_async_malloc(result->entry->mchar, result->mchar_value_node_id, sizeof(mycss_selectors_object_attribute_t));
    
    if(set_clean)
        memset(attr, 0, sizeof(mycss_selectors_object_attribute_t));
    
    return attr;
}

void * mycss_selectors_value_attribute_destroy(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy)
{
    if(mycss_selector_value_attribute(value)->value) {
        myhtml_string_destroy(mycss_selector_value_attribute(value)->value, 0);
        mcobject_async_free(result->entry->mcasync_string, mycss_selector_value_attribute(value)->value);
    }
    
    if(self_destroy) {
        mchar_async_free(result->entry->mchar, result->mchar_value_node_id, value);
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_undef_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_undef_destroy(mycss_result_t* result, mycss_selectors_type_t type, void* value, bool self_destroy)
{
    myhtml_string_destroy(mycss_selector_value_string(value), 0);
    mcobject_async_free(result->entry->mcasync_string, value);
    
    return NULL;
}
