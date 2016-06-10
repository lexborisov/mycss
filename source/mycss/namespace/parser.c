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

#include "mycss/namespace/parser.h"


void mycss_namespace_parser_begin(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    mycss_namespace_entry_t* new_ns_entry = mcobject_async_malloc(result->entry->mcasync_namespace_entries, result->namespace_entries_id, NULL);
    mycss_namespace_entry_clean(new_ns_entry);
    
    if(ns_entry) {
        ns_entry->next = new_ns_entry;
        new_ns_entry->prev = ns_entry;
    }
    
    ns->ns_entry = new_ns_entry;
}

void mycss_namespace_parser_name(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    ns_entry->name = str;
}

void mycss_namespace_parser_url(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_async_malloc(result->entry->mcasync_string, result->string_node_id, NULL);
    mycss_token_data_to_string(result->entry, token, str, true);
    
    ns_entry->url = str;
}

void mycss_namespace_parser_end(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(ns_entry->name) {
        myhtml_string_t *str = ns_entry->name;
        
        myhtml_namespace_t ns_id;
        bool find_it = myhtml_namespace_id_by_name(str->data, str->length, &ns_id);
        
        if(find_it) {
            ns_entry->ns_id = ns_id;
        }
        else {
            mctree_index_t idx = mctree_search_lowercase(ns->name_tree, str->data, str->length);
            
            if(idx == 0) {
                ns_entry->ns_id = MyHTML_NAMESPACE_LAST_ENTRY + mctree_insert(ns->name_tree, str->data, str->length, (void*)ns_entry, NULL);
                ns->ns_id_counter++;
            }
            else {
                ns_entry->ns_id = (size_t)(ns->name_tree->nodes[ idx ].value);
            }
        }
    }
}

void mycss_namespace_parser_expectations_error(mycss_result_t* result, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    if(ns_entry->name) {
        myhtml_string_destroy(ns_entry->name, 0);
        mcobject_async_free(result->entry->mcasync_string, ns_entry->name);
    }
    
    if(ns_entry->url) {
        myhtml_string_destroy(ns_entry->url, 0);
        mcobject_async_free(result->entry->mcasync_string, ns_entry->url);
    }
    
    if(ns_entry)
        mcobject_async_free(result->entry->mcasync_namespace_entries, ns_entry);
    
    if(ns_entry->prev) {
        ns->ns_entry = ns_entry->prev;
        ns->ns_entry->next = NULL;
    }
    else {
        ns->ns_entry = NULL;
    }
    
    printf("Expectations error: Namespace!\n");
}


