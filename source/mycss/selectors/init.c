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

mycss_selectors_t * mycss_selectors_create(void)
{
    return (mycss_selectors_t*)mycalloc(1, sizeof(mycss_selectors_t));
}

mycss_status_t mycss_selectors_init(mycss_entry_t* entry, mycss_selectors_t* selectors)
{
    selectors->entry    = entry;
    selectors->result   = entry->result;
    selectors->state    = NULL;
    selectors->selector = NULL;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_selectors_clean_all(mycss_selectors_t* selectors)
{
    selectors->state    = NULL;
    selectors->selector = NULL;
    selectors->result   = NULL;
    
    return MyCSS_STATUS_OK;
}

mycss_selectors_t * mycss_selectors_destroy(mycss_selectors_t* selectors, bool self_destroy)
{
    if(selectors == NULL)
        return NULL;
    
    if(self_destroy) {
        myfree(selectors);
        return NULL;
    }
    
    return selectors;
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


void mycss_selectors_print_selector(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, FILE* fh)
{
    switch(selector->type) {
        case MyCSS_SELECTORS_TYPE_ELEMENT: {
            if(selector->ns)
                mycss_namespace_print(selectors->result->ns, selector->ns, fh, true);
            
            fprintf(fh, "%s", selector->key->data);
            break;
        }
        case MyCSS_SELECTORS_TYPE_ATTRIBUTE: {
            fprintf(fh, "[");
            
            if(selector->ns)
                mycss_namespace_print(selectors->result->ns, selector->ns, fh, true);
            
            fprintf(fh, "%s", selector->key->data);
            
            if(selector->value == NULL) {
                fprintf(fh, "]");
                break;
            }
            
            fprintf(fh, " %s ", mycss_selectors_resource_matcher_names_map[ selector->match ]);
            fprintf(fh, "%s]", selector->value->data);
            
            break;
        }
        case MyCSS_SELECTORS_TYPE_ID: {
            fprintf(fh, "#%s", selector->key->data);
            break;
        }
        case MyCSS_SELECTORS_TYPE_CLASS: {
            fprintf(fh, ".%s", selector->key->data);
            break;
        }
        default: {
            fprintf(fh, "Unknown selector type: %d", selector->type);
            break;
        }
    };
}

void mycss_selectors_print_chain(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, FILE* fh)
{
    while(selector) {
        mycss_selectors_print_selector(selectors, selector, fh);
        
        if(selector->next) {
            if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_DESCENDANT)
                fprintf(fh, " ");
            else if(selector->combinator == MyCSS_SELECTORS_COMBINATOR_UNDEF)
                fprintf(fh, "");
            else
                fprintf(fh, " %s ", mycss_selectors_resource_combinator_names_map[ selector->combinator ]);
        }
        
        selector = selector->next;
    }
}


