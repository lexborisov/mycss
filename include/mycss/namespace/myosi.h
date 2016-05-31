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

#ifndef MyHTML_MyCSS_NAMESPACE_MYOSI_H
#define MyHTML_MyCSS_NAMESPACE_MYOSI_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "mycss/myosi.h"
#include "myhtml/mystring.h"

typedef struct mycss_namespace mycss_namespace_t;
typedef struct mycss_namespace_entry mycss_namespace_entry_t;

typedef bool (*mycss_namespace_state_f)(mycss_result_t* result, mycss_namespace_t* ns, mycss_token_t* token);


struct mycss_namespace {
    mycss_namespace_state_f state;
};

struct mycss_namespace_entry {
    myhtml_string_t name;
    myhtml_string_t url;
};


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_NAMESPACE_MYOSI_H */
