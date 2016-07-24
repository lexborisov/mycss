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
void mycss_selectors_function_begin_nth_child(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_CHILD;
    
    mycss_an_plus_b_entry_t *anb_entry = mycss_selectors_value_pseudo_class_function_nth_child_create(entry, true);
    selector->value = anb_entry;
    
    entry->anb->anb_entry = anb_entry;
    
    entry->parser_switch = mycss_selectors_function_nth_with_selectors_before_of_parser;
    entry->parser_state = mycss_an_plus_b_state_anb;
    
    if(entry->parser != mycss_an_plus_b_state_token_skip_whitespace)
        entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_has(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_HAS;
    selector->value = mycss_selectors_list_create_next_level_of_selectors(entry->selectors, entry->stylesheet, entry->stylesheet->sel_list_last);
    
    /* switch to function not parser find simple selector */
    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
        entry->parser = mycss_selectors_state_token_skip_whitespace;
    
    entry->parser_original = mycss_selectors_function_has_parser;
    entry->parser_switch   = mycss_selectors_parser_selectors_list_with_combinator_first_process;
    entry->parser_state           = mycss_selectors_state_simple_selector;
}

void mycss_selectors_function_begin_nth_last_child(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_CHILD;
    
    mycss_an_plus_b_entry_t *anb_entry = mycss_selectors_value_pseudo_class_function_nth_child_create(entry, true);
    selector->value = anb_entry;
    
    entry->anb->anb_entry = anb_entry;
    
    entry->parser_switch = mycss_selectors_function_nth_with_selectors_before_of_parser;
    entry->parser_state = mycss_an_plus_b_state_anb;
    
    if(entry->parser != mycss_an_plus_b_state_token_skip_whitespace)
        entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_drop(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_DROP;
    selector->value    = NULL;
    
    if(entry->parser != mycss_selectors_function_drop_parser)
        entry->parser = mycss_selectors_function_drop_parser;
}

void mycss_selectors_function_begin_nth_last_of_type(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_OF_TYPE;
    
    mycss_an_plus_b_entry_t *anb_entry = mycss_selectors_value_pseudo_class_function_nth_child_create(entry, true);
    selector->value = anb_entry;
    
    entry->anb->anb_entry = anb_entry;
    
    entry->parser_switch = mycss_selectors_function_nth_without_selectors_parser;
    entry->parser_state = mycss_an_plus_b_state_anb;
    
    if(entry->parser != mycss_an_plus_b_state_token_skip_whitespace)
        entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_not(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NOT;
    selector->value = mycss_selectors_list_create_next_level_of_selectors(entry->selectors, entry->stylesheet, entry->stylesheet->sel_list_last);
    
    /* switch to function not parser find simple selector list */
    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
        entry->parser = mycss_selectors_state_token_skip_whitespace;
    
    entry->parser_original = mycss_selectors_function_not_or_matches_parser;
    entry->parser_switch   = mycss_selectors_parser_selectors_list_process;
    entry->parser_state    = mycss_selectors_state_simple_selector;
}

void mycss_selectors_function_begin_current(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_CURRENT;
    selector->value = mycss_selectors_list_create_next_level_of_selectors(entry->selectors, entry->stylesheet, entry->stylesheet->sel_list_last);
    
    /* switch to function not parser find simple selector list */
    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
        entry->parser = mycss_selectors_state_token_skip_whitespace;
    
    entry->parser_original = mycss_selectors_function_not_or_matches_parser;
    entry->parser_switch   = mycss_selectors_parser_selectors_list_process;
    entry->parser_state           = mycss_selectors_state_simple_selector;
}

void mycss_selectors_function_begin_nth_of_type(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_OF_TYPE;
    
    mycss_an_plus_b_entry_t *anb_entry = mycss_selectors_value_pseudo_class_function_nth_child_create(entry, true);
    selector->value = anb_entry;
    
    entry->anb->anb_entry = anb_entry;
    
    entry->parser_switch = mycss_selectors_function_nth_without_selectors_parser;
    entry->parser_state = mycss_an_plus_b_state_anb;
    
    if(entry->parser != mycss_an_plus_b_state_token_skip_whitespace)
        entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_nth_last_column(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_COLUMN;
    
    mycss_an_plus_b_entry_t *anb_entry = mycss_selectors_value_pseudo_class_function_nth_child_create(entry, true);
    selector->value = anb_entry;
    
    entry->anb->anb_entry = anb_entry;
    
    entry->parser_switch = mycss_selectors_function_nth_without_selectors_parser;
    entry->parser_state = mycss_an_plus_b_state_anb;
    
    if(entry->parser != mycss_an_plus_b_state_token_skip_whitespace)
        entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_dir(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_DIR;
    selector->value    = NULL;
    
    if(entry->parser != mycss_selectors_function_dir_parser)
        entry->parser = mycss_selectors_function_dir_parser;
}

void mycss_selectors_function_begin_matches(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_MATCHES;
    selector->value = mycss_selectors_list_create_next_level_of_selectors(entry->selectors, entry->stylesheet, entry->stylesheet->sel_list_last);
    
    /* switch to function "not parser" find simple selector list */
    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
        entry->parser = mycss_selectors_state_token_skip_whitespace;
    
    entry->parser_original = mycss_selectors_function_not_or_matches_parser;
    entry->parser_switch   = mycss_selectors_parser_selectors_list_process;
    entry->parser_state           = mycss_selectors_state_simple_selector;
}

void mycss_selectors_function_begin_nth_column(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_COLUMN;
    
    mycss_an_plus_b_entry_t *anb_entry = mycss_selectors_value_pseudo_class_function_nth_child_create(entry, true);
    selector->value = anb_entry;
    
    entry->anb->anb_entry = anb_entry;
    
    entry->parser_switch = mycss_selectors_function_nth_without_selectors_parser;
    entry->parser_state = mycss_an_plus_b_state_anb;
    
    if(entry->parser != mycss_an_plus_b_state_token_skip_whitespace)
        entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
}

void mycss_selectors_function_begin_lang(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_LANG;
    selector->value    = NULL;
    
    if(entry->parser != mycss_selectors_function_lang_parser)
        entry->parser = mycss_selectors_function_lang_parser;
}

/////////////////////////////////////////////////////////
//// Unknown, for not exists function
////
/////////////////////////////////////////////////////////
void mycss_selectors_begin_unknown(mycss_entry_t* entry, mycss_selectors_entry_t* selector)
{
    selector->sub_type = MyCSS_SELECTORS_SUB_TYPE_UNKNOWN;
    
    entry->parser = mycss_selectors_unknown_parser;
}

bool mycss_selectors_unknown_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS)
    {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// NOT AND MATCHES AND CURRENT
////
/////////////////////////////////////////////////////////
void mycss_selectors_function_find_not_selector(mycss_selectors_list_t* selectors_list)
{
    for(size_t i = 0; i < selectors_list->selector_list_length; i++) {
        mycss_selectors_entry_t* selector = selectors_list->selector_list[i];
        
        while(selector) {
            if(selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_MATCHES ||
               selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NOT ||
               selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_CURRENT ||
               selector->type == MyCSS_SELECTORS_TYPE_PSEUDO_ELEMENT)
            {
                selector = selectors_list->parent->selector;
                
                if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                return;
            }
            
            selector = selector->next;
        }
    }
}

bool mycss_selectors_function_not_or_matches_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    mycss_selectors_t *selectors = entry->selectors;
    mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
    
    mycss_selectors_list_t *selectors_list = entry->stylesheet->sel_list_last;
    mycss_selectors_function_find_not_selector(selectors_list);
    
    entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, selectors_list);
    
    if(selectors_list->selector_list_length == 0) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        mycss_selectors_list_destroy(entry->selectors, selector->value, true);
        selector->value = NULL;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    }
    
    mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    return true;
}

/////////////////////////////////////////////////////////
//// HAS
////
/////////////////////////////////////////////////////////
void mycss_selectors_function_find_has_selector(mycss_selectors_list_t* selectors_list)
{
    for(size_t i = 0; i < selectors_list->selector_list_length; i++) {
        mycss_selectors_entry_t* selector = selectors_list->selector_list[i];
        
        while(selector) {
            if(selector->type == MyCSS_SELECTORS_TYPE_PSEUDO_ELEMENT)
            {
                selector = selectors_list->parent->selector;
                
                if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                return;
            }
            
            selector = selector->next;
        }
    }
}

bool mycss_selectors_function_has_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    mycss_selectors_t *selectors = entry->selectors;
    mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
    
    mycss_selectors_list_t *selectors_list = entry->stylesheet->sel_list_last;
    mycss_selectors_function_find_has_selector(selectors_list);
    
    entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, selectors_list);
    
    if(selectors_list->selector_list_length == 0) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        mycss_selectors_list_destroy(entry->selectors, selector->value, true);
        selector->value = NULL;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    }
    
    mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    return true;
}

/////////////////////////////////////////////////////////
//// NTH OF SELECTORS
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_nth_with_selectors_skip_all_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        selector->value = mycss_selectors_value_pseudo_class_function_nth_child_destroy(entry, selector->value, true);
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    
    return true;
}

bool mycss_selectors_function_nth_with_selectors_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->value) {
            mycss_an_plus_b_entry_t *anb = mycss_selector_value_an_plus_b(selector->value);
            
            if(anb->is_broken || (anb->of && anb->of->selector_list_length == 0) ||
               selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
            {
                if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                selector->value = mycss_selectors_value_pseudo_class_function_nth_child_destroy(entry, selector->value, true);
            }
        }
        
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        /* parse error */
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_nth_with_selectors_skip_all_parser;
    }
    
    return true;
}

bool mycss_selectors_function_nth_with_selectors_before_of_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
           mycss_selector_value_an_plus_b(selector->value)->is_broken)
        {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser_state = mycss_an_plus_b_state_skip_all;
            entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
            
            selector->value = mycss_selectors_value_pseudo_class_function_nth_child_destroy(entry, selector->value, true);
            return true;
        }
        
        myhtml_string_t str;
        mycss_token_data_to_string(entry, token, &str, true, false);
        
        if(myhtml_strcasecmp(str.data, "of") != 0) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser_state = mycss_an_plus_b_state_skip_all;
            entry->parser = mycss_an_plus_b_state_token_skip_whitespace;
            
            selector->value = mycss_selectors_value_pseudo_class_function_nth_child_destroy(entry, selector->value, true);
            
            myhtml_string_destroy(&str, false);
            return true;
        }
        
        myhtml_string_destroy(&str, false);
        
        /* create and switch entry, and create selector */
        mycss_selector_value_an_plus_b(selector->value)->of = mycss_selectors_list_create_next_level_of_selectors(entry->selectors, entry->stylesheet, entry->stylesheet->sel_list_last);
        
        /* switch to parse selector */
        entry->parser_switch = mycss_selectors_function_nth_of_selectors_parser;
        entry->parser_state = mycss_selectors_state_simple_selector;
        
        if(entry->parser != mycss_selectors_state_token_skip_whitespace)
            entry->parser = mycss_selectors_state_token_skip_whitespace;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->value) {
            mycss_an_plus_b_entry_t *anb = mycss_selector_value_an_plus_b(selector->value);
            
            if(anb->is_broken) {
                if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                selector->value = mycss_selectors_value_pseudo_class_function_nth_child_destroy(entry, selector->value, true);
            }
        }
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_nth_with_selectors_skip_all_parser;
    }
    
    return true;
}

bool mycss_selectors_function_nth_of_selectors_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        entry->parser = mycss_selectors_function_nth_of_whitespace_selectors_parser;
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS)
    {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        if(selector->value) {
            mycss_an_plus_b_entry_t *anb = mycss_selector_value_an_plus_b(selector->value);
            
            if(anb->is_broken || anb->of->selector_list_length == 0) {
                if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                selector->value = mycss_selectors_value_pseudo_class_function_nth_child_destroy(entry, selector->value, true);
            }
        }
        
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
           entry->parser_state == mycss_selectors_state_simple_selector)
        {
            entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
            
            selector = entry->stylesheet->sel_list_last->selector;
            
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser = mycss_selectors_function_nth_with_selectors_skip_all_parser;
            return true;
        }
        
        entry->parser_state = mycss_selectors_state_simple_selector;
        
        if(entry->parser != mycss_selectors_state_token_all)
            entry->parser = mycss_selectors_state_token_all;
        
        return false;
    }
    
    return true;
}

bool mycss_selectors_function_nth_of_whitespace_selectors_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_nth_with_selectors_skip_all_parser;
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// NTH
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_nth_without_selectors_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        if(selector->value) {
            mycss_an_plus_b_entry_t *anb = mycss_selector_value_an_plus_b(selector->value);
            
            if(anb->is_broken) {
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                selector->value = mycss_selectors_value_pseudo_class_function_nth_last_column_destroy(entry, selector->value, true);
            }
        }
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0) {
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            selector->value = mycss_selectors_value_pseudo_class_function_nth_last_column_destroy(entry, selector->value, true);
        }
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// DROP
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_drop_skip_all_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        if(selector->value == NULL) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        }
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    
    return true;
}

bool mycss_selectors_function_drop_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        mycss_selectors_function_drop_type_t drop_val = mycss_selector_value_drop(selector->value);
        
        myhtml_string_t str;
        mycss_token_data_to_string(entry, token, &str, true, false);
        
        if(myhtml_strcasecmp(str.data ,"active") == 0) {
            drop_val |= MyCSS_SELECTORS_FUNCTION_DROP_TYPE_ACTIVE;
        }
        else if(myhtml_strcasecmp(str.data ,"valid") == 0) {
            drop_val |= MyCSS_SELECTORS_FUNCTION_DROP_TYPE_VALID;
        }
        else if(myhtml_strcasecmp(str.data ,"invalid") == 0) {
            drop_val |= MyCSS_SELECTORS_FUNCTION_DROP_TYPE_INVALID;
        }
        else {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        }
        
        selector->value = (void*)drop_val;
        
        myhtml_string_destroy(&str, false);
        
        entry->parser = mycss_selectors_function_drop_column_parser;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        if(selector->value == NULL) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        }
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_drop_skip_all_parser;
        return false;
    }
    
    return true;
}

bool mycss_selectors_function_drop_column_wait_ident_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        entry->parser = mycss_selectors_function_drop_parser;
        return false;
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_drop_skip_all_parser;
        return false;
    }
}

bool mycss_selectors_function_drop_column_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_COLUMN) {
        entry->parser = mycss_selectors_function_drop_column_wait_ident_parser;
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        entry->parser = mycss_selectors_function_drop_parser;
        return false;
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_drop_skip_all_parser;
        return false;
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// DIR
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_dir_wait_parenthesis_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->value == NULL) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        }
        
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    }
    
    return true;
}

bool mycss_selectors_function_dir_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT) {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->value) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            return true;
        }
        
        selector->value = mycss_selectors_value_pseudo_class_function_dir_create(entry, true);
        mycss_token_data_to_string(entry, token, selector->value, true, false);
        
        entry->parser = mycss_selectors_function_dir_wait_parenthesis_parser;
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_dir_wait_parenthesis_parser;
        return false;
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// LANG
////
/////////////////////////////////////////////////////////
bool mycss_selectors_function_lang_skip_all_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    
    return true;
}

bool mycss_selectors_function_lang_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT ||
            token->type == MyCSS_TOKEN_TYPE_STRING)
    {
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        mycss_selectors_value_lang_t *lang;
        
        if(selector->value) {
            lang = mycss_selector_value_lang(selector->value);
            
            while(lang->next)
                lang = lang->next;
            
            lang->next = mycss_selectors_value_pseudo_class_function_lang_create(entry, true);
            mycss_token_data_to_string(entry, token, &lang->next->str, true, false);
        }
        else {
            lang = mycss_selectors_value_pseudo_class_function_lang_create(entry, true);
            mycss_token_data_to_string(entry, token, &lang->str, true, false);
            
            selector->value = lang;
        }
        
        entry->parser = mycss_selectors_function_lang_after_parser;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        mycss_selectors_t *selectors = entry->selectors;
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if(selector->value == NULL) {
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        }
        
        mycss_selectors_end(entry->stylesheet->sel_list_last, selectors, false);
        entry->stylesheet->sel_list_last = mycss_entry_get_parent_set_parser(entry, entry->stylesheet->sel_list_last);
        
        mycss_selectors_parser_selector_pseudo_class_function_end(entry, selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_lang_skip_all_parser;
    }
    
    return true;
}

bool mycss_selectors_function_lang_after_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_COMMA)
    {
        entry->parser = mycss_selectors_function_lang_comma_parser;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        entry->parser = mycss_selectors_function_lang_parser;
        return false;
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_lang_skip_all_parser;
    }
    
    return true;
}

bool mycss_selectors_function_lang_comma_parser(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_WHITESPACE) {
        return true;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_IDENT ||
            token->type == MyCSS_TOKEN_TYPE_STRING)
    {
        entry->parser = mycss_selectors_function_lang_parser;
        return false;
    }
    else {
        /* parse error */
        mycss_selectors_entry_t *selector = entry->stylesheet->sel_list_last->selector;
        
        if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        entry->parser = mycss_selectors_function_lang_skip_all_parser;
        return false;
    }
}


