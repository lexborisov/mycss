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

#include "mycss/namespace/init.h"

mycss_namespace_t * mycss_namespace_create(void)
{
    return (mycss_namespace_t*)myhtml_calloc(1, sizeof(mycss_namespace_t));
}

mycss_status_t mycss_namespace_init(mycss_entry_t* entry, mycss_namespace_t* ns)
{
    ns->name_tree = mctree_create(14);
    ns->ns_id_counter = 0;
    
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_namespace_clean_all(mycss_namespace_t* ns)
{
    mctree_clean(ns->name_tree);
    
    ns->ns_id_counter = 0;
    
    return MyCSS_STATUS_OK;
}

mycss_namespace_t * mycss_namespace_destroy(mycss_namespace_t* ns, bool self_destroy)
{
    if(ns == NULL)
        return NULL;
    
    mctree_destroy(ns->name_tree);
    
    if(self_destroy) {
        myhtml_free(ns);
        return NULL;
    }
    
    return ns;
}

void mycss_namespace_entry_clean(mycss_namespace_entry_t* ns_entry)
{
    memset(ns_entry, 0, sizeof(mycss_namespace_entry_t));
}

const char * mycss_namespace_name_by_id(mycss_namespace_t* ns, size_t ns_id, size_t* length)
{
    if(ns_id < MyHTML_NAMESPACE_LAST_ENTRY)
        return myhtml_namespace_name_by_id((myhtml_namespace_t)ns_id, length);
    
    ns_id -= MyHTML_NAMESPACE_LAST_ENTRY;
    
    mycss_namespace_entry_t *entry = (mycss_namespace_entry_t*)(ns->name_tree->nodes[ ns_id ].value);
    
    if(length)
        *length = entry->name->length;
    
    return entry->name->data;
}

void mycss_namespace_print(mycss_namespace_t* ns, size_t ns_id, FILE* fh, bool with_vbar)
{
    size_t length;
    const char *name = mycss_namespace_name_by_id(ns, ns_id, &length);
    
    if(length == 0) {
        fprintf(fh, "*");
        
        if(with_vbar)
            fprintf(fh, "|");
        
        return;
    }
    
    fprintf(fh, "%s", name);
    
    if(with_vbar)
        fprintf(fh, "|");
}

