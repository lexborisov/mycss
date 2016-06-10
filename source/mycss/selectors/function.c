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

#include "mycss/selectors/function.h"
#include "mycss/selectors/function_resource.h"
#include "myhtml/utils/resources.h"

const mycss_selectots_function_begin_entry_t * mycss_function_begin_entry_by_name(const char* name, size_t length)
{
    size_t idx = ((myhtml_string_chars_lowercase_map[ (const unsigned char)name[0] ] *
                   myhtml_string_chars_lowercase_map[ (const unsigned char)name[(length - 1)] ] *
                   length)
                  % MyCSS_SELECTORS_FUNCTION_NAME_STATIC_SIZE) + 1;
    
    while (mycss_selectors_function_begin_map_index[idx].name)
    {
        if(mycss_selectors_function_begin_map_index[idx].length == length) {
            if(myhtml_strncasecmp(mycss_selectors_function_begin_map_index[idx].name, name, length) == 0)
                return &mycss_selectors_function_begin_map_index[idx];
            
            if(mycss_selectors_function_begin_map_index[idx].next)
                idx = mycss_selectors_function_begin_map_index[idx].next;
            else
                return NULL;
        }
        else if(mycss_selectors_function_begin_map_index[idx].length > length) {
            return NULL;
        }
        else {
            idx = mycss_selectors_function_begin_map_index[idx].next;
        }
    }
    
    return NULL;
}

mycss_selectors_function_begin_f mycss_function_begin_by_name(const char *name, size_t length)
{
    const mycss_selectots_function_begin_entry_t *entry = mycss_function_begin_entry_by_name(name, length);
    
    if(entry) {
        return entry->func;
    }
    
    return NULL;
}

void mycss_selectors_function_begin_nth_child(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_CHILD;
}

void mycss_selectors_function_begin_has(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_HAS;
    
    mycss_result_entry_t *res_entry = mycss_selectors_value_function_create(result, true);
    selector->value = res_entry;
    
    res_entry->parent = result->result_entry;
    result->result_entry = res_entry;
    
    result->selectors->selector = mycss_selectors_entry_create(result->selectors);
    mycss_result_entry_append_selector(result, result->result_entry, result->selectors->selector);
    
    /* switch to function not parser find simple selector */
    result->selectors->switch_parser = mycss_selectors_function_has_parser;
    result->selectors->state = mycss_selectors_state_simple_selector;
}

void mycss_selectors_function_begin_nth_last_child(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_CHILD;
}

void mycss_selectors_function_begin_drop(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_DROP;
}

void mycss_selectors_function_begin_nth_last_of_type(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_OF_TYPE;
}

void mycss_selectors_function_begin_not(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NOT;
    
    mycss_result_entry_t *res_entry = mycss_selectors_value_function_create(result, true);
    selector->value = res_entry;
    
    res_entry->parent = result->result_entry;
    result->result_entry = res_entry;
    
    result->selectors->selector = mycss_selectors_entry_create(result->selectors);
    mycss_result_entry_append_selector(result, result->result_entry, result->selectors->selector);
    
    /* switch to function not parser find simple selector */
    result->selectors->switch_parser = mycss_selectors_function_not_or_matches_parser;
    result->selectors->state = mycss_selectors_state_simple_selector;
}

void mycss_selectors_function_begin_current(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_CURRENT;
    mycss_selectors_function_begin_not(result, selector);
}

void mycss_selectors_function_begin_nth_of_type(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_OF_TYPE;
}

void mycss_selectors_function_begin_nth_last_column(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_COLUMN;
}

void mycss_selectors_function_begin_dir(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_DIR;
}

void mycss_selectors_function_begin_matches(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_MATCHES;
    mycss_selectors_function_begin_not(result, selector);
}

void mycss_selectors_function_begin_nth_column(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_COLUMN;
}

void mycss_selectors_function_begin_lang(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_LANG;
}

/* Unknown, for not exists function */
void mycss_selectors_begin_unknown(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_UNKNOWN;
    
    mycss_result_entry_t *res_entry = mycss_selectors_value_function_create(result, true);
    selector->value = res_entry;
    
    res_entry->parent = result->result_entry;
    result->result_entry = res_entry;
    
    result->selectors->selector = mycss_selectors_entry_create(result->selectors);
    mycss_result_entry_append_selector(result, result->result_entry, result->selectors->selector);
    
    result->selectors->switch_parser = mycss_selectors_unknown_parser;
    result->selectors->state = mycss_selectors_state_function_skip_all;
}

bool mycss_selectors_unknown_parser(mycss_result_t* result, mycss_token_t* token)
{
    mycss_selectors_t *selectors = result->selectors;
    
    mycss_selectors_end(selectors);
    
    result->result_entry = result->result_entry->parent;
    selectors->selector = result->result_entry->selector_list[ (result->result_entry->selector_list_length - 1) ];
    
    while(selectors->selector->next)
        selectors->selector = selectors->selector->next;
    
    mycss_selectors_parser_selector_function_end(result, result->selectors, result->selectors->selector, token);
    
    if(result->result_entry->parent == NULL) {
        result->parser = mycss_parser_token;
        result->selectors->switch_parser = mycss_parser_token;
    }
    
    return true;
}

/* ::not or ::matches */
bool mycss_selectors_function_not_or_matches_parser(mycss_result_t* result, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_COMMA: {
            mycss_selectors_parser_selector_comma(result, result->selectors, result->selectors->selector, token);
            
            result->selectors->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            mycss_selectors_t *selectors = result->selectors;
            
            mycss_selectors_end(selectors);
            
            result->result_entry = result->result_entry->parent;
            selectors->selector = result->result_entry->selector_list[ (result->result_entry->selector_list_length - 1) ];
            
            while(selectors->selector->next)
                selectors->selector = selectors->selector->next;
            
            mycss_selectors_parser_selector_function_end(result, result->selectors, result->selectors->selector, token);
            
            if(result->result_entry->parent) {
                result->selectors->state = mycss_selectors_state_simple_selector;
                
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
            }
            else {
                result->parser = mycss_parser_token;
                result->selectors->switch_parser = mycss_parser_token;
            }
            
            break;
        }
            
        default: {
            if(result->selectors->state == mycss_selectors_state_simple_selector) {
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
                
                break;
            }
            
            result->selectors->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

/* ::has */
bool mycss_selectors_function_has_parser(mycss_result_t* result, mycss_token_t* token)
{
    switch (token->type) {
            /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            mycss_selectors_parser_selector_combinator_column(result, result->selectors, result->selectors->selector, token);
            
            result->selectors->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
                    /* combinator */
                case '+': {
                    mycss_selectors_parser_selector_combinator_plus(result, result->selectors, result->selectors->selector, token);
                    break;
                }
                case '>': {
                    mycss_selectors_parser_selector_combinator_greater_than(result, result->selectors, result->selectors->selector, token);
                    result->selectors->state = mycss_selectors_state_combinator_greater_than;
                    
                    if(result->parser != mycss_selectors_state_token_all)
                        result->parser = mycss_selectors_state_token_all;
                    
                    return true;
                }
                case '~': {
                    mycss_selectors_parser_selector_combinator_tilde(result, result->selectors, result->selectors->selector, token);
                    break;
                }
                default: {
                    // parse error
                    break;
                }
            }
            
            result->selectors->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_COMMA: {
            result->selectors->selector = NULL;
            result->selectors->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            mycss_selectors_t *selectors = result->selectors;
            
            mycss_selectors_end(selectors);
            
            result->result_entry = result->result_entry->parent;
            selectors->selector = result->result_entry->selector_list[ (result->result_entry->selector_list_length - 1) ];
            
            while(selectors->selector->next)
                selectors->selector = selectors->selector->next;
            
            mycss_selectors_parser_selector_function_end(result, result->selectors, result->selectors->selector, token);
            
            if(result->result_entry->parent) {
                result->selectors->state = mycss_selectors_state_simple_selector;
                
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
            }
            else
                result->parser = mycss_parser_token;
            
            break;
        }
            
        default: {
            if(result->selectors->state == mycss_selectors_state_simple_selector) {
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
                
                break;
            }
            
            result->selectors->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}


