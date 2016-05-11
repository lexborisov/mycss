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

#ifndef MyHTML_MyCSS_ENTRY_H
#define MyHTML_MyCSS_ENTRY_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mycss/myosi.h"
#include "mycss/mycss.h"
#include "mycss/parser.h"
#include "myhtml/utils/mcobject_async.h"
#include "myhtml/utils/mchar_async.h"

struct mycss_entry {
    mycss_t* mycss;
    mycss_token_t* token;
    
    mchar_async_t* mchar;
    size_t mchar_node_id;
    
    mycss_entry_type_t type;
    myhtml_encoding_t encoding;
    
    mycss_tokenizer_state_t state;
    mycss_tokenizer_state_t state_back;
    mycss_token_ready_callback_f token_ready_callback;
    
    mcobject_async_t *mcasync_token;
    size_t token_id;
    
    myhtml_incoming_buffer_t* first_buffer;
    myhtml_incoming_buffer_t* current_buffer;
    size_t incoming_buffer_id;
    
    size_t token_counter;
    size_t help_counter;
};

mycss_entry_t * mycss_entry_create(void);
mycss_status_t mycss_entry_init(mycss_t* mycss, mycss_entry_t* entry);
mycss_status_t mycss_entry_clean_all(mycss_entry_t* entry);
mycss_entry_t * mycss_entry_destroy(mycss_entry_t* entry, bool self_destroy);

mycss_token_ready_callback_f mycss_entry_token_ready_callback(mycss_entry_t* entry, mycss_token_ready_callback_f callback_f);

size_t mycss_entry_token_count(mycss_entry_t* entry);
myhtml_incoming_buffer_t * mycss_entry_incoming_buffer_current(mycss_entry_t* entry);
myhtml_incoming_buffer_t * mycss_entry_incoming_buffer_first(mycss_entry_t* entry);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_ENTRY_H */
