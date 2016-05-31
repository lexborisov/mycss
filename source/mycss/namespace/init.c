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
    return (mycss_namespace_t*)mycalloc(1, sizeof(mycss_namespace_t));
}

mycss_status_t mycss_namespace_init(mycss_entry_t* entry, mycss_namespace_t* ns)
{
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_namespace_clean_all(mycss_namespace_t* ns)
{
    return MyCSS_STATUS_OK;
}

mycss_namespace_t * mycss_namespace_destroy(mycss_namespace_t* ns, bool self_destroy)
{
    if(ns == NULL)
        return NULL;
    
    if(self_destroy) {
        myfree(ns);
        return NULL;
    }
    
    return ns;
}


