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

#ifndef MyHTML_MyCSS_AN_PLUS_B_H
#define MyHTML_MyCSS_AN_PLUS_B_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
#include <mycss/result.h>
#include <mycss/entry.h>

struct mycss_an_plus_b {
    mycss_an_plus_b_state_f state;
    mycss_parser_token_f switch_parser;
    
    mycss_an_plus_b_entry_t* anb_entry;
};

struct mycss_an_plus_b_entry {
    long a;
    long b;
    long n;
    
    bool is_broken;
    mycss_result_entry_t* of;
};

mycss_an_plus_b_t * mycss_an_plus_b_create(void);
mycss_status_t mycss_an_plus_b_init(mycss_entry_t* entry, mycss_an_plus_b_t* anb);
mycss_status_t mycss_an_plus_b_clean_all(mycss_an_plus_b_t* anb);
mycss_an_plus_b_t * mycss_an_plus_b_destroy(mycss_an_plus_b_t* anb, bool self_destroy);

bool mycss_an_plus_b_state_token_all(mycss_result_t* result, mycss_token_t* token);
bool mycss_an_plus_b_state_token_skip_whitespace(mycss_result_t* result, mycss_token_t* token);
bool mycss_an_plus_b_state_skip_all(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);

void mycss_an_plus_b_print(mycss_an_plus_b_entry_t* anb_entry, FILE* fh);

void mycss_an_plus_b_parser_expectations_error(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);

bool mycss_an_plus_b_state_anb(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);
bool mycss_an_plus_b_state_anb_plus(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);
bool mycss_an_plus_b_state_anb_plus_n_hyphen(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);
bool mycss_an_plus_b_state_anb_plus_n(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);
bool mycss_an_plus_b_state_anb_plus_n_plus(mycss_result_t* result, mycss_an_plus_b_t* anb, mycss_an_plus_b_entry_t* anb_entry, mycss_token_t* token);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_AN_PLUS_B_H */
