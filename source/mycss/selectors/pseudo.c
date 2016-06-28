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

#include "mycss/selectors/pseudo.h"
#include "mycss/selectors/pseudo_resource.h"
#include "myhtml/utils/resources.h"

/////////////////////////////////////////////////////////
//// Functions for a find Begin Function
////
/////////////////////////////////////////////////////////
const mycss_selectots_pseudo_begin_entry_t * mycss_pseudo_begin_entry_by_name(const char* name, size_t length)
{
    size_t idx = ((myhtml_string_chars_lowercase_map[ (const unsigned char)name[0] ] *
                   myhtml_string_chars_lowercase_map[ (const unsigned char)name[(length - 1)] ] *
                   length)
                  % MyCSS_SELECTORS_PSEUDO_NAME_STATIC_SIZE) + 1;
    
    while (mycss_selectors_pseudo_begin_map_index[idx].name)
    {
        if(mycss_selectors_pseudo_begin_map_index[idx].length == length) {
            if(myhtml_strncasecmp(mycss_selectors_pseudo_begin_map_index[idx].name, name, length) == 0)
                return &mycss_selectors_pseudo_begin_map_index[idx];
            
            if(mycss_selectors_pseudo_begin_map_index[idx].next)
                idx = mycss_selectors_pseudo_begin_map_index[idx].next;
            else
                return NULL;
        }
        else if(mycss_selectors_pseudo_begin_map_index[idx].length > length) {
            return NULL;
        }
        else {
            idx = mycss_selectors_pseudo_begin_map_index[idx].next;
        }
    }
    
    return NULL;
}

mycss_selectors_sub_type_t mycss_pseudo_begin_by_name(const char *name, size_t length)
{
    const mycss_selectots_pseudo_begin_entry_t *entry = mycss_pseudo_begin_entry_by_name(name, length);
    
    if(entry)
        return entry->sub_type;
    
    return MyCSS_SELECTORS_SUB_TYPE_UNKNOWN;
}


