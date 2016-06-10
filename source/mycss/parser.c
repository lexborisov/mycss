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
    
    while(entry->result->parser(entry->result, token) == false) {};
    
    return entry->token;
}

bool mycss_parser_token(mycss_result_t* result, mycss_token_t* token)
{
    switch (token->type) {
    /* Selectors */
        case MyCSS_TOKEN_TYPE_IDENT: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_ident");
            
            mycss_selectors_parser_selector_ident_type(result, result->selectors, result->selectors->selector, token);
            result->selectors->state = mycss_selectors_state_simple_selector_ident;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_COMMA: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_comma");
            
            mycss_selectors_parser_selector_comma(result, result->selectors, result->selectors->selector, token);
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
            /* combinator */
                case '+': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_plus");
                    
                    mycss_selectors_parser_selector_combinator_plus(result, result->selectors, result->selectors->selector, token);
                    return true;
                }
                case '>': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_greater_than");
                    
                    mycss_selectors_parser_selector_combinator_greater_than(result, result->selectors, result->selectors->selector, token);
                    result->selectors->state = mycss_selectors_state_combinator_greater_than;
                    
                    if(result->parser != mycss_selectors_state_token_skip_whitespace)
                        result->parser = mycss_selectors_state_token_skip_whitespace;
                    
                    break;
                }
                case '~': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_tilde");
                    
                    mycss_selectors_parser_selector_combinator_tilde(result, result->selectors, result->selectors->selector, token);
                    return true;
                }
                
            /* simple-selector */
                case '*': {
                    MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_asterisk");
                    
                    mycss_selectors_parser_selector_ident_type(result, result->selectors, result->selectors->selector, token);
                    mycss_selectors_parser_selector_end(result, result->selectors, result->selectors->selector, token);
                    
                    return true;
                }
                case '.': {
                    result->selectors->state = mycss_selectors_state_simple_selector_full_stop;
                    break;
                }
                case '|': {
                    mycss_selectors_parser_selector_ident_type(result, result->selectors, result->selectors->selector, token);
                    mycss_selectors_parser_selector_namespace(result, result->selectors, result->selectors->selector, token);
                    result->selectors->state = mycss_selectors_state_simple_selector_vertical_bar;
                    break;
                }
                default: {
                    // parse error
                    mycss_selectors_parser_bad_token(result, result->selectors, result->selectors->selector, token);
                    return true;
                }
            }
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            
            break;
        }
        /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_combinator_column");
            
            mycss_selectors_parser_selector_combinator_column(result, result->selectors, result->selectors->selector, token);
            
            result->parser = mycss_parser_token;
            break;
        }
        case MyCSS_TOKEN_TYPE_WHITESPACE: {
            if(result->selectors->selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
                mycss_selectors_parser_selector_create_new_entry(result, result->selectors, result->selectors->selector);
            
            if(result->selectors->selector->combinator == MyCSS_SELECTORS_COMBINATOR_UNDEF && result->selectors->selector->prev) {
                mycss_selectors_parser_selector_combinator_whitespace(result, result->selectors, result->selectors->selector, token);
            }
            
            break;
        }
        case MyCSS_TOKEN_TYPE_COLON: {
            result->selectors->state = mycss_selectors_state_simple_selector_colon;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_LEFT_SQUARE_BRACKET: {
            result->selectors->state = mycss_selectors_state_simple_selector_left_bracket;
            
            if(result->parser != mycss_selectors_state_token_skip_whitespace)
                result->parser = mycss_selectors_state_token_skip_whitespace;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_HASH: {
            MyCSS_DEBUG_MESSAGE("mycss_selectors_state_simple_selector_hash");
            
            mycss_selectors_parser_selector_id(result, result->selectors, result->selectors->selector, token);
            break;
        }
    /* Namespace and Media */
        case MyCSS_TOKEN_TYPE_AT_KEYWORD: {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            if(myhtml_strncasecmp(str.data, "namespace", 9) == 0) {
                mycss_namespace_parser_begin(result, result->ns, result->ns->ns_entry, token);
                result->ns->state = mycss_namespace_state_namespace_namespace;
                
                if(result->parser != mycss_namespace_state_token_skip_whitespace)
                    result->parser = mycss_namespace_state_token_skip_whitespace;
            }
            else if(myhtml_strncasecmp(str.data, "media", 5) == 0) {
                result->ns->state = mycss_namespace_state_namespace_namespace;
                
                if(result->parser != mycss_namespace_state_token_skip_whitespace)
                    result->parser = mycss_namespace_state_token_skip_whitespace;
            }
            else {
                // parse error
                myhtml_string_destroy(&str, false);
                mycss_selectors_parser_bad_token(result, result->selectors, result->selectors->selector, token);
                
                return true;
            }
            
            myhtml_string_destroy(&str, false);
            break;
        }
    /* Rules */
        case MyCSS_TOKEN_TYPE_LEFT_CURLY_BRACKET: {
            result->rules->state = mycss_rules_state_body;
            result->selectors->selector = NULL;
            
            if(result->parser != mycss_rules_state_token_skip_whitespace)
                result->parser = mycss_rules_state_token_skip_whitespace;
            
            break;
        }
        default: {
            // parse error
            mycss_selectors_parser_bad_token(result, result->selectors, result->selectors->selector, token);
            break;
        }
    }
    
    return true;
}


