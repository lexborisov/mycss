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


void mycss_selectors_parser_selector_combinator(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
}

void mycss_selectors_parser_selector_ident(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_ELEMENT;
    selector->key  = str;
}

void mycss_selectors_parser_selector_namespace(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->ns = selector->key;
    selector->key = NULL;
}

void mycss_selectors_parser_selector_key(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_ATTRIBUTE;
    selector->key  = str;
}

void mycss_selectors_parser_selector_id(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_ID;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

void mycss_selectors_parser_selector_class(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->type = MyCSS_SELECTORS_TYPE_CLASS;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(result, selectors, selector, token);
}

void mycss_selectors_parser_selector_matcher(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
}

void mycss_selectors_parser_selector_value(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str);
    
    selector->value = str;
}

void mycss_selectors_parser_selector_end(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selectors->selector = mcobject_async_malloc(result->entry->mcasync_selectors_entries, result->selectors_entries_id, NULL);
    
    selector->next = selectors->selector;
    selectors->selector->prev = selector;
}

void mycss_selectors_parser_selector_modifier(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->mod = MyCSS_SELECTORS_MOD_I;
}

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

void mycss_selectors_parser_expectations_error(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    //printf("Expectations error!\n");
}


