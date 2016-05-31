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
//    if(token->type == MyCSS_TOKEN_TYPE_COMMENT)
//        return token;
//    
//    while(entry->result->parser(entry->result, token) == false) {};
//    
//    entry->token = mcobject_async_malloc(entry->mcasync_token, entry->token_id, NULL);
//    entry->selectors->parser(entry, token);
//    
//    myhtml_string_t str;
//    mycss_token_data_to_string(entry, token, &str);
//    
//    //printf("%s\n", str.data);
//    if(token->type == MyCSS_TOKEN_TYPE_NUMBER)
//    {
//        double return_num;
//        mycss_convert_data_to_double(str.data, str.length, &return_num);
//        
//        //printf("Number %s: %f\n", str.data, return_num);
//    }
//    else if(token->type == MyCSS_TOKEN_TYPE_UNICODE_RANGE)
//    {
//        size_t start, end;
//        mycss_convert_unicode_range_to_codepoint(str.data, str.length, &start, &end);
//        
//        if(end)
//            //printf("Unicode range: %zu-%zu\n", start, end);
//        else
//            //printf("Unicode range: %zu\n", start);
//    }
    
//    myhtml_string_destroy(&str, false);
    
    return entry->token;
}

bool mycss_parser_token(mycss_result_t* result, mycss_token_t* token)
{
    switch (token->type) {
    /* Selectors */
        case MyCSS_TOKEN_TYPE_IDENT: {
            //printf("mycss_selectors_state_simple_selector_ident\n");  /* End of selector */
            
            result->selectors->state = mycss_selectors_state_simple_selector_ident;
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            switch(*token->data) {
            /* combinator */
                case '+': {
                    mycss_selectors_parser_selector_combinator(result, result->selectors, result->selectors->selector, token);
                    //printf("mycss_selectors_state_combinator_plus\n");  /* End of selector */
                    return true;
                }
                case '>': {
                    //printf("mycss_selectors_state_combinator_greater_than\n");  /* End of selector */
                    mycss_selectors_parser_selector_combinator(result, result->selectors, result->selectors->selector, token);
                    result->selectors->state = mycss_selectors_state_combinator_greater_than;
                    
                    if(result->parser != mycss_selectors_state_token_skip_whitespace)
                        result->parser = mycss_selectors_state_token_skip_whitespace;
                    
                    break;
                }
                case '~': {
                    mycss_selectors_parser_selector_combinator(result, result->selectors, result->selectors->selector, token);
                    //printf("mycss_selectors_state_combinator_tilde\n");  /* End of selector */
                    return true;
                }
                
            /* simple-selector */
                case '*': {
                    //printf("mycss_selectors_state_simple_selector_asterisk\n");  /* End of selector */
                    return true;
                }
                case '.': {
                    result->selectors->state = mycss_selectors_state_simple_selector_full_stop;
                    break;
                }
                case '|': {
                    result->selectors->state = mycss_selectors_state_simple_selector_vertical_bar;
                    break;
                }
                default: {
                    // parse error
                    return true;
                }
            }
            
            if(result->parser != mycss_selectors_state_token_all)
                result->parser = mycss_selectors_state_token_all;
            
            break;
        }
        /* combinator */
        case MyCSS_TOKEN_TYPE_COLUMN: {
            //printf("mycss_selectors_state_combinator_column\n");  /* End of selector */
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
            //printf("mycss_selectors_state_simple_selector_hash\n");  /* End of selector */
            
            break;
        }
    /* Namespace and Media */
        case MyCSS_TOKEN_TYPE_AT_KEYWORD: {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str);
            
            if(myhtml_strncasecmp(str.data, "namespace", 9) == 0) {
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
            }
            
            myhtml_string_destroy(&str, false);
            break;
        }
    /* Rules */
        case MyCSS_TOKEN_TYPE_LEFT_CURLY_BRACKET: {
            result->rules->state = mycss_rules_state_body;
                
            if(result->parser != mycss_rules_state_token_skip_whitespace)
                result->parser = mycss_rules_state_token_skip_whitespace;
            
            break;
        }
        default: {
            break;
        }
    }
    
    return true;
}


