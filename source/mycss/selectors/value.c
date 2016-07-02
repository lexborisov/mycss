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
#include "mycss/selectors/value_resource.h"

/////////////////////////////////////////////////////////
//// Create
////
/////////////////////////////////////////////////////////
void * mycss_selectors_value_undef_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

mycss_selectors_object_attribute_t * mycss_selectors_value_attribute_create(mycss_result_t* result, bool set_clean)
{
    mycss_selectors_object_attribute_t* attr = (mycss_selectors_object_attribute_t*)
        mchar_async_malloc(result->entry->mchar, result->mchar_value_node_id, sizeof(mycss_selectors_object_attribute_t));
    
    if(set_clean)
        memset(attr, 0, sizeof(mycss_selectors_object_attribute_t));
    
    return attr;
}

/////////////////////////////////////////////////////////
//// Destroy
////
/////////////////////////////////////////////////////////
void * mycss_selectors_value_undef_destroy(mycss_result_t* result, mycss_selectors_type_t type, mycss_selectors_sub_type_t sub_type, void* value, bool self_destroy)
{
    myhtml_string_destroy(mycss_selector_value_string(value), 0);
    mcobject_async_free(result->entry->mcasync_string, value);
    
    return NULL;
}

void * mycss_selectors_value_id_destroy(mycss_result_t* result, mycss_selectors_type_t type, mycss_selectors_sub_type_t sub_type, void* value, bool self_destroy)
{
    myhtml_string_destroy(mycss_selector_value_string(value), 0);
    
    if(self_destroy) {
        mcobject_async_free(result->entry->mcasync_string, value);
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_class_destroy(mycss_result_t* result, mycss_selectors_type_t type, mycss_selectors_sub_type_t sub_type, void* value, bool self_destroy)
{
    myhtml_string_destroy(mycss_selector_value_string(value), 0);
    
    if(self_destroy) {
        mcobject_async_free(result->entry->mcasync_string, value);
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_element_destroy(mycss_result_t* result, mycss_selectors_type_t type, mycss_selectors_sub_type_t sub_type, void* value, bool self_destroy)
{
    myhtml_string_destroy(mycss_selector_value_string(value), 0);
    
    if(self_destroy) {
        mcobject_async_free(result->entry->mcasync_string, value);
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_attribute_destroy(mycss_result_t* result, mycss_selectors_type_t type, mycss_selectors_sub_type_t sub_type, void* value, bool self_destroy)
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

void * mycss_selectors_value_pseudo_class_function_destroy(mycss_result_t* result, mycss_selectors_type_t type, mycss_selectors_sub_type_t sub_type, void* value, bool self_destroy)
{
    if(sub_type > MyCSS_SELECTORS_SUB_TYPE_UNKNOWN && sub_type < MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_LAST_ENTRY)
        return (mycss_selectors_value_function_destroy_map[sub_type](result, value, self_destroy));
    
    return value;
}

/////////////////////////////////////////////////////////
//// Pseudo Class Function Create
////
/////////////////////////////////////////////////////////
void * mycss_selectors_value_pseudo_class_function_undef_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_current_create(mycss_result_t* result, bool set_clean)
{
    return mycss_result_entry_create(result);
}

void * mycss_selectors_value_pseudo_class_function_dir_create(mycss_result_t* result, bool set_clean)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    
    if(set_clean)
        myhtml_string_clean_all(str);
    
    return str;
}

void * mycss_selectors_value_pseudo_class_function_drop_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_has_create(mycss_result_t* result, bool set_clean)
{
    return mycss_result_entry_create(result);
}

void * mycss_selectors_value_pseudo_class_function_lang_create(mycss_result_t* result, bool set_clean)
{
    mycss_selectors_value_lang_t* lang = (mycss_selectors_value_lang_t*)
    mchar_async_malloc(result->entry->mchar, result->mchar_value_node_id, sizeof(mycss_selectors_value_lang_t));
    
    if(set_clean) {
        lang->next = NULL;
        myhtml_string_clean_all(&lang->str);
    }
    
    return lang;
}

void * mycss_selectors_value_pseudo_class_function_matches_create(mycss_result_t* result, bool set_clean)
{
    return mycss_result_entry_create(result);
}

void * mycss_selectors_value_pseudo_class_function_not_create(mycss_result_t* result, bool set_clean)
{
    return mycss_result_entry_create(result);
}

void * mycss_selectors_value_pseudo_class_function_nth_child_create(mycss_result_t* result, bool set_clean)
{
    mycss_an_plus_b_entry_t* anb_entry = (mycss_an_plus_b_entry_t*)
        mchar_async_malloc(result->entry->mchar, result->mchar_value_node_id, sizeof(mycss_an_plus_b_entry_t));
    
    if(set_clean)
        memset(anb_entry, 0, sizeof(mycss_an_plus_b_entry_t));
    
    return anb_entry;
}

void * mycss_selectors_value_pseudo_class_function_nth_column_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_nth_last_child_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_nth_last_column_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_nth_last_of_type_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_nth_of_type_create(mycss_result_t* result, bool set_clean)
{
    return NULL;
}

/////////////////////////////////////////////////////////
//// Pseudo Class Function Destroy
////
/////////////////////////////////////////////////////////
void * mycss_selectors_value_pseudo_class_function_undef_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    return NULL;
}

void * mycss_selectors_value_pseudo_class_function_current_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    return mycss_result_entry_destroy(result, value, self_destroy);
}

void * mycss_selectors_value_pseudo_class_function_dir_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    return myhtml_string_destroy(value, self_destroy);
}

void * mycss_selectors_value_pseudo_class_function_drop_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(self_destroy) {
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_has_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    return mycss_result_entry_destroy(result, value, self_destroy);
}

void * mycss_selectors_value_pseudo_class_function_lang_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(value == NULL)
        return NULL;
    
    myhtml_string_destroy(&mycss_selector_value_lang(value)->str, false);
    
    if(self_destroy) {
        mchar_async_free(result->entry->mchar, result->mchar_value_node_id, value);
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_matches_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    return mycss_result_entry_destroy(result, value, self_destroy);
}

void * mycss_selectors_value_pseudo_class_function_not_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    return mycss_result_entry_destroy(result, value, self_destroy);
}

void * mycss_selectors_value_pseudo_class_function_nth_child_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(value == NULL)
        return NULL;
    
    mycss_an_plus_b_entry_t *anb = value;
    
    if(anb->of) {
        // destroy this
    }
    
    if(self_destroy) {
        mchar_async_free(result->entry->mchar, result->mchar_value_node_id, value);
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_nth_column_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(self_destroy) {
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_nth_last_child_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(self_destroy) {
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_nth_last_column_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(self_destroy) {
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_nth_last_of_type_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(self_destroy) {
        return NULL;
    }
    
    return value;
}

void * mycss_selectors_value_pseudo_class_function_nth_of_type_destroy(mycss_result_t* result, void* value, bool self_destroy)
{
    if(self_destroy) {
        return NULL;
    }
    
    return value;
}


