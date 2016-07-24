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

#include "mycss/selectors/list.h"


mycss_selectors_list_t * mycss_selectors_list_create(mycss_selectors_t* selectors)
{
    mycss_selectors_list_t* selectors_list = mcobject_malloc(selectors->mcobject_list_entries, NULL);
    mycss_selectors_list_clean(selectors_list);
    return selectors_list;
}

void mycss_selectors_list_clean(mycss_selectors_list_t* selectors_list)
{
    memset(selectors_list, 0, sizeof(mycss_selectors_list_t));
}

mycss_selectors_list_t * mycss_selectors_list_destroy(mycss_selectors_t* selectors, mycss_selectors_list_t* selectors_list, bool self_destroy)
{
    if(selectors_list == NULL)
        return NULL;
    
    mycss_entry_t *entry = selectors->entry;
    
    if(selectors_list->selector_list)
    {
        for(size_t i = 0; i < selectors_list->selector_list_length; i++)
            mycss_selectors_entry_destroy(entry->selectors, selectors_list->selector_list[i], true);
        
        mycss_selectors_entry_list_destroy(entry->selectors, selectors_list->selector_list);
    }
    
    if(self_destroy) {
        mcobject_free(selectors->mcobject_list_entries, selectors_list);
        return NULL;
    }
    
    return selectors_list;
}

mycss_selectors_list_t * mycss_selectors_list_create_and_push(mycss_selectors_t* selectors, mycss_selectors_list_t* current_list)
{
    mycss_selectors_list_t* selectors_list = mycss_selectors_list_create(selectors);
    
    if(current_list) {
        selectors_list->prev = current_list;
        current_list->next = selectors_list;
    }
    
    return selectors_list;
}

mycss_selectors_list_t * mycss_selectors_list_append_selector(mycss_selectors_t* selectors, mycss_selectors_list_t* current_list, mycss_selectors_entry_t* selector)
{
    if(current_list->selector_list == NULL) {
        current_list->selector_list = mycss_selectors_entry_list_create(selectors);
    }
    else {
        current_list->selector_list = mycss_selectors_entry_list_add_one(selectors, current_list->selector_list, current_list->selector_list_length);
    }
    
    current_list->selector_list[current_list->selector_list_length] = selector;
    current_list->selector_list_length++;
    
    return current_list;
}

mycss_selectors_list_t * mycss_selectors_list_create_next_level_of_selectors(mycss_selectors_t* selectors, mycss_stylesheet_t* stylesheet, mycss_selectors_list_t* current_list)
{
    mycss_selectors_list_t *list = mycss_selectors_list_create(selectors);
    
    list->parent = current_list;
    
    list->selector = mycss_selectors_entry_create(selectors);
    mycss_selectors_list_append_selector(selectors, list, list->selector);
    
    stylesheet->sel_list_last = list;
    
    return list;
}


