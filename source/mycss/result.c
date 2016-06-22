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
#include "mycss/selectors/function_resource.h"

mycss_result_t * mycss_result_create(void)
{
    return (mycss_result_t*)myhtml_calloc(1, sizeof(mycss_result_t));
}

mycss_status_t mycss_result_init(mycss_entry_t* entry, mycss_result_t* result)
{
    result->entry         = entry;
    result->parser        = mycss_parser_token;
    result->switch_parser = mycss_parser_token;
    result->state         = NULL;
    
    result->mchar_value_node_id = mchar_async_node_add(entry->mchar);
    result->mchar_selector_list_node_id = mchar_async_node_add(entry->mchar);
    
    mcobject_async_status_t mcstatus;
    
    // init for selectors entry objects
    result->selectors_entries_id = mcobject_async_node_add(entry->mcasync_selectors_entries, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_SELECTORS_ENTRIES_NODE_ADD;
    
    // init for namespace entries objects
    result->namespace_entries_id = mcobject_async_node_add(entry->mcasync_namespace_entries, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_NAMESPACE_NODE_ADD;
    
    /* Result entries */
    result->mcobject_result_entries_node_id = mcobject_async_node_add(entry->mcasync_result_entries, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_RESULT_ENTRIES_ADD_NODE;
    
    /* MyHTML::String objects */
    result->string_node_id = mcobject_async_node_add(entry->mcasync_string, &mcstatus);
    if(mcstatus)
        return MyCSS_STATUS_ERROR_STRING_NODE_INIT;
    
    /* Selectors */
    result->selectors = mycss_selectors_create();
    if(result->selectors == NULL)
        return MyCSS_STATUS_ERROR_SELECTORS_CREATE;
    
    mycss_status_t status = mycss_selectors_init(entry, result->selectors);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* Namespace */
    result->ns = mycss_namespace_create();
    if(result->ns == NULL)
        return MyCSS_STATUS_ERROR_NAMESPACE_CREATE;
    
    status = mycss_namespace_init(entry, result->ns);
    if(status != MyCSS_STATUS_OK)
        return status;
    
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
    
    /* An+B */
    result->anb = mycss_an_plus_b_create();
    if(result->ns == NULL)
        return MyCSS_STATUS_ERROR_AN_PLUS_B_CREATE;
    
    status = mycss_an_plus_b_init(entry, result->anb);
    if(status != MyCSS_STATUS_OK)
        return status;
    
    /* create first result entry and selector */
    result->result_entry = result->result_entry_first = mycss_result_entry_create(result);
    result->result_entry->selector = mycss_selectors_entry_create(result->selectors);
    mycss_result_entry_append_selector(result, result->result_entry, result->result_entry->selector);
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_result_clean_all(mycss_result_t* result)
{
    result->parser        = mycss_parser_token;
    result->switch_parser = mycss_parser_token;
    result->state         = NULL;
    
    mchar_async_node_clean(result->entry->mchar, result->mchar_value_node_id);
    mchar_async_node_clean(result->entry->mchar, result->mchar_selector_list_node_id);
    
    /* Selectors */
    mycss_selectors_clean_all(result->selectors);
    mcobject_async_node_clean(result->entry->mcasync_selectors_entries, result->selectors_entries_id);
    
    /* Namespace */
    mycss_namespace_clean_all(result->ns);
    
    /* Rules */
    mycss_rules_clean_all(result->rules);
    
    /* Media */
    mycss_media_clean_all(result->media);
    
    /* Result entries */
    mcobject_async_node_clean(result->entry->mcasync_result_entries, result->mcobject_result_entries_node_id);
    
    /* An+B */
    mycss_an_plus_b_clean_all(result->anb);
    
    return MyCSS_STATUS_OK;
}

mycss_result_t * mycss_result_destroy(mycss_result_t* result, bool self_destroy)
{
    if(result == NULL)
        return NULL;
    
    mchar_async_node_delete(result->entry->mchar, result->mchar_value_node_id);
    mchar_async_node_delete(result->entry->mchar, result->mchar_selector_list_node_id);
    
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
    
    /* Result entries */
    mcobject_async_node_delete(result->entry->mcasync_result_entries, result->mcobject_result_entries_node_id);
    
    /* An+B */
    mycss_an_plus_b_destroy(result->anb, true);
    
    if(result) {
        myhtml_free(result);
        return NULL;
    }
    
    return result;
}

void mycss_result_end(mycss_result_t* result)
{
    mycss_selectors_end(result->result_entry, result->selectors);
}

mycss_result_entry_t * mycss_result_entry_create(mycss_result_t* result)
{
    mycss_result_entry_t* res_entry = mcobject_async_malloc(result->entry->mcasync_result_entries, result->mcobject_result_entries_node_id, NULL);
    mycss_result_entry_clean(res_entry);
    return res_entry;
}

void mycss_result_entry_clean(mycss_result_entry_t* result_entry)
{
    memset(result_entry, 0, sizeof(mycss_result_entry_t));
}

mycss_result_entry_t * mycss_result_entry_destroy(mycss_result_t* result, mycss_result_entry_t* result_entry, bool self_destroy)
{
    if(result_entry->selector_list) {
        for(size_t i = 0; i < result_entry->selector_list_length; i++) {
            mcobject_async_free(result->entry->mcasync_selectors_entries, result_entry->selector_list[i]);
        }
        
        mycss_selectors_entry_list_destroy(result->selectors, result_entry->selector_list);
    }
    
    if(self_destroy) {
        mcobject_async_free(result->entry->mcasync_result_entries, result_entry);
        return NULL;
    }
    
    return result_entry;
}

mycss_result_entry_t * mycss_result_entry_create_and_push(mycss_result_t* result)
{
    mycss_result_entry_t* res_entry = mycss_result_entry_create(result);
    
    if(result->result_entry) {
        res_entry->prev = result->result_entry;
        result->result_entry->next = res_entry;
    }
    
    result->result_entry = res_entry;
    return res_entry;
}

mycss_result_entry_t * mycss_result_entry_append_selector(mycss_result_t* result, mycss_result_entry_t* res_entry, mycss_selectors_entry_t* selector)
{
    if(res_entry->selector_list == NULL) {
        res_entry->selector_list = mycss_selectors_entry_list_create(result->selectors);
    }
    else {
        res_entry->selector_list = mycss_selectors_entry_list_add_one(result->selectors, res_entry->selector_list, res_entry->selector_list_length);
    }
    
    res_entry->selector_list[res_entry->selector_list_length] = selector;
    res_entry->selector_list_length++;
    
    return res_entry;
}

mycss_result_entry_t * mycss_result_entry_create_next_level_of_selectors(mycss_result_t* result, mycss_result_entry_t* current_res_entry)
{
    mycss_result_entry_t *res_entry = mycss_result_entry_create(result);
    
    res_entry->parent = current_res_entry;
    result->result_entry = res_entry;
    
    res_entry->selector = mycss_selectors_entry_create(result->selectors);
    mycss_result_entry_append_selector(result, result->result_entry, res_entry->selector);
    
    return res_entry;
}

mycss_result_entry_t * mycss_result_get_parent_set_parser(mycss_result_t* result, mycss_result_entry_t* res_entry)
{
    if(res_entry->parent == NULL) {
        if(result->parser != mycss_parser_token)
            result->parser = mycss_parser_token;
        
        if(result->switch_parser != mycss_parser_token)
            result->switch_parser = mycss_parser_token;
        
        return res_entry;
    }
    
    res_entry = res_entry->parent;
    
    if(res_entry->parent) {
        mycss_selectors_entry_t* selector = res_entry->parent->selector;
        
        if(selector->type == MyCSS_SELECTORS_TYPE_FUNCTION) {
            const mycss_selectors_function_index_t *findex = &mycss_selectors_function_parser_map_by_sub_type[ selector->sub_type ];
            
            result->parser = findex->parser;
            result->switch_parser = findex->switch_parser;
        }
        else {
            result->parser = mycss_parser_token;
            result->switch_parser = mycss_parser_token;
        }
    }
    else {
        if(result->parser != mycss_parser_token)
            result->parser = mycss_parser_token;
        
        if(result->switch_parser != mycss_parser_token)
            result->switch_parser = mycss_parser_token;
    }
    
    return res_entry;
}

/* Print */

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

void mycss_result_entry_print(mycss_result_t* result, mycss_result_entry_t* res_entry, FILE* fh)
{
    while(res_entry) {
        for(size_t i = 0; i < res_entry->selector_list_length; i++) {
            mycss_selectors_print_chain(result->selectors, res_entry->selector_list[i], fh);
            
            if((i + 1) != res_entry->selector_list_length)
                fprintf(fh, ", ");
        }
        
        if(res_entry->next)
            fprintf(fh, " {}\n");
        
        res_entry = res_entry->next;
    }
}


