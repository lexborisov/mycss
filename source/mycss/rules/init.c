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

#include "mycss/rules/init.h"

mycss_rules_t * mycss_rules_create(void)
{
    return (mycss_rules_t*)myhtml_calloc(1, sizeof(mycss_rules_t));
}

mycss_status_t mycss_rules_init(mycss_entry_t* entry, mycss_rules_t* rules)
{
    return MyCSS_STATUS_OK;
}

mycss_status_t mycss_rules_clean_all(mycss_rules_t* rules)
{
    return MyCSS_STATUS_OK;
}

mycss_rules_t * mycss_rules_destroy(mycss_rules_t* rules, bool self_destroy)
{
    if(rules == NULL)
        return NULL;
    
    if(self_destroy) {
        myhtml_free(rules);
        return NULL;
    }
    
    return rules;
}


