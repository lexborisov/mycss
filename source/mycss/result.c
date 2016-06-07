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

#include "mycss/result.h"


mycss_result_t * mycss_result_create(void)
{
    return (mycss_result_t*)mycalloc(1, sizeof(mycss_result_t));
}

mycss_status_t mycss_result_init(mycss_entry_t* entry, mycss_result_t* result)
{
    result->entry = entry;
    result->parser = mycss_parser_token;
    
    /* Selectors */
    result->selectors = mycss_selectors_create();
    if(result->selectors == NULL)
        return MyCSS_STATUS_ERROR_SELECTORS_CREATE;
    
    mycss_status_t status = mycss_selectors_init(entry, result->selectors);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    // init for selectors entry objects
    mcobject_async_status_t mcstatus;
    result->selectors_entries_id = mcobject_async_node_add(entry->mcasync_selectors_entries, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_SELECTORS_ENTRIES_NODE_ADD;
    
    /* Namespace */
    result->ns = mycss_namespace_create();
    if(result->ns == NULL)
        return MyCSS_STATUS_ERROR_NAMESPACE_CREATE;
    
    status = mycss_namespace_init(entry, result->ns);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    // init for namespace entries objects
    result->namespace_entries_id = mcobject_async_node_add(entry->mcasync_namespace_entries, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_NAMESPACE_NODE_ADD;
    
    /* Rules */
    result->rules = mycss_rules_create();
    if(result->rules == NULL)
        return MyCSS_STATUS_ERROR_RULES_CREATE;
    
    status = mycss_rules_init(entry, result->rules);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* Media */
    result->media = mycss_media_create();
    if(result->media == NULL)
        return MyCSS_STATUS_ERROR_RULES_CREATE;
    
    status = mycss_media_init(entry, result->media);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* MyHTML::String objects */
    result->string_node_id = mcobject_async_node_add(entry->mcasync_string, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_STRING_NODE_INIT;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_result_clean_all(mycss_result_t* result)
{
    /* Selectors */
    mycss_selectors_clean_all(result->selectors);
    mcobject_async_node_clean(result->entry->mcasync_selectors_entries, result->selectors_entries_id);
    
    /* Namespace */
    mycss_namespace_clean_all(result->ns);
    
    /* Rules */
    mycss_rules_clean_all(result->rules);
    
    /* Media */
    mycss_media_clean_all(result->media);
    
    return MyCSS_STATUS_OK;
}

mycss_result_t * mycss_result_destroy(mycss_result_t* result, bool self_destroy)
{
    if(result == NULL)
        return NULL;
    
    /* Selectors */
    mycss_selectors_destroy(result->selectors, true);
    mcobject_async_node_delete(result->entry->mcasync_selectors_entries, result->selectors_entries_id);
    
    /* Namespace */
    mycss_namespace_destroy(result->ns, true);
    
    /* Rules */
    mycss_rules_destroy(result->rules, true);
    
    /* Media */
    mycss_media_destroy(result->media, true);
    
    /* MyHTML::String objects */
    mcobject_async_node_delete(result->entry->mcasync_string, result->string_node_id);
    
    if(result) {
        myfree(result);
        return NULL;
    }
    
    return result;
}

size_t mycss_result_detect_namespace_by_name(mycss_result_t* result, const char* ns, size_t length)
{
    if(result->ns == NULL)
        return 0;
    
    myhtml_namespace_t ns_id;
    bool find_it = myhtml_namespace_id_by_name(ns, length, &ns_id);
    
    if(find_it)
        return (size_t)ns_id;
    
    if(result->ns->name_tree == NULL)
        return 0;
    
    mctree_index_t idx = mctree_search_lowercase(result->ns->name_tree, ns, length);
    
    if(idx == 0)
        return 0;
    
    mycss_namespace_entry_t *entry = (mycss_namespace_entry_t*)(result->ns->name_tree->nodes[ idx ].value);
    return entry->ns_id;
}





