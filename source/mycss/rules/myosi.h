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

#ifndef MyHTML_MyCSS_RULES_MYOSI_H
#define MyHTML_MyCSS_RULES_MYOSI_H
#pragma once

#include "mycss/myosi.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mycss_rules mycss_rules_t;

typedef bool (*mycss_rules_state_f)(mycss_entry_t* entry, mycss_rules_t* rules, mycss_token_t* token);


struct mycss_rules {
};


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_RULES_MYOSI_H */
