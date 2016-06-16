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

#include "mycss/an_plus_b.h"
#include "myhtml/utils/resources.h"

mycss_an_plus_b_t * mycss_an_plus_b_create(void)
{
    return (mycss_an_plus_b_t*)mycalloc(1, sizeof(mycss_an_plus_b_t));
}

mycss_status_t mycss_an_plus_b_init(mycss_entry_t* entry, mycss_an_plus_b_t* anb)
{
    anb->state = NULL;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_an_plus_b_clean_all(mycss_an_plus_b_t* anb)
{
    anb->state = NULL;
    
    return MyCSS_STATUS_OK;
}

mycss_an_plus_b_t * mycss_an_plus_b_destroy(mycss_an_plus_b_t* anb, bool self_destroy)
{
    if(anb == NULL)
        return NULL;
    
    if(self_destroy) {
        myfree(anb);
        return NULL;
    }
    
    return anb;
}

bool mycss_an_plus_b_state_token_all(mycss_result_t* result, mycss_token_t* token)
{
    mycss_an_plus_b_t *and = result->anb;
    return and->state(result, and, and->anb_entry, token);
}

bool mycss_an_plus_b_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token)
{
    if(token->type != MyCSS_TOKEN_TYPE_WHITESPACE) {
        mycss_an_plus_b_t *and = result->anb;
        return and->state(result, and, and->anb_entry, token);
    }
    
    return true;
}

/////////////////////////////////////////////////////////
//// An+B Parser
////
/////////////////////////////////////////////////////////
void mycss_an_plus_b_parser_expectations_error(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    anb_entry->is_broken = true;
}

/////////////////////////////////////////////////////////
//// An+B Print
////
/////////////////////////////////////////////////////////
void mycss_an_plus_b_print(mycss_an_plus_b_entry_t* anb_entry, FILE* fh)
{
    if(anb_entry->a != 0)
    {
        fprintf(fh, "%ld", anb_entry->a);
        
        if(anb_entry->b != 0) {
            if(anb_entry->n < 0)
                fprintf(fh, "-n");
            else
                fprintf(fh, "n");
            
            if(anb_entry->b > 0)
                fprintf(fh, "+");
            
            fprintf(fh, "%ld", anb_entry->b);
        }
    }
    else {
        if(anb_entry->n < 0)
            fprintf(fh, "-n");
        else
            fprintf(fh, "n");
        
        if(anb_entry->b) {
            if(anb_entry->b > 0)
                fprintf(fh, "+");
            
            fprintf(fh, "%ld", anb_entry->b);
        }
    }
}

/////////////////////////////////////////////////////////
//// An+B State
////
/////////////////////////////////////////////////////////
bool mycss_an_plus_b_state_skip_all(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_PARENTHESIS) {
        result->parser = anb->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_an_plus_b_state_anb(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_IDENT: {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            if(myhtml_strncasecmp(str.data, "-n-", 3) == 0)
            {
                anb_entry->n = -1;
                
                if(str.length == 3) {
                    anb_entry->b = -1;
                    
                    anb->state = mycss_an_plus_b_state_anb_plus_n_hyphen;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                /* -n-* */
                size_t len = str.length - 2;
                if(mycss_convert_data_to_integer(&str.data[2], len, &anb_entry->b) != len) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_hyphen_n_hyphen_asterisk_end");
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strncasecmp(str.data, "-n", 2) == 0) {
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_hyphen_n_end")
                
                anb_entry->n = -1;
                
                if(str.length == 2) {
                    anb->state = mycss_an_plus_b_state_anb_plus_n;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                
                size_t len = str.length - 2;
                if(mycss_convert_data_to_integer(&str.data[2], len, &anb_entry->b) != len) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strncasecmp(str.data, "n-", 2) == 0)
            {
                if(str.length == 2) {
                    anb->state = mycss_an_plus_b_state_anb_plus_n_hyphen;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                
                size_t len = str.length - 1;
                if(mycss_convert_data_to_integer(&str.data[1], len, &anb_entry->b) != len) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_n");
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strncasecmp(str.data, "n", 1) == 0) {
                
                if(str.length == 1) {
                    anb->state = mycss_an_plus_b_state_anb_plus_n;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                
                size_t len = str.length - 1;
                if(mycss_convert_data_to_integer(&str.data[1], len, &anb_entry->b) != len) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_n");
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strcasecmp(str.data, "even") == 0) {
                anb_entry->a = 2;
                anb_entry->b = 1;
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_e_v_e_n")
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strcasecmp(str.data, "odd") == 0) {
                anb_entry->a = 2;
                anb_entry->b = 0;
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_o_d_d")
                result->parser = anb->switch_parser;
            }
            else {
                myhtml_string_destroy(&str, false);
                
                mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
                result->parser = anb->switch_parser;
                return false;
            }
            
            myhtml_string_destroy(&str, false);
            break;
        }
        case MyCSS_TOKEN_TYPE_DELIM: {
            if(*token->data == '+') {
                anb->state = mycss_an_plus_b_state_anb_plus;
                
                if(result->parser != mycss_an_plus_b_state_token_all)
                    result->parser = mycss_an_plus_b_state_token_all;
            }
            else {
                mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
                result->parser = anb->switch_parser;
                return false;
            }
            break;
        }
        case MyCSS_TOKEN_TYPE_NUMBER: {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            mycss_convert_data_to_integer(str.data, str.length, &anb_entry->a);
            
            myhtml_string_destroy(&str, false);
            
            MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_number")
            result->parser = anb->switch_parser;
            break;
        }
        case MyCSS_TOKEN_TYPE_DIMENSION: {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            size_t consumed_len = mycss_convert_data_to_integer(str.data, str.length, &anb_entry->a);
            
            if(myhtml_strncasecmp(&str.data[consumed_len], "n-", 2) == 0)
            {
                size_t rem = str.length - consumed_len;
                
                if(rem == 2) {
                    anb->state = mycss_an_plus_b_state_anb_plus_n_hyphen;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                
                rem -= 1;
                if(mycss_convert_data_to_integer(&str.data[(consumed_len + 1)], rem, &anb_entry->b) != rem) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_n_hyphen_asterisk_end")
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strncasecmp(&str.data[consumed_len], "n", 1) == 0)
            {
                size_t rem = (str.length - consumed_len) - 1;
                
                if(rem == 0) {
                    anb->state = mycss_an_plus_b_state_anb_plus_n;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                
                if(mycss_convert_data_to_integer(&str.data[(consumed_len + 1)], rem, &anb_entry->b) != rem) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_n_end")
                result->parser = anb->switch_parser;
            }
            else {
                myhtml_string_destroy(&str, false);
                
                mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
                result->parser = anb->switch_parser;
                return false;
            }
            
            myhtml_string_destroy(&str, false);
            break;
        }
        default: {
            mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
            result->parser = anb->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_an_plus_b_state_anb_plus(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_IDENT: {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            if(myhtml_strncasecmp(str.data, "n-", 2) == 0)
            {
                if(str.length == 2) {
                    anb->state = mycss_an_plus_b_state_anb_plus_n_hyphen;
                    
                    if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                        result->parser = mycss_an_plus_b_state_token_skip_whitespace;
                    
                    myhtml_string_destroy(&str, false);
                    return true;
                }
                
                size_t len = str.length - 1;
                if(mycss_convert_data_to_integer(&str.data[1], len, &anb_entry->b) != len) {
                    /* parse error */
                    anb_entry->is_broken = true;
                }
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_plus_n_hyphen_asterisk_end")
                result->parser = anb->switch_parser;
            }
            else if(myhtml_strncasecmp(str.data, "n", 1) == 0) {
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_plus_n")
                anb->state = mycss_an_plus_b_state_anb_plus_n;
                
                if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                    result->parser = mycss_an_plus_b_state_token_skip_whitespace;
            }
            else {
                myhtml_string_destroy(&str, false);
                
                mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
                result->parser = anb->switch_parser;
                return false;
            }
            
            myhtml_string_destroy(&str, false);
            break;
        }
        default: {
            mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
            result->parser = anb->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_an_plus_b_state_anb_plus_n_hyphen(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_NUMBER) {
        /* begin from 0-9 */
        if(myhtml_string_chars_num_map[ (const unsigned char)(*token->data) ] != 0xff)
        {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            long res;
            mycss_convert_data_to_integer(str.data, str.length, &res);
            
            anb_entry->b = -res;
            
            MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_plus_n_hyphen_zero_hyphen_nine_end")
            result->parser = anb->switch_parser;
            
            myhtml_string_destroy(&str, false);
        }
        else {
            mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
            result->parser = anb->switch_parser;
            return false;
        }
    }
    else {
        mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
        result->parser = anb->switch_parser;
        return false;
    }
    
    return true;
}

bool mycss_an_plus_b_state_anb_plus_n(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_DELIM: {
            if(*token->data == '+') {
                anb->state = mycss_an_plus_b_state_anb_plus_n_plus;
                
                if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                    result->parser = mycss_an_plus_b_state_token_skip_whitespace;
            }
            else if(*token->data == '-') {
                anb->state = mycss_an_plus_b_state_anb_plus_n_hyphen;
                
                if(result->parser != mycss_an_plus_b_state_token_skip_whitespace)
                    result->parser = mycss_an_plus_b_state_token_skip_whitespace;
            }
            else {
                mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
                result->parser = anb->switch_parser;
                return false;
            }
            break;
        }
        case MyCSS_TOKEN_TYPE_NUMBER: {
            if(*token->data == '+' || *token->data == '-')
            {
                myhtml_string_t str;
                mycss_token_data_to_string(result->entry, token, &str, true);
                
                mycss_convert_data_to_integer(str.data, str.length, &anb_entry->b);
                
                MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_plus_n_begin_plus_vertical_bar_hyphen")
                result->parser = anb->switch_parser;
                
                myhtml_string_destroy(&str, false);
            }
            else {
                mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
                result->parser = anb->switch_parser;
                return false;
            }
            break;
        }
        default: {
            result->parser = anb->switch_parser;
            return false;
        }
    }
    
    return true;
}

bool mycss_an_plus_b_state_anb_plus_n_plus(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_NUMBER) {
        /* begin from 0-9 */
        if(myhtml_string_chars_num_map[ (const unsigned char)(*token->data) ] != 0xff)
        {
            myhtml_string_t str;
            mycss_token_data_to_string(result->entry, token, &str, true);
            
            mycss_convert_data_to_integer(str.data, str.length, &anb_entry->b);
            
            MyCSS_DEBUG_MESSAGE("mycss_an_plus_b_state_anb_plus_n_plus_zero_hyphen_nine_end")
            result->parser = anb->switch_parser;
            
            myhtml_string_destroy(&str, false);
        }
        else {
            mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
            result->parser = anb->switch_parser;
            return false;
        }
    }
    else {
        mycss_an_plus_b_parser_expectations_error(result, anb, anb_entry, token);
        result->parser = anb->switch_parser;
        return false;
    }
    
    return true;
}



