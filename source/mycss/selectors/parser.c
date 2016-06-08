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

mycss_selectors_entry_t * mycss_selectors_parser_selector_create_new_entry(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector)
{
    if(selector && (selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_GOOD) == 0)
    {
        if(selector->key) {
            myhtml_string_destroy(selector->key, 0);
            mcobject_async_free(result->entry->mcasync_string, selector->key);
        }
        
        if(selector->value) {
            myhtml_string_destroy(selector->value, 0);
            mcobject_async_free(result->entry->mcasync_string, selector->value);
        }
        
        mycss_selectors_entry_t* selector_prev = selector->prev;
        mycss_selectors_entry_clean(selector);
        selector->prev = selector_prev;
        
        return selector;
    }
    
    selectors->selector = mcobject_async_malloc(result->entry->mcasync_selectors_entries, result->selectors_entries_id, NULL);
    
    mycss_selectors_entry_clean(selectors->selector);
    
    if(selector) {
        selector->next = selectors->selector;
        selectors->selector->prev = selector;
    }
    
    return selectors->selector;
}

/////////////////////////////////////////////////////////
//// Combinator
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_combinator_greater_than(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_CHILD)
        selector->combinator = MyCSS_SELECTORS_COMBINATOR_DESCENDANT; // ">>"
    else
        selector->combinator = MyCSS_SELECTORS_COMBINATOR_CHILD;      // ">"
}

void mycss_selectors_parser_selector_combinator_plus(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_NEXT_SIBLING;
}

void mycss_selectors_parser_selector_combinator_tilde(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_FOLLOWING_SIBLING;
}

void mycss_selectors_parser_selector_combinator_column(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_COLUMN;
}

void mycss_selectors_parser_selector_combinator_whitespace(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_DESCENDANT;
}

/////////////////////////////////////////////////////////
//// Selector type
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_ident_type(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector = mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_ELEMENT;
    selector->key  = str;
}

void mycss_selectors_parser_selector_ident_attr(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector = mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_ATTRIBUTE;
    selector->key  = str;
}

void mycss_selectors_parser_selector_id(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector = mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_ID;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

void mycss_selectors_parser_selector_class(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector = mycss_selectors_parser_selector_create_new_entry(result, selectors, selector);
    
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
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
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->key = str;
}

/////////////////////////////////////////////////////////
//// Value and Modifier
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_value(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->value = str;
}

void mycss_selectors_parser_selector_modifier(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->mod = MyCSS_SELECTORS_MOD_I;
}

/////////////////////////////////////////////////////////
//// Pseudo and Function
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_pseudo_class(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->type = MyCSS_SELECTORS_TYPE_PSEUDO_CLASS;
}

void mycss_selectors_parser_selector_function(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->type = MyCSS_SELECTORS_TYPE_FUNCTION;
}

void mycss_selectors_parser_selector_function_end(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    
}

/////////////////////////////////////////////////////////
//// End and bad selectors
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_end(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_GOOD;
}

void mycss_selectors_parser_expectations_error(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector == NULL)
        return;
    
    if(selector->key) {
        myhtml_string_destroy(selector->key, 0);
        mcobject_async_free(result->entry->mcasync_string, selector->key);
    }
    
    if(selector->value) {
        myhtml_string_destroy(selector->value, 0);
        mcobject_async_free(result->entry->mcasync_string, selector->value);
    }
    
    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    
    //printf("Expectations error!\n");
}


