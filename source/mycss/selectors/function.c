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

/////////////////////////////////////////////////////////
//// Functions for a find Begin Function
////
/////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////
//// Begin functions
////
/////////////////////////////////////////////////////////
void mycss_selectors_function_begin_nth_child(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_CHILD;
    
    mycss_an_plus_b_entry_t *res_entry = mycss_selectors_value_function_nth_child_create(result, true);
    selector->value = res_entry;
    
    result->anb->anb_entry = res_entry;
    
    result->anb->switch_parser = mycss_selectors_function_nth_parser;
    result->anb->state = mycss_an_plus_b_state_anb;
    
    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_has(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_FUNCTION_HAS;
    selector->value = mycss_result_entry_create_next_level_of_selectors(result, result->result_entry);
    
    /* switch to function not parser find simple selector */
    result->switch_parser = mycss_selectors_function_has_parser;
    result->state = mycss_selectors_state_simple_selector;
    
    if(result->parser != mycss_selectors_state_token_skip_whitespace)
        result->parser = mycss_selectors_state_token_skip_whitespace;
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
    selector->value = mycss_result_entry_create_next_level_of_selectors(result, result->result_entry);
    
    /* switch to function not parser find simple selector */
    result->switch_parser = mycss_selectors_function_not_or_matches_parser;
    result->state = mycss_selectors_state_simple_selector;
    
    if(result->parser != mycss_selectors_state_token_skip_whitespace)
        result->parser = mycss_selectors_state_token_skip_whitespace;
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

/////////////////////////////////////////////////////////
//// Unknown, for not exists function
////
/////////////////////////////////////////////////////////
void mycss_selectors_begin_unknown(mycss_result_t* result, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_UNKNOWN;
    
    result->switch_parser = mycss_selectors_unknown_parser;
    result->state = mycss_selectors_state_function_skip_all;
    
    if(result->parser != mycss_selectors_state_token_skip_whitespace)
        result->parser = mycss_selectors_state_token_skip_whitespace;
}

bool mycss_selectors_unknown_parser(mycss_result_t* result, mycss_token_t* token)
{
    mycss_selectors_parser_selector_function_end(result, result->selectors, result->result_entry->selector, token);
    
    if(result->result_entry->parent == NULL) {
        result->parser = mycss_parser_token;
        result->switch_parser = mycss_parser_token;
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// NOT AND MATCHES
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_not_or_matches_parser(mycss_result_t* result, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_COMMA: {
            mycss_selectors_parser_selector_comma(result, result->selectors, result->result_entry->selector, token);
            
            result->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            mycss_selectors_t *selectors = result->selectors;
            mycss_selectors_end(result->result_entry, selectors);
            
            result->result_entry = mycss_result_get_parent_set_parser(result, result->result_entry);
            mycss_selectors_parser_selector_function_end(result, result->selectors, result->result_entry->selector, token);
            
            break;
        }
            
        default: {
            if(result->state == mycss_selectors_state_simple_selector) {
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
                
                break;
            }
            
            result->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// HAS
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_has_parser(mycss_result_t* result, mycss_token_t* token)
{
    switch (token->type) {
            /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            mycss_selectors_parser_selector_combinator_column(result, result->selectors, result->result_entry->selector, token);
            
            result->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
                    /* combinator */
                case '+': {
                    mycss_selectors_parser_selector_combinator_plus(result, result->selectors, result->result_entry->selector, token);
                    break;
                }
                case '>': {
                    mycss_selectors_parser_selector_combinator_greater_than(result, result->selectors, result->result_entry->selector, token);
                    result->state = mycss_selectors_state_combinator_greater_than;
                    
                    if(result->parser != mycss_selectors_state_token_all)
                        result->parser = mycss_selectors_state_token_all;
                    
                    return true;
                }
                case '~': {
                    mycss_selectors_parser_selector_combinator_tilde(result, result->selectors, result->result_entry->selector, token);
                    break;
                }
                default: {
                    // parse error
                    break;
                }
            }
            
            result->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_COMMA: {
            result->result_entry->selector = NULL;
            result->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            mycss_selectors_t *selectors = result->selectors;
            mycss_selectors_end(result->result_entry, selectors);
            
            result->result_entry = mycss_result_get_parent_set_parser(result, result->result_entry);
            mycss_selectors_parser_selector_function_end(result, result->selectors, result->result_entry->selector, token);
            
            break;
        }
            
        default: {
            if(result->state == mycss_selectors_state_simple_selector) {
                if(result->parser != mycss_selectors_state_token_skip_whitespace)
                    result->parser = mycss_selectors_state_token_skip_whitespace;
                
                break;
            }
            
            result->state = mycss_selectors_state_simple_selector;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// NTH OF SELECTORS
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_nth_parser(mycss_result_t* result, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_entry_t *selector = result->result_entry->selector;
        
        if(selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
           mycss_selector_value_an_plus_b(selector->value)->is_broken)
        {
            result->anb->state = mycss_an_plus_b_state_skip_all;
            result->parser = mycss_an_plus_b_state_token_skip_whitespace;
            
            return true;
        }
        
        myhtml_string_t str;
        mycss_token_data_to_string(result->entry, token, &str, true);
        
        if(myhtml_strcasecmp(str.data, "of") != 0) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            result->anb->state = mycss_an_plus_b_state_skip_all;
            result->parser = mycss_an_plus_b_state_token_skip_whitespace;
            
            return true;
        }
        
        /* create and switch result, and create selector */
        mycss_selector_value_an_plus_b(selector->value)->of = mycss_result_entry_create_next_level_of_selectors(result, result->result_entry);
        
        /* switch to parse selector */
        result->switch_parser = mycss_selectors_function_nth_of_selectors_parser;
        result->state = mycss_selectors_state_simple_selector;
        
        if(result->parser != mycss_selectors_state_token_skip_whitespace)
            result->parser = mycss_selectors_state_token_skip_whitespace;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = result->selectors;
        mycss_selectors_end(result->result_entry, selectors);
        
        result->result_entry = mycss_result_get_parent_set_parser(result, result->result_entry);
        mycss_selectors_parser_selector_function_end(result, selectors, result->result_entry->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = result->result_entry->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    }
    
    return true;
}

bool mycss_selectors_function_nth_of_selectors_parser(mycss_result_t* result, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        mycss_selectors_entry_t *selector = result->result_entry->parent->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
        result->switch_parser = mycss_selectors_function_nth_of_selectors_parser;
        result->state = mycss_an_plus_b_state_skip_all;
        
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = result->selectors;
        mycss_selectors_end(result->result_entry, selectors);
        
        result->result_entry = mycss_result_get_parent_set_parser(result, result->result_entry);
        mycss_selectors_parser_selector_function_end(result, selectors, result->result_entry->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = result->result_entry->selector;
        
        if(selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        {
            result->parser = mycss_an_plus_b_state_token_skip_whitespace;
            result->switch_parser = mycss_selectors_function_nth_of_selectors_parser;
            result->state = mycss_an_plus_b_state_skip_all;
        }
        
        result->state = mycss_selectors_state_simple_selector;
        
        if(result->parser != mycss_selectors_state_token_all)
            result->parser = mycss_selectors_state_token_all;
    }
    
    return true;
}


