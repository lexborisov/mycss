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


bool mycss_rules_state_token_all(mycss_entry_t* entry, mycss_token_t* token)
{
    mycss_rules_t *rules = entry->rules;
    return ((mycss_rules_state_f)entry->parser_state)(entry, rules, token);
}

bool mycss_rules_state_token_skip_whitespace(mycss_entry_t* entry, mycss_token_t* token)
{
    if(token->type != MyCSS_TOKEN_TYPE_WHITESPACE) {
        mycss_rules_t *rules = entry->rules;
        return ((mycss_rules_state_f)entry->parser_state)(entry, rules, token);
    }
    
    return true;
}

bool mycss_rules_state_body(mycss_entry_t* entry, mycss_rules_t* rules, mycss_token_t* token)
{
    if(token->type == MyCSS_TOKEN_TYPE_RIGHT_CURLY_BRACKET) {
        //printf("mycss_rules_state_body_closing_brace\n");  /* End of rules */
        
        mycss_stylesheet_t *stylesheet = entry->stylesheet;
        
        stylesheet->sel_list_last = mycss_selectors_list_create_and_push(entry->selectors, stylesheet->sel_list_last);
        stylesheet->sel_list_last->selector = mycss_selectors_entry_create(entry->selectors);
        mycss_selectors_list_append_selector(entry->selectors, stylesheet->sel_list_last, stylesheet->sel_list_last->selector);
        
        entry->parser = mycss_parser_token;
    }
    
    return true;
}


