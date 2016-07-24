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


void mycss_namespace_parser_begin(mycss_entry_t* entry, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    mycss_namespace_entry_t* new_ns_entry = mcobject_malloc(ns->mcobject_entries, NULL);
    mycss_namespace_entry_clean(new_ns_entry);
    
    if(ns_entry) {
        ns_entry->next = new_ns_entry;
        new_ns_entry->prev = ns_entry;
    }
    
    ns->ns_entry = new_ns_entry;
}

void mycss_namespace_parser_name(mycss_entry_t* entry, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    ns_entry->name = str;
}

void mycss_namespace_parser_url(mycss_entry_t* entry, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    myhtml_string_t *str = mcobject_malloc(entry->mcobject_string_entries, NULL);
    mycss_token_data_to_string(entry, token, str, true, false);
    
    ns_entry->url = str;
}

void mycss_namespace_parser_end(mycss_entry_t* entry, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    myhtml_string_t *str_url = ns_entry->url;
    ns_entry->ns_id = myhtml_namespace_id_by_url(str_url->data, str_url->length);
    
    if(ns_entry->name) {
        myhtml_string_t *str = ns_entry->name;
        
        ns_entry->mctree_id = mctree_insert(entry->stylesheet->ns_stylesheet.name_tree, str->data, str->length, (void*)ns_entry, NULL);
        
        if(str_url->length && ns_entry->ns_id == MyHTML_NAMESPACE_UNDEF)
            ns_entry->ns_id = MyHTML_NAMESPACE_LAST_ENTRY + (myhtml_namespace_t)ns_entry->mctree_id;
        
        return;
    }
    
    mycss_namespace_stylesheet_append_default(&entry->stylesheet->ns_stylesheet, ns_entry);
    ns_entry->mctree_id = mctree_insert(entry->stylesheet->ns_stylesheet.name_tree, " ", 1, (void*)ns_entry, NULL);
    
    if(str_url->length && ns_entry->ns_id == MyHTML_NAMESPACE_UNDEF)
        ns_entry->ns_id = MyHTML_NAMESPACE_LAST_ENTRY + (myhtml_namespace_t)ns_entry->mctree_id;
}

void mycss_namespace_parser_expectations_error(mycss_entry_t* entry, mycss_namespace_t* ns, mycss_namespace_entry_t* ns_entry, mycss_token_t* token)
{
    mycss_namespace_entry_destroy(ns_entry, entry, false);
    
    if(ns_entry)
        mcobject_free(ns->mcobject_entries, ns_entry);
    
    if(ns_entry->prev) {
        ns->ns_entry = ns_entry->prev;
        ns->ns_entry->next = NULL;
    }
    else {
        ns->ns_entry = NULL;
    }
    
    printf("Expectations error: Namespace!\n");
}


