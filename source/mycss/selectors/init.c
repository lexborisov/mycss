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

#include "mycss/selectors/init.h"
#include "mycss/selectors/myosi_resource.h"
#include "mycss/selectors/value_resource.h"

mycss_selectors_t * mycss_selectors_create(void)
{
    return (mycss_selectors_t*)myhtml_calloc(1, sizeof(mycss_selectors_t));
}

mycss_status_t mycss_selectors_init(mycss_entry_t* entry, mycss_selectors_t* selectors)
{
    selectors->entry  = entry;
    selectors->result = entry->result;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_selectors_clean_all(mycss_selectors_t* selectors)
{
    selectors->result = NULL;
    
    return MyCSS_STATUS_OK;
}

mycss_selectors_t * mycss_selectors_destroy(mycss_selectors_t* selectors, bool self_destroy)
{
    if(selectors == NULL)
        return NULL;
    
    if(self_destroy) {
        myhtml_free(selectors);
        return NULL;
    }
    
    return selectors;
}

void mycss_selectors_end(mycss_result_entry_t* res_entry, mycss_selectors_t* selectors)
{
    if(res_entry->selector->type == MyCSS_SELECTORS_TYPE_UNDEF) {
        if(res_entry->selector->prev) {
            res_entry->selector->prev->next = NULL;
            res_entry->selector->prev = NULL;
        }
        else {
            if(res_entry->selector_list_length) {
                res_entry->selector_list_length--;
                res_entry->selector_list[ res_entry->selector_list_length ] = NULL;
            }
        }
        
        mycss_selectors_entry_destroy(selectors, res_entry->selector, false);
    }
}

void mycss_selectors_entry_clean(mycss_selectors_entry_t* sel_entry)
{
    memset(sel_entry, 0, sizeof(mycss_selectors_entry_t));
}

mycss_selectors_entry_t * mycss_selectors_entry_find_first(mycss_selectors_entry_t* selector)
{
    while(selector->prev)
        selector = selector->prev;
    
    return selector;
}

mycss_selectors_entry_t * mycss_selectors_entry_create(mycss_selectors_t* selectors)
{
    mycss_result_t* result = selectors->result;
    
    mycss_selectors_entry_t* selector = mcobject_async_malloc(result->entry->mcasync_selectors_entries, result->selectors_entries_id, NULL);
    mycss_selectors_entry_clean(selector);
    
    return selector;
}

mycss_selectors_entry_t * mycss_selectors_entry_destroy(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, bool self_destroy)
{
    mycss_result_t* result = selectors->result;
    
    if(selector->key) {
        myhtml_string_destroy(selector->key, 0);
        mcobject_async_free(result->entry->mcasync_string, selector->key);
    }
    
    if(selector->value) {
        mycss_selector_value_destroy(result, selector->type, selector->sub_type, selector->value, true);
    }
    
    if(self_destroy) {
        mcobject_async_free(result->entry->mcasync_selectors_entries, selector);
        return NULL;
    }
    
    return selector;
}

mycss_selectors_entry_t ** mycss_selectors_entry_list_create(mycss_selectors_t* selectors)
{
    mycss_result_t* result = selectors->result;
    
    return (mycss_selectors_entry_t**) mchar_async_malloc(result->entry->mchar,
                                                          result->mchar_selector_list_node_id,
                                                          sizeof(mycss_selectors_entry_t*));
}

mycss_selectors_entry_t ** mycss_selectors_entry_list_add_one(mycss_selectors_t* selectors, mycss_selectors_entry_t** list, size_t current_size)
{
    mycss_result_t* result = selectors->result;
    size_t current_size_char = current_size * sizeof(mycss_selectors_entry_t *);
    
    return (mycss_selectors_entry_t**)
    mchar_async_realloc(result->entry->mchar, result->mchar_selector_list_node_id,
                        (char*)list, current_size_char, (current_size_char + sizeof(mycss_selectors_entry_t*)));
}

mycss_selectors_entry_t ** mycss_selectors_entry_list_destroy(mycss_selectors_t* selectors, mycss_selectors_entry_t** list)
{
    mycss_result_t* result = selectors->result;
    mchar_async_free(result->entry->mchar, result->mchar_selector_list_node_id, (char*)list);
    
    return NULL;
}

void mycss_selectors_print_selector(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, FILE* fh)
{
    switch(selector->type) {
        case MyCSS_SELECTORS_TYPE_ELEMENT: {
            if(selector->ns)
                mycss_namespace_print(selectors->result->ns, selector->ns, fh, true);
            
            if(selector->key)
                fprintf(fh, "%s", selector->key->data);
            break;
        }
        case MyCSS_SELECTORS_TYPE_ATTRIBUTE: {
            fprintf(fh, "[");
            
            if(selector->ns)
                mycss_namespace_print(selectors->result->ns, selector->ns, fh, true);
            
            if(selector->key)
                fprintf(fh, "%s", selector->key->data);
            
            if(selector->value == NULL) {
                fprintf(fh, "]");
                break;
            }
            
            fprintf(fh, " %s ", mycss_selectors_resource_matcher_names_map[ mycss_selector_value_attribute(selector->value)->match ]);
            
            if(mycss_selector_value_attribute(selector->value)->value) {
                fprintf(fh, "%s]", mycss_selector_value_attribute(selector->value)->value->data);
            }
            else {
                fprintf(fh, "]");
            }
            
            break;
        }
        case MyCSS_SELECTORS_TYPE_ID: {
            if(selector->key)
                fprintf(fh, "#%s", selector->key->data);
            break;
        }
        case MyCSS_SELECTORS_TYPE_CLASS: {
            if(selector->key)
                fprintf(fh, ".%s", selector->key->data);
            break;
        }
        case MyCSS_SELECTORS_TYPE_PSEUDO_CLASS_FUNCTION: {
            if(selector->key)
                fprintf(fh, ":%s", selector->key->data);
            
            fprintf(fh, "(");
            
            switch (selector->sub_type) {
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_HAS:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NOT:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_MATCHES:
                    if(selector->value)
                        mycss_result_entry_print(selectors->result, selector->value, fh);
                    break;
                
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_CHILD:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_CHILD:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_COLUMN:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_COLUMN:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_OF_TYPE:
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_OF_TYPE:
                    mycss_an_plus_b_print(selector->value, fh);
                    
                    if(mycss_selector_value_an_plus_b(selector->value)->of) {
                        fprintf(fh, " of ");
                        mycss_result_entry_print(selectors->result, mycss_selector_value_an_plus_b(selector->value)->of, fh);
                    }
                    
                    break;
                
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_DROP: {
                    mycss_selectors_function_drop_type_t drop_val = mycss_selector_value_drop(selector->value);
                    
                    if(drop_val & MyCSS_SELECTORS_FUNCTION_DROP_TYPE_ACTIVE) {
                        fprintf(fh, "active");
                        
                        if(drop_val != MyCSS_SELECTORS_FUNCTION_DROP_TYPE_ACTIVE)
                            fprintf(fh, " || ");
                    }
                    
                    if(drop_val & MyCSS_SELECTORS_FUNCTION_DROP_TYPE_VALID) {
                        fprintf(fh, "valid");
                        
                        if(drop_val & MyCSS_SELECTORS_FUNCTION_DROP_TYPE_INVALID)
                            fprintf(fh, " || ");
                    }
                    
                    if(drop_val & MyCSS_SELECTORS_FUNCTION_DROP_TYPE_INVALID) {
                        fprintf(fh, "invalid");
                    }
                    
                    break;
                }
                
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_DIR: {
                    if(selector->value)
                        fprintf(fh, "%s", mycss_selector_value_string(selector->value)->data);
                    
                    break;
                }
                
                case MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_LANG: {
                    if(selector->value) {
                        mycss_selectors_value_lang_t *lang = mycss_selector_value_lang(selector->value);
                        
                        while(lang) {
                            fprintf(fh, "%s", lang->str.data);
                            
                            if(lang->next)
                                fprintf(fh, ", ");
                            
                            lang = lang->next;
                        }
                        
                    }
                    
                    break;
                }
                
                default:
                    break;
            }
            
            fprintf(fh, ")");
            
            break;
        }
            
        case MyCSS_SELECTORS_TYPE_PSEUDO_CLASS: {
            fprintf(fh, "::");
            
            if(selector->key)
                fprintf(fh, "%s", selector->key->data);
            
            break;
        }
            
        default: {
            if(selector->key)
                fprintf(fh, "%s", selector->key->data);
            
            fprintf(fh, "^U");
            break;
        }
    };
    
    if(selector->sub_type == MyCSS_SELECTORS_SUB_TYPE_UNKNOWN)
        fprintf(fh, "^UST");
    
    if(selector->flags & MyCSS_SELECTORS_FLAGS_SELECTOR_BAD)
        fprintf(fh, "^B");
}

void mycss_selectors_print_chain(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, FILE* fh)
{
    while(selector) {
        if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_DESCENDANT)
            fprintf(fh, " ");
        else if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_UNDEF)
            fprintf(fh, "");
        else
            fprintf(fh, " %s ", mycss_selectors_resource_combinator_names_map[ selector->combinator ]);
        
        mycss_selectors_print_selector(selectors, selector, fh);
        
        selector = selector->next;
    }
}


