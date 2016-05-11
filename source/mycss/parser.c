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

#include "mycss/parser.h"
#include "mycss/tokenizer_resource.h"

mycss_token_t * mycss_parser_token_ready_callback_function(mycss_entry_t* entry, mycss_token_t* token)
{
    //mycss_token_t* new_token = mcobject_async_malloc(entry->mcasync_token, entry->token_id, NULL);
    
//    if(token->type == MyCSS_TOKEN_TYPE_IDENT ||
//       token->type == MyCSS_TOKEN_TYPE_STRING ||
//       token->type == MyCSS_TOKEN_TYPE_URL ||
//       token->type == MyCSS_TOKEN_TYPE_FUNCTION)
//    {
    myhtml_string_t str;
    mycss_token_data_to_string(entry, token, &str);
    
    printf("%s", str.data);
    
    myhtml_string_destroy(&str, false);
//    }
    
    //printf("%zu: <%s>\n", entry->token_counter, mycss_token_type_description[ token->type ]);
    
    return entry->token;
}

