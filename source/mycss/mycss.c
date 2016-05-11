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

#include "mycss/mycss.h"
#include "mycss/tokenizer_resource.h"

mycss_t * mycss_create(void)
{
    return (mycss_t*)mycalloc(1, sizeof(mycss_t));
}

mycss_status_t mycss_init(mycss_t* mycss)
{
    mycss_status_t status = mycss_tokenizer_state_init(mycss);
    
    if(status != MyCSS_STATUS_OK)
        return status;
    
    mycss->async_incoming_buffer = mcobject_async_create();
    if(mycss->async_incoming_buffer == NULL)
        return MyCSS_STATUS_ERROR_MEMORY_ALLOCATION;
    
    mcobject_async_status_t mcstatus = mcobject_async_init(mycss->async_incoming_buffer, 32, 1024, sizeof(myhtml_incoming_buffer_t));
    if(mcstatus != MCOBJECT_ASYNC_STATUS_OK)
        return MyCSS_STATUS_ERROR_INCOMING_BUFFER_INIT;
    
    return MyCSS_STATUS_OK;
}

mycss_t * mycss_destroy(mycss_t* mycss, bool self_destroy)
{
    if(mycss == NULL)
        return NULL;
    
    mycss_tokenizer_state_destroy(mycss);
    mcobject_async_destroy(mycss->async_incoming_buffer, 1);
    
    if(self_destroy) {
        myfree(mycss);
        return NULL;
    }
    
    return mycss;
}

mycss_status_t mycss_parse(mycss_entry_t* entry, myhtml_encoding_t encoding, const char* css, size_t css_size)
{
    if(entry->type & MyCSS_ENTRY_TYPE_END) {
        mycss_entry_clean_all(entry);
    }
    
    mycss_encoding_set(entry, encoding);
    
    mycss_status_t status = mycss_tokenizer_chunk(entry, css, css_size);
    if(status)
        return status;
    
    status = mycss_tokenizer_end(entry);
    
    return status;
}

mycss_status_t mycss_parse_chunk(mycss_entry_t* entry, const char* css, size_t css_size)
{
    if(entry->type & MyCSS_ENTRY_TYPE_END) {
        mycss_entry_clean_all(entry);
    }
    
    return mycss_tokenizer_chunk(entry, css, css_size);
}

mycss_status_t mycss_parse_chunk_end(mycss_entry_t* entry)
{
    return mycss_tokenizer_end(entry);
}

// token
size_t mycss_token_position(mycss_token_t* token, size_t *return_length)
{
    if(return_length)
        *return_length = token->length;
    
    return token->begin;
}

size_t mycss_token_begin(mycss_token_t* token)
{
    return token->begin;
}

size_t mycss_token_length(mycss_token_t* token)
{
    return token->length;
}

mycss_token_type_t mycss_token_type(mycss_token_t* token)
{
    return token->type;
}

const char * mycss_token_name_by_type(mycss_token_type_t type)
{
    if(type >= MyCSS_TOKEN_TYPE_LAST_ENTRY)
        return mycss_token_type_description[MyCSS_TOKEN_TYPE_LAST_ENTRY];
    
    return mycss_token_type_description[type];
}

size_t mycss_token_data_to_string(mycss_entry_t* entry, mycss_token_t* token, myhtml_string_t* str)
{
    myhtml_string_init(entry->mchar, entry->mchar_node_id, str, (token->length + 4));
    
    mycss_string_res_t out_res;
    mycss_string_res_clean(&out_res);
    
    out_res.encoding = entry->encoding;
    
    myhtml_incoming_buffer_t *buffer = myhtml_incoming_buffer_find_by_position(entry->current_buffer, token->begin);
    size_t relative_begin = token->begin - buffer->offset;
    
    // if token data length in one buffer then print them all at once
    if((relative_begin + token->length) <= buffer->size) {
        mycss_string_data_process(str, &buffer->data[relative_begin], 0, token->length, &out_res);
        mycss_string_data_process_end(str, &out_res);
        
        return token->length;
    }
    
    // if the data are spread across multiple buffers that join them
    size_t length = token->length;
    while(buffer) {
        size_t relative_end = relative_begin + length;
        
        if(relative_end > buffer->size)
        {
            length = relative_end - buffer->size;
            
            mycss_string_data_process(str, buffer->data, relative_begin, (relative_end - length), &out_res);
            
            relative_begin = 0;
            buffer         = buffer->next;
        }
        else {
            mycss_string_data_process(str, &buffer->data[relative_begin], 0, length, &out_res);
            break;
        }
    }
    
    mycss_string_data_process_end(str, &out_res);
    
    return token->length;
}

// encoding

void mycss_encoding_set(mycss_entry_t* entry, myhtml_encoding_t encoding)
{
    entry->encoding = encoding;
}

myhtml_encoding_t mycss_encoding_get(mycss_entry_t* entry)
{
    return entry->encoding;
}

myhtml_encoding_t mycss_encoding_check_charset_rule(const char* css, size_t size)
{
    if(size < 15)
        return MyHTML_ENCODING_UTF_8;
    
    if(strncmp("@charset \"", css, 10))
        return MyHTML_ENCODING_UTF_8;
    
    size_t begin = 10;
    size_t length = begin;
    
    if(begin >= size)
        return MyHTML_ENCODING_UTF_8;
    
    while(length < size) {
        if(css[length] == '"')
        {
            ++length;
            
            if(length >= size || css[length] != ';')
                return MyHTML_ENCODING_UTF_8;
            
            size_t name_len = (length - begin) - 1;
            
            myhtml_encoding_t encoding;
            if(myhtml_encoding_by_name(&css[begin], name_len, &encoding))
                return encoding;
            
            break;
        }
        
        ++length;
    }
    
    return MyHTML_ENCODING_UTF_8;
}


