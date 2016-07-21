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

#include "mycss/namespace/state.h"

bool mycss_namespace_state_token_all(mycss_result_t* result, mycss_token_t* token)
{
    mycss_namespace_t *ns = result->entry->ns;
    return ((mycss_namespace_state_f)result->state)(result, ns, ns->ns_entry, token);
}

bool mycss_namespace_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token)
{
    if(token->type != MyCSS_TOKEN_TYPE_WHITESPACE) {
        mycss_namespace_t *ns = result->entry->ns;
        return ((mycss_namespace_state_f)result->state)(result, ns, ns->ns_entry, token);
    }
    
    return true;
}

bool mycss_namespace_state_namespace(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_AT_KEYWORD) {
        myhtml_string_t str;
        mycss_token_data_to_string(result->entry, token, &str, true, true);
        
        if(myhtml_strcmp(str.data, "namespace") == 0) {
            mycss_namespace_parser_begin(result, ns, ns_entry, token);
            result->state = mycss_namespace_state_namespace_namespace;
            
            if(result->parser != mycss_namespace_state_token_skip_whitespace)
                result->parser = mycss_namespace_state_token_skip_whitespace;
        }
        else {
            mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
            result->parser = mycss_parser_token;
        }
        
        myhtml_string_destroy(&str, false);
    }
    else {
        mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
        result->parser = mycss_parser_token;
    }
    
    return true;
}

bool mycss_namespace_state_namespace_namespace(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    switch (token->type) {
        case MyCSS_TOKEN_TYPE_IDENT: {
            mycss_namespace_parser_name(result, ns, ns_entry, token);
            result->state = mycss_namespace_state_namespace_namespace_ident;
            
            if(result->parser != mycss_namespace_state_token_skip_whitespace)
                result->parser = mycss_namespace_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_STRING: {
            mycss_namespace_parser_url(result, ns, ns_entry, token);
            result->state = mycss_namespace_state_namespace_namespace_string;
            
            if(result->parser != mycss_namespace_state_token_skip_whitespace)
                result->parser = mycss_namespace_state_token_skip_whitespace;
            break;
        }
        case MyCSS_TOKEN_TYPE_URL: {
            mycss_namespace_parser_url(result, ns, ns_entry, token);
            result->state = mycss_namespace_state_namespace_namespace_url;
            
            if(result->parser != mycss_namespace_state_token_skip_whitespace)
                result->parser = mycss_namespace_state_token_skip_whitespace;
            break;
        }
        default: {
            mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
            result->parser = mycss_parser_token;
            break;
        }
    }
    
    return true;
}

bool mycss_namespace_state_namespace_namespace_ident(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_STRING) {
        mycss_namespace_parser_url(result, ns, ns_entry, token);
        result->state = mycss_namespace_state_namespace_namespace_ident_string;
        
        if(result->parser != mycss_namespace_state_token_skip_whitespace)
            result->parser = mycss_namespace_state_token_skip_whitespace;
    }
    else if(token->type == MyCSS_TOKEN_TYPE_URL) {
        mycss_namespace_parser_url(result, ns, ns_entry, token);
        result->state = mycss_namespace_state_namespace_namespace_ident_url;
        
        if(result->parser != mycss_namespace_state_token_skip_whitespace)
            result->parser = mycss_namespace_state_token_skip_whitespace;
    }
    else {
        mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
        result->parser = mycss_parser_token;
    }
    
    return true;
}

bool mycss_namespace_state_namespace_namespace_ident_string(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_SEMICOLON) {
        mycss_namespace_parser_end(result, ns, ns_entry, token);
        printf("mycss_namespace_state_namespace_namespace_ident_string_semicolon\n");  /* End of selector */
        result->parser = mycss_parser_token;
    }
    else {
        mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
        result->parser = mycss_parser_token;
    }
    
    return true;
}

bool mycss_namespace_state_namespace_namespace_ident_url(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_SEMICOLON) {
        mycss_namespace_parser_end(result, ns, ns_entry, token);
        printf("mycss_namespace_state_namespace_namespace_ident_url_semicolon\n");  /* End of selector */
        result->parser = mycss_parser_token;
    }
    else {
        mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
        result->parser = mycss_parser_token;
    }
    
    return true;
}

bool mycss_namespace_state_namespace_namespace_string(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_SEMICOLON) {
        mycss_namespace_parser_end(result, ns, ns_entry, token);
        printf("mycss_namespace_state_namespace_namespace_string_semicolon\n");  /* End of selector */
        result->parser = mycss_parser_token;
    }
    else {
        mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
        result->parser = mycss_parser_token;
    }
    
    return true;
}

bool mycss_namespace_state_namespace_namespace_url(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_SEMICOLON) {
        mycss_namespace_parser_end(result, ns, ns_entry, token);
        printf("mycss_namespace_state_namespace_namespace_url_semicolon\n");  /* End of selector */
        result->parser = mycss_parser_token;
    }
    else {
        mycss_namespace_parser_expectations_error(result, ns, ns_entry, token);
        result->parser = mycss_parser_token;
    }
    
    return true;
}


