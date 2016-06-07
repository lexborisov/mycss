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

#ifndef MyHTML_MyCSS_RESULT_H
#define MyHTML_MyCSS_RESULT_H
#pragma once

#include "mycss/myosi.h"
#include "mycss/namespace/myosi.h"
#include "mycss/namespace/init.h"
#include "mycss/selectors/myosi.h"
#include "mycss/selectors/init.h"
#include "mycss/rules/myosi.h"
#include "mycss/rules/init.h"
#include "mycss/media/myosi.h"
#include "mycss/media/init.h"

#ifdef __cplusplus
extern "C" {
#endif

struct mycss_result {
    /* refs */
    mycss_entry_t* entry;
    
    mycss_namespace_t* ns;
    mycss_selectors_t* selectors;
    mycss_rules_t* rules;
    mycss_media_t* media;
    
    mycss_parser_token_f parser;
    
    size_t selectors_entries_id;
    size_t string_node_id;
    size_t namespace_entries_id;
};


mycss_result_t * mycss_result_create(void);
mycss_status_t mycss_result_init(mycss_entry_t* entry, mycss_result_t* result);
mycss_status_t mycss_result_clean_all(mycss_result_t* result);
mycss_result_t * mycss_result_destroy(mycss_result_t* result, bool self_destroy);

size_t mycss_result_detect_namespace_by_name(mycss_result_t* result, const char* ns, size_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_RESULT_H */
