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

mycss_selectors_entry_t * mycss_selectors_parser_selector_create_new_entry(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector)
{
    mycss_selectors_entry_t *new_selector = mycss_selectors_entry_create(selectors);
    
    if(selector) {
        selector->next = new_selector;
        new_selector->prev = selector;
    }
    
    entry->stylesheet->sel_list_last->selector = new_selector;
    return new_selector;
}

/////////////////////////////////////////////////////////
//// Comma (,)
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_comma(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_list_append_selector(selectors, entry->stylesheet->sel_list_last, selector);
    
    selector->prev->next = NULL;
    selector->prev = NULL;
}

/////////////////////////////////////////////////////////
//// Combinator
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_combinator_greater_than(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
    
    if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_CHILD)
        selector->combinator = MyCSS_SELECTORS_COMBINATOR_DESCENDANT; // ">>"
    else
        selector->combinator = MyCSS_SELECTORS_COMBINATOR_CHILD;      // ">"
}

void mycss_selectors_parser_selector_combinator_plus(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_NEXT_SIBLING;
}

void mycss_selectors_parser_selector_combinator_tilde(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_FOLLOWING_SIBLING;
}

void mycss_selectors_parser_selector_combinator_column(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_COLUMN;
}

void mycss_selectors_parser_selector_combinator_whitespace(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(selector->type & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
    
    selector->combinator = MyCSS_SELECTORS_COMBINATOR_DESCENDANT;
}

/////////////////////////////////////////////////////////
//// Selector type
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_ident_type(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    /* set default namespace */
    mycss_stylesheet_t* stylesheet = selectors->entry->stylesheet;
    selector->ns_entry = stylesheet->ns_stylesheet.entry_default;
    
    selector->type = MyCSS_SELECTORS_TYPE_ELEMENT;
    selector->key  = str;
}

void mycss_selectors_parser_selector_ident_attr(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    /* set default namespace */
    mycss_stylesheet_t* stylesheet = selectors->entry->stylesheet;
    selector->ns_entry = stylesheet->ns_stylesheet.entry_default;
    
    selector->type = MyCSS_SELECTORS_TYPE_ATTRIBUTE;
    selector->key  = str;
}

void mycss_selectors_parser_selector_id(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    /* set default namespace */
    mycss_stylesheet_t* stylesheet = selectors->entry->stylesheet;
    selector->ns_entry = stylesheet->ns_stylesheet.entry_default;
    
    selector->type = MyCSS_SELECTORS_TYPE_ID;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(entry, selectors, selector, token);
}

void mycss_selectors_parser_selector_class(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    /* set default namespace */
    mycss_stylesheet_t* stylesheet = selectors->entry->stylesheet;
    selector->ns_entry = stylesheet->ns_stylesheet.entry_default;
    
    selector->type = MyCSS_SELECTORS_TYPE_CLASS;
    selector->key  = str;
    
    mycss_selectors_parser_selector_end(entry, selectors, selector, token);
}

/////////////////////////////////////////////////////////
//// Namespace
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_namespace(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = selector->key;
    
    if(str == NULL || str->length == 0) {
        myhtml_string_destroy(str, 0);
        
        selector->key = NULL;
        selector->ns_entry = &entry->stylesheet->ns_stylesheet.entry_undef;
        
        return;
    }
    
    if(str->length == 1 && *str->data == '*') {
        myhtml_string_destroy(str, 0);
        
        selector->key = NULL;
        selector->ns_entry = &entry->stylesheet->ns_stylesheet.entry_any;
        
        return;
    }
    
    selector->ns_entry = mycss_namespace_entry_by_name(entry->ns, entry->stylesheet->ns_stylesheet.name_tree, str->data, str->length, false);
    
    if(selector->ns_entry == NULL) {
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        mycss_selectors_parser_check_and_set_bad_parent_selector(entry->stylesheet->sel_list_last);
    }
    
    myhtml_string_destroy(str, 0);
    selector->key = NULL;
}

void mycss_selectors_parser_selector_namespace_ident(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_parser_selector_namespace(entry, selectors, selector, token);
    
    if(selector->type == MyCSS_SELECTORS_TYPE_UNDEF)
        selector->type = MyCSS_SELECTORS_TYPE_ELEMENT;
}

void mycss_selectors_parser_selector_namespace_attr(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_parser_selector_namespace(entry, selectors, selector, token);
    
    if(selector->type == MyCSS_SELECTORS_TYPE_UNDEF)
        selector->type = MyCSS_SELECTORS_TYPE_ATTRIBUTE;
}

void mycss_selectors_parser_selector_after_namespace(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, true);
    
    selector->key = str;
}

/////////////////////////////////////////////////////////
//// Value and Modifier
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_value(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_object_attribute_t *attr = mycss_selectors_value_attribute_create(entry, 1);
    
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    attr->value = str;
    selector->value = attr;
}

void mycss_selectors_parser_selector_modifier(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selector_value_attribute(selector->value)->mod = MyCSS_SELECTORS_MOD_I;
}

/////////////////////////////////////////////////////////
//// Pseudo Class
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_pseudo_class(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, true);
    
    selector->key  = str;
    selector->type = MyCSS_SELECTORS_TYPE_PSEUDO_CLASS;
    
    selector->sub_type = mycss_pseudo_class_by_name(str->data, str->length);
    
    if(selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_UNKNOWN)
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    
    mycss_selectors_parser_check_and_set_bad_parent_selector(entry->stylesheet->sel_list_last);
    mycss_selectors_parser_selector_end(entry, selectors, selector, token);
}

void mycss_selectors_parser_selector_pseudo_class_function(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, true);
    
    selector->key   = str;
    selector->type  = MyCSS_SELECTORS_TYPE_PSEUDO_CLASS_FUNCTION;
    
    mycss_selectors_function_begin_f to_func = mycss_function_begin_by_name(str->data, str->length);
    
    if(to_func) {
        to_func(entry, selector);
    }
    else {
        /* skip and set bad type for current selector */
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        mycss_selectors_begin_unknown(entry, selector);
    }
}

void mycss_selectors_parser_selector_pseudo_class_function_end(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_parser_check_and_set_bad_parent_selector(entry->stylesheet->sel_list_last);
    mycss_selectors_parser_selector_end(entry, selectors, selector, token);
}

/////////////////////////////////////////////////////////
//// Pseudo Element
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_pseudo_element(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, true);
    
    selector->key  = str;
    selector->type = MyCSS_SELECTORS_TYPE_PSEUDO_ELEMENT;
    
    selector->sub_type = mycss_pseudo_element_by_name(str->data, str->length);
    
    if(selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_UNKNOWN)
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    
    mycss_selectors_parser_check_and_set_bad_parent_selector(entry->stylesheet->sel_list_last);
    mycss_selectors_parser_selector_end(entry, selectors, selector, token);
}

void mycss_selectors_parser_selector_pseudo_element_function(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, true);
    
    selector->key   = str;
    selector->type  = MyCSS_SELECTORS_TYPE_PSEUDO_ELEMENT_FUNCTION;
    
    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    mycss_selectors_begin_unknown(entry, selector);
}

void mycss_selectors_parser_selector_pseudo_element_function_end(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    mycss_selectors_parser_check_and_set_bad_parent_selector(entry->stylesheet->sel_list_last);
    mycss_selectors_parser_selector_end(entry, selectors, selector, token);
}

/////////////////////////////////////////////////////////
//// End and bad selectors
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_selector_end(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if(entry->callback_selector_done)
        entry->callback_selector_done(selectors, selector);
    
    mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
}

void mycss_selectors_parser_expectations_error(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    
    mycss_selectors_list_t *selectors_list = entry->stylesheet->sel_list_last;
    if(selectors_list->parent && selectors_list->parent->selector)
    {
        mycss_selectors_entry_t* parent_selector = selectors_list->parent->selector;
        
        if((parent_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
            parent_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
    }
    
    mycss_selectors_parser_selector_create_new_entry(entry, selectors, selector);
    
    //printf("Expectations error!\n");
}

void mycss_selectors_parser_bad_token(mycss_entry_t* entry, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token)
{
    if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0) {
        selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        
        myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
        selector->key = str;
        
        mycss_token_data_to_string(entry, token, selector->key, true, false);
        return;
    }
    
    mycss_token_data_to_string(entry, token, selector->key, false, false);
}

/////////////////////////////////////////////////////////
//// Set bad
////
/////////////////////////////////////////////////////////
void mycss_selectors_parser_check_and_set_bad_parent_selector(mycss_selectors_list_t* selectors_list)
{
    mycss_selectors_entry_t *selector = selectors_list->selector;
    
    if(selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
    {
        if(selectors_list->parent && selectors_list->parent->selector) {
            selector = selectors_list->parent->selector;
            
            if((selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
        }
    }
}

/////////////////////////////////////////////////////////
//// Selectors list
////
/////////////////////////////////////////////////////////
bool mycss_selectors_parser_selectors_list_begin(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            entry->parser_switch = mycss_selectors_parser_selectors_list_process;
            entry->parser_state = mycss_selectors_state_simple_selector;
            
            if(entry->parser != mycss_selectors_state_token_all)
                entry->parser = mycss_selectors_state_token_all;
            
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_process(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_COMMA: {
            if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
               entry->parser_state == mycss_selectors_state_simple_selector)
            {
                mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                
                if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                break;
            }
            
            mycss_selectors_parser_selector_comma(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser = mycss_selectors_parser_selectors_list_comma;
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            entry->parser = mycss_selectors_parser_selectors_list_whitespace;
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
               entry->parser_state == mycss_selectors_state_simple_selector)
            {
                mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                
                if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                break;
            }
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_comma(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
            
            if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            entry->parser_state = mycss_selectors_state_simple_selector;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_whitespace(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_COMMA: {
            mycss_selectors_parser_selector_comma(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser = mycss_selectors_parser_selectors_list_comma;
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
            
            if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser = mycss_selectors_parser_selectors_list_skip_all;
            break;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_skip_all(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        entry->parser = entry->parser_original;
        return false;
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// Selectors list with combinator firstly
////
/////////////////////////////////////////////////////////
bool mycss_selectors_parser_selectors_list_with_combinator_first_process(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
            /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            mycss_selectors_parser_selector_combinator_column(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
                    /* combinator */
                case '+': {
                    mycss_selectors_parser_selector_combinator_plus(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    break;
                }
                case '>': {
                    mycss_selectors_parser_selector_combinator_greater_than(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    
                    entry->parser = mycss_selectors_parser_selectors_list_with_combinator_first_greater_than;
                    return true;
                }
                case '~': {
                    mycss_selectors_parser_selector_combinator_tilde(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    break;
                }
                default: {
                    if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
                       entry->parser_state == mycss_selectors_state_simple_selector)
                    {
                        mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                        
                        if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                            prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                        
                        entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                        break;
                    }
                    
                    entry->parser_state = mycss_selectors_state_simple_selector;
                    
                    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                        entry->parser = mycss_selectors_state_token_skip_whitespace;
                    
                    return false;
                }
            }
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_COMMA: {
            if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
               entry->parser_state == mycss_selectors_state_simple_selector)
            {
                mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                
                if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                break;
            }
            
            mycss_selectors_parser_selector_comma(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser = mycss_selectors_parser_selectors_list_with_combinator_first_comma;
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            entry->parser = mycss_selectors_parser_selectors_list_with_combinator_first_whitespace;
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
               entry->parser_state == mycss_selectors_state_simple_selector)
            {
                mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                
                if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                break;
            }
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_with_combinator_first_greater_than(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_DELIM && *token->data == '>') {
        MyCSS_DEBUG_MESSAGE("mycss_selectors_parser_selectors_list_with_combinator_first_greater_than")
        
        mycss_selectors_parser_selector_combinator_greater_than(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
    }
    
    entry->parser_state = mycss_selectors_state_simple_selector;
    entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
    
    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
        entry->parser = mycss_selectors_state_token_skip_whitespace;
    
    return true;
}

bool mycss_selectors_parser_selectors_list_with_combinator_first_comma(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            mycss_selectors_parser_selector_combinator_column(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
                case '+': {
                    mycss_selectors_parser_selector_combinator_plus(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    break;
                }
                case '>': {
                    mycss_selectors_parser_selector_combinator_greater_than(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    
                    entry->parser = mycss_selectors_parser_selectors_list_with_combinator_first_greater_than;
                    return true;
                }
                case '~': {
                    mycss_selectors_parser_selector_combinator_tilde(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    break;
                }
                default: {
                    entry->parser_state = mycss_selectors_state_simple_selector;
                    
                    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                        entry->parser = mycss_selectors_state_token_skip_whitespace;
                    
                    return false;
                }
            }
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
            
            if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            entry->parser_state = mycss_selectors_state_simple_selector;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            return false;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_with_combinator_first_whitespace(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_COMMA: {
            mycss_selectors_parser_selector_comma(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser = mycss_selectors_parser_selectors_list_comma;
            break;
        }
            
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
            
        /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            mycss_selectors_parser_selector_combinator_column(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
                case '+': {
                    mycss_selectors_parser_selector_combinator_plus(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    break;
                }
                case '>': {
                    mycss_selectors_parser_selector_combinator_greater_than(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    
                    entry->parser = mycss_selectors_parser_selectors_list_with_combinator_first_greater_than;
                    return true;
                }
                case '~': {
                    mycss_selectors_parser_selector_combinator_tilde(entry, entry->selectors, entry->stylesheet->sel_list_last->selector, token);
                    break;
                }
                default: {
                    mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                    
                    if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                        prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                    
                    entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                    return true;
                }
            }
            
            entry->parser_state = mycss_selectors_state_simple_selector;
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
            
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            entry->parser = entry->parser_original;
            return false;
        }
            
        default: {
            mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
            
            if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            
            entry->parser = mycss_selectors_parser_selectors_list_skip_all;
            break;
        }
    }
    
    return true;
}

bool mycss_selectors_parser_selectors_list_with_combinator_first_after_combinator(mycss_entry_t* entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            break;
        }
        
        case MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS: {
            if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
               entry->parser_state == mycss_selectors_state_simple_selector)
            {
                mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                
                if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
            }
            
            entry->parser = entry->parser_original;
            return false;
        }
        
        default: {
            if(entry->stylesheet->sel_list_last->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD ||
               entry->parser_state == mycss_selectors_state_simple_selector)
            {
                mycss_selectors_entry_t *prev_selector = entry->stylesheet->sel_list_last->parent->selector;
                
                if((prev_selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD) == 0)
                    prev_selector->flags |= MyCSS_SELECTORS_FLAGS_SELECTOR_BAD;
                
                entry->parser = mycss_selectors_parser_selectors_list_skip_all;
                break;
            }
            
            entry->parser_switch = mycss_selectors_parser_selectors_list_with_combinator_first_process;
            entry->parser = mycss_selectors_parser_selectors_list_with_combinator_first_process;
            
            return false;
        }
    }
    
    return true;
}


