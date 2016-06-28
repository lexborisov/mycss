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

#include "mycss/selectors/parser.h"
#include "mycss/selectors/value_resource.h"

mycss_selectors_entry_t * mycss_selectors_parser_selector_create_new_entry(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector)
{
    mycss_selectors_entry_t* new_selector = mcobject_async_malloc(result->entry->mcasync_selectors_entries, result->selectors_entries_id, NULL);
    mycss_selectors_entry_clean(new_selector);
    
    if(selector) {
        selector->next = new_selector;
        new_selector->prev = selector;
    }
    
    result->result_entry->selector = new_selector;
    return new_selector;
}

/////////////////////////////////////////////////////////
//// Comma (,)
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_comma(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_result_entry_append_selector(result, result->result_entry, selector);
    
    selector->prev->next = NULL;
    selector->prev = NULL;
}

/////////////////////////////////////////////////////////
//// Combinator
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_combinator_greater_than(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_CHILD)
        selector->combinator = MyCSS_SELECTORS_COMBINATOR_DESCENDANT; // ">>"
    else
        selector->combinator = MyCSS_SELECTORS_COMBINATOR_CHILD;      // ">"
}

void mycss_selectors_parser_selector_combinator_plus(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_NEXT_SIBLING;
}

void mycss_selectors_parser_selector_combinator_tilde(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_FOLLOWING_SIBLING;
}

void mycss_selectors_parser_selector_combinator_column(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_COLUMN;
}

void mycss_selectors_parser_selector_combinator_whitespace(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_DESCENDANT;
}

/////////////////////////////////////////////////////////
//// Selector type
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_ident_type(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->type = MyCSS_SELECTORS_TYPE_ELEMENT;
    selector->key  = str;
}

void mycss_selectors_parser_selector_ident_attr(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->type = MyCSS_SELECTORS_TYPE_ATTRIBUTE;
    selector->key  = str;
}

void mycss_selectors_parser_selector_id(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->type = MyCSS_SELECTORS_TYPE_ID;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

void mycss_selectors_parser_selector_class(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->type = MyCSS_SELECTORS_TYPE_CLASS;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

/////////////////////////////////////////////////////////
//// Namespace
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_namespace(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = selector->key;
    
    if(str == NULL || str->length == 0) {
        myhtml_string_destroy(str, 0);
        selector->key = NULL;
        return;
    }
    
    if(str->length == 1 && *str->data == '*') {
        myhtml_string_destroy(str, 0);
        selector->key = NULL;
        return;
    }
    
    selector->ns = mycss_result_detect_namespace_by_name(result, str->data, str->length);
    
    myhtml_string_destroy(str, 0);
    selector->key = NULL;
}

void mycss_selectors_parser_selector_after_namespace(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->key = str;
}

/////////////////////////////////////////////////////////
//// Value and Modifier
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_value(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_object_attribute_t *attr = mycss_selectors_value_attribute_create(result, 1);
    
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    attr->value = str;
    selector->value = attr;
}

void mycss_selectors_parser_selector_modifier(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selector_value_attribute(selector->value)->mod = MyCSS_SELECTORS_MOD_I;
}

/////////////////////////////////////////////////////////
//// Pseudo and Function
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_pseudo_class(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->key  = str;
    selector->type = MyCSS_SELECTORS_TYPE_PSEUDO_CLASS;
    
    selector->sub_type = mycss_pseudo_begin_by_name(str->data, str->length);
    
    if(selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_UNKNOWN)
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

void mycss_selectors_parser_selector_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    selector->key   = str;
    selector->type  = MyCSS_SELECTORS_TYPE_FUNCTION;
    
    mycss_selectors_function_begin_f to_func = mycss_function_begin_by_name(str->data, str->length);
    
    if(to_func) {
        to_func(result, selector);
    }
    else {
        /* skip and set bad type for current selector */
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        mycss_selectors_begin_unknown(result, selector);
    }
}

void mycss_selectors_parser_selector_function_end(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

/////////////////////////////////////////////////////////
//// End and bad selectors
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_end(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
}

void mycss_selectors_parser_expectations_error(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    //printf("Expectations error!\n");
}

void mycss_selectors_parser_bad_token(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0) {
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
        selector->key = str;
        
        mycss_token_data_to_string(result->entry, token, selector->key, true);
        return;
    }
    
    mycss_token_data_to_string(result->entry, token, selector->key, false);
}



