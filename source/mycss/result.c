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
    result->entry                  = entry;
    result->parser                 = mycss_parser_token;
    result->parser_switch          = mycss_parser_token;
    result->parser_original        = NULL;
    result->state                  = NULL;
    result->callback_selector_done = NULL;
    
    /* Result Entries */
    result->mcobject_entries = mcobject_create();
    if(result->mcobject_entries == NULL)
        return MyCSS_STATUS_ERROR_RESULT_ENTRIES_CREATE;
    
    myhtml_status_t myhtml_status = mcobject_init(result->mcobject_entries, 256, sizeof(mycss_result_entry_t));
    if(myhtml_status)
        return MyCSS_STATUS_ERROR_RESULT_ENTRIES_INIT;
    
    /* String Entries */
    result->mcobject_string_entries = mcobject_create();
    if(result->mcobject_string_entries == NULL)
        return MyCSS_STATUS_ERROR_STRING_CREATE;
    
    myhtml_status = mcobject_init(result->mcobject_string_entries, 256, sizeof(mycss_result_entry_t));
    if(myhtml_status)
        return MyCSS_STATUS_ERROR_STRING_INIT;
    
    /* create first result entry and selector */
    if(entry->selectors) {
        result->result_entry = result->result_entry_first = mycss_result_entry_create(result);
        result->result_entry->selector = mycss_selectors_entry_create(entry->selectors);
        mycss_result_entry_append_selector(result, result->result_entry, result->result_entry->selector);
    }
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_result_clean_all(mycss_result_t* result)
{
    result->parser                 = mycss_parser_token;
    result->parser_switch          = mycss_parser_token;
    result->parser_original        = NULL;
    result->state                  = NULL;
    result->callback_selector_done = NULL;
    
    mcobject_clean(result->mcobject_entries);
    mcobject_clean(result->mcobject_string_entries);
    
    return MyCSS_STATUS_OK;
}

mycss_result_t * mycss_result_destroy(mycss_result_t* result, bool self_destroy)
{
    if(result == NULL)
        return NULL;
    
    result->mcobject_entries = mcobject_destroy(result->mcobject_entries, true);
    result->mcobject_string_entries = mcobject_destroy(result->mcobject_string_entries, true);
    
    if(result) {
        myhtml_free(result);
        return NULL;
    }
    
    return result;
}

void mycss_result_end(mycss_result_t* result)
{
    mycss_selectors_end(result->result_entry, result->entry->selectors, false);
}

mycss_result_entry_t * mycss_result_entry_create(mycss_result_t* result)
{
    mycss_result_entry_t* res_entry = mcobject_malloc(result->mcobject_entries, NULL);
    mycss_result_entry_clean(res_entry);
    return res_entry;
}

void mycss_result_entry_clean(mycss_result_entry_t* result_entry)
{
    memset(result_entry, 0, sizeof(mycss_result_entry_t));
}

mycss_result_entry_t * mycss_result_entry_destroy(mycss_result_t* result, mycss_result_entry_t* result_entry, bool self_destroy)
{
    if(result_entry == NULL)
        return NULL;
    
    mycss_entry_t *entry = result->entry;
    
    if(result_entry->selector_list)
    {
        for(size_t i = 0; i < result_entry->selector_list_length; i++)
            mycss_selectors_entry_destroy(entry->selectors, result_entry->selector_list[i], true);
        
        mycss_selectors_entry_list_destroy(entry->selectors, result_entry->selector_list);
    }
    
    if(self_destroy) {
        mcobject_free(result->mcobject_entries, result_entry);
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
        res_entry->selector_list = mycss_selectors_entry_list_create(result->entry->selectors);
    }
    else {
        res_entry->selector_list = mycss_selectors_entry_list_add_one(result->entry->selectors, res_entry->selector_list, res_entry->selector_list_length);
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
    
    res_entry->selector = mycss_selectors_entry_create(result->entry->selectors);
    mycss_result_entry_append_selector(result, result->result_entry, res_entry->selector);
    
    return res_entry;
}

mycss_result_entry_t * mycss_result_get_parent_set_parser(mycss_result_t* result, mycss_result_entry_t* res_entry)
{
    if(res_entry->parent == NULL) {
        if(result->parser != mycss_parser_token)
            result->parser = mycss_parser_token;
        
        if(result->parser_switch != mycss_parser_token)
            result->parser_switch = mycss_parser_token;
        
        return res_entry;
    }
    
    res_entry = res_entry->parent;
    
    if(res_entry->parent) {
        mycss_selectors_entry_t* selector = res_entry->parent->selector;
        
        if(selector->type == MyCSS_SELECTORS_TYPE_PSEUDO_CLASS_FUNCTION) {
            const mycss_selectors_function_index_t *findex = &mycss_selectors_function_parser_map_by_sub_type[ selector->sub_type ];
            
            result->parser = findex->parser;
            result->parser_switch = findex->switch_parser;
        }
        else {
            result->parser = mycss_parser_token;
            result->parser_switch = mycss_parser_token;
        }
    }
    else {
        if(result->parser != mycss_parser_token)
            result->parser = mycss_parser_token;
        
        if(result->parser_switch != mycss_parser_token)
            result->parser_switch = mycss_parser_token;
    }
    
    return res_entry;
}

/* Print */

size_t mycss_result_detect_namespace_by_name(mycss_result_t* result, const char* ns_name, size_t length)
{
    mycss_namespace_t *ns = result->entry->ns;
    
    if(ns == NULL)
        return 0;
    
    myhtml_namespace_t ns_id;
    bool find_it = myhtml_namespace_id_by_name(ns_name, length, &ns_id);
    
    if(find_it)
        return (size_t)ns_id;
    
    if(ns->name_tree == NULL)
        return 0;
    
    mctree_index_t idx = mctree_search_lowercase(ns->name_tree, ns_name, length);
    
    if(idx == 0)
        return 0;
    
    mycss_namespace_entry_t *entry = (mycss_namespace_entry_t*)(ns->name_tree->nodes[ idx ].value);
    return entry->ns_id;
}

void mycss_result_entry_print(mycss_result_t* result, mycss_result_entry_t* res_entry, FILE* fh)
{
    while(res_entry) {
        for(size_t i = 0; i < res_entry->selector_list_length; i++) {
            mycss_selectors_print_chain(result->entry->selectors, res_entry->selector_list[i], fh);
            
            if((i + 1) != res_entry->selector_list_length)
                fprintf(fh, ", ");
        }
        
        if(res_entry->next)
            fprintf(fh, " {}\n");
        
        res_entry = res_entry->next;
    }
}


