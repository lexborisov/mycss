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

#include "mycss/rules/state.h"


bool mycss_rules_state_token_all(mycss_result_t* result, mycss_token_t* token)
{
    mycss_rules_t *rules = result->rules;
    return ((mycss_rules_state_f)result->state)(result, rules, token);
}

bool mycss_rules_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token)
{
    if(token->type != MyCSS_TOKEN_TYPE_WHITESPACE) {
        mycss_rules_t *rules = result->rules;
        return ((mycss_rules_state_f)result->state)(result, rules, token);
    }
    
    return true;
}

bool mycss_rules_state_body(mycss_result_t* result, mycss_rules_t* rules, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_CURLY_BRACKET) {
        //printf("mycss_rules_state_body_closing_brace\n");  /* End of rules */
        
        mycss_result_entry_create_and_push(result);
        result->parser = mycss_parser_token;
    }
    
    return true;
}


