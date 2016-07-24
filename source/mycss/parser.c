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

#include "mycss/parser.h"
#include "mycss/tokenizer_resource.h"

mycss_token_t * mycss_parser_token_ready_callback_function(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_COMMENT)
        return token;
    
    while(entry->parser(entry, token) == false) {};
    
    return entry->token;
}

bool mycss_parser_token(mycss_entry_t* entry, mycss_token_t* token)
{
    mycss_selectors_list_t *selectors_list = entry->stylesheet->sel_list_last;
    
    switch (token->type) {
    /* Selectors */
        case MyCSS_TOKEN_TYPE_IDENT: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_ident");
            
            mycss_selectors_parser_selector_ident_type(entry, entry->selectors, selectors_list->selector, token);
            entry->parser_state = mycss_selectors_state_simple_selector_ident;
            
            if(entry->parser != mycss_selectors_state_token_all)
                entry->parser = mycss_selectors_state_token_all;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_COMMA: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_comma");
            
            mycss_selectors_parser_selector_comma(entry, entry->selectors, selectors_list->selector, token);
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
            /* combinator */
                case '+': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_plus");
                    
                    mycss_selectors_parser_selector_combinator_plus(entry, entry->selectors, selectors_list->selector, token);
                    return true;
                }
                case '>': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_greater_than");
                    
                    mycss_selectors_parser_selector_combinator_greater_than(entry, entry->selectors, selectors_list->selector, token);
                    entry->parser_state = mycss_selectors_state_combinator_greater_than;
                    
                    if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                        entry->parser = mycss_selectors_state_token_skip_whitespace;
                    
                    break;
                }
                case '~': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_tilde");
                    
                    mycss_selectors_parser_selector_combinator_tilde(entry, entry->selectors, selectors_list->selector, token);
                    return true;
                }
                
            /* simple-selector */
                case '*': {
                    mycss_selectors_parser_selector_ident_type(entry, entry->selectors, selectors_list->selector, token);
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_asterisk")
                    
                    entry->parser_state = mycss_selectors_state_simple_selector_ident;
                    break;
                }
                case '.': {
                    entry->parser_state = mycss_selectors_state_simple_selector_full_stop;
                    break;
                }
                case '|': {
                    mycss_selectors_parser_selector_namespace_ident(entry, entry->selectors, selectors_list->selector, token);
                    entry->parser_state = mycss_selectors_state_simple_selector_vertical_bar;
                    break;
                }
                default: {
                    // parse error
                    mycss_selectors_parser_bad_token(entry, entry->selectors, selectors_list->selector, token);
                    return true;
                }
            }
            
            if(entry->parser != mycss_selectors_state_token_all)
                entry->parser = mycss_selectors_state_token_all;
            
            break;
        }
        /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_column");
            
            mycss_selectors_parser_selector_combinator_column(entry, entry->selectors, selectors_list->selector, token);
            
            entry->parser = mycss_parser_token;
            break;
        }
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            if(selectors_list->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
                mycss_selectors_parser_selector_create_new_entry(entry, entry->selectors, selectors_list->selector);
            
            if(selectors_list->selector->combinator == MyCSS_SELECTORS_COMBINATOR_UNDEF && selectors_list->selector->prev) {
                mycss_selectors_parser_selector_combinator_whitespace(entry, entry->selectors, selectors_list->selector, token);
            }
            
            break;
        }
        case MyCSS_TOKEN_TYPE_COLON: {
            entry->parser_state = mycss_selectors_state_simple_selector_colon;
            
            if(entry->parser != mycss_selectors_state_token_all)
                entry->parser = mycss_selectors_state_token_all;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_LEFT_SQUARE_BRACKET: {
            entry->parser_state = mycss_selectors_state_simple_selector_left_bracket;
            
            if(entry->parser != mycss_selectors_state_token_skip_whitespace)
                entry->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_HASH: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_hash");
            
            mycss_selectors_parser_selector_id(entry, entry->selectors, selectors_list->selector, token);
            break;
        }
    /* Namespace and Media */
        case MyCSS_TOKEN_TYPE_AT_KEYWORD: {
            myhtml_string_t str;
            mycss_token_data_to_string(entry, token, &str, true, true);
            
            if(myhtml_strcmp(str.data, "namespace") == 0) {
                mycss_namespace_parser_begin(entry, entry->ns, entry->ns->ns_entry, token);
                entry->parser_state = mycss_namespace_state_namespace_namespace;
                
                if(entry->parser != mycss_namespace_state_token_skip_whitespace)
                    entry->parser = mycss_namespace_state_token_skip_whitespace;
            }
            else if(myhtml_strcmp(str.data, "media") == 0) {
                entry->parser_state = mycss_media_state_skep_all;
                
                if(entry->parser != mycss_media_state_token_skip_whitespace)
                    entry->parser = mycss_media_state_token_skip_whitespace;
            }
            else {
                // parse error
                myhtml_string_destroy(&str, false);
                mycss_selectors_parser_bad_token(entry, entry->selectors, selectors_list->selector, token);
                
                return true;
            }
            
            myhtml_string_destroy(&str, false);
            break;
        }
    /* Rules */
        case MyCSS_TOKEN_TYPE_LEFT_CURLY_BRACKET: {
            mycss_selectors_end(selectors_list, entry->selectors, true);
            
            entry->parser_state = mycss_rules_state_body;
            
            if(entry->parser != mycss_rules_state_token_skip_whitespace)
                entry->parser = mycss_rules_state_token_skip_whitespace;
            
            break;
        }
        default: {
            // parse error
            mycss_selectors_parser_bad_token(entry, entry->selectors, selectors_list->selector, token);
            break;
        }
    }
    
    return true;
}


