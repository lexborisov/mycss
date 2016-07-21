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

#include "mycss/entry.h"


mycss_entry_t * mycss_entry_create(void)
{
    return (mycss_entry_t*)myhtml_calloc(1, sizeof(mycss_entry_t));
}

mycss_status_t mycss_entry_init(mycss_t* mycss, mycss_entry_t* entry)
{
    entry->mycss = mycss;
    
    // Other init
    entry->mchar = mchar_async_create(128, (4096 * 5));
    entry->mchar_node_id = mchar_async_node_add(entry->mchar);
    entry->mchar_value_node_id = mchar_async_node_add(entry->mchar);
    
    /* Selectors */
    entry->selectors = mycss_selectors_create();
    if(entry->selectors == NULL)
        return MyCSS_STATUS_ERROR_SELECTORS_CREATE;
    
    mycss_status_t status = mycss_selectors_init(entry, entry->selectors);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* Namespace */
    entry->ns = mycss_namespace_create();
    if(entry->ns == NULL)
        return MyCSS_STATUS_ERROR_NAMESPACE_CREATE;
    
    status = mycss_namespace_init(entry, entry->ns);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* Rules */
    entry->rules = mycss_rules_create();
    if(entry->rules == NULL)
        return MyCSS_STATUS_ERROR_RULES_CREATE;
    
    status = mycss_rules_init(entry, entry->rules);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* Media */
    entry->media = mycss_media_create();
    if(entry->media == NULL)
        return MyCSS_STATUS_ERROR_RULES_CREATE;
    
    status = mycss_media_init(entry, entry->media);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* An+B */
    entry->anb = mycss_an_plus_b_create();
    if(entry->ns == NULL)
        return MyCSS_STATUS_ERROR_AN_PLUS_B_CREATE;
    
    status = mycss_an_plus_b_init(entry, entry->anb);
    if(status != MyCSS_STATUS_OK)
        return status;
        
    /* Incoming Buffer */
    entry->mcobject_incoming_buffer = mcobject_create();
    if(entry->mcobject_incoming_buffer == NULL)
        return MyCSS_STATUS_ERROR_ENTRY_INCOMING_BUFFER_CREATE;
    
    myhtml_status_t myhtml_status = mcobject_init(entry->mcobject_incoming_buffer, 256, sizeof(myhtml_incoming_buffer_t));
    if(myhtml_status)
        return MyCSS_STATUS_ERROR_ENTRY_INCOMING_BUFFER_INIT;
    
    /* callbacks */
    entry->token_ready_callback = mycss_parser_token_ready_callback_function;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_entry_clean_all(mycss_entry_t* entry)
{
    mcobject_clean(entry->mcobject_incoming_buffer);
    mchar_async_node_clean(entry->mchar, entry->mchar_node_id);
    mchar_async_node_clean(entry->mchar, entry->mchar_value_node_id);
    
    /* CSS Modules */
    mycss_selectors_clean_all(entry->selectors);
    mycss_namespace_clean_all(entry->ns);
    mycss_rules_clean_all(entry->rules);
    mycss_media_clean_all(entry->media);
    mycss_an_plus_b_clean_all(entry->anb);
    
    entry->token                = NULL;
    entry->result               = NULL;
    entry->state                = MyCSS_TOKENIZER_STATE_DATA;
    entry->state_back           = MyCSS_TOKENIZER_STATE_DATA;
    entry->first_buffer         = NULL;
    entry->current_buffer       = NULL;
    entry->token_counter        = 0;
    entry->help_counter         = 0;
    entry->type                 = MyCSS_ENTRY_TYPE_CLEAN;
    
    if(entry->token) {
        myhtml_free(entry->token);
        entry->token = NULL;
    }
    
    return MyCSS_STATUS_OK;
}

mycss_entry_t * mycss_entry_destroy(mycss_entry_t* entry, bool self_destroy)
{
    if(entry == NULL)
        return NULL;
    
    entry->mchar = mchar_async_destroy(entry->mchar, 1);
    
    /* CSS Modules */
    entry->selectors = mycss_selectors_destroy(entry->selectors, true);
    entry->ns        = mycss_namespace_destroy(entry->ns, true);
    entry->rules     = mycss_rules_destroy(entry->rules, true);
    entry->media     = mycss_media_destroy(entry->media, true);
    entry->anb       = mycss_an_plus_b_destroy(entry->anb, true);
    
    entry->mcobject_incoming_buffer = mcobject_destroy(entry->mcobject_incoming_buffer, true);
    
    if(entry->token) {
        myhtml_free(entry->token);
        entry->token = NULL;
    }
    
    if(self_destroy) {
        myhtml_free(entry);
        return NULL;
    }
    
    return entry;
}

mycss_token_ready_callback_f mycss_entry_token_ready_callback(mycss_entry_t* entry, mycss_token_ready_callback_f callback_f)
{
    if(callback_f)
        entry->token_ready_callback = callback_f;
    
    return entry->token_ready_callback;
}

size_t mycss_entry_token_count(mycss_entry_t* entry)
{
    return entry->token_counter;
}

myhtml_incoming_buffer_t * mycss_entry_incoming_buffer_current(mycss_entry_t* entry)
{
    return entry->current_buffer;
}

myhtml_incoming_buffer_t * mycss_entry_incoming_buffer_first(mycss_entry_t* entry)
{
    return entry->first_buffer;
}

mycss_result_t * mycss_entry_result(mycss_entry_t* entry)
{
    return entry->result;
}


