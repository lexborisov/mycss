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

#ifndef MyHTML_MyCSS_SELECTORS_MYOSI_H
#define MyHTML_MyCSS_SELECTORS_MYOSI_H
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mycss_selectors mycss_selectors_t;
typedef struct mycss_selectors_entry mycss_selectors_entry_t;

#include "mycss/myosi.h"
#include "mycss/mystring.h"

typedef bool (*mycss_selectors_state_f)(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);

enum mycss_selectors_match {
    MyCSS_SELECTORS_MATCH_EQUAL      = 0x00, //  =
    MyCSS_SELECTORS_MATCH_INCLUDE    = 0x01, // ~=
    MyCSS_SELECTORS_MATCH_DASH       = 0x02, // |=
    MyCSS_SELECTORS_MATCH_PREFIX     = 0x03, // ^=
    MyCSS_SELECTORS_MATCH_SUFFIX     = 0x04, // $=
    MyCSS_SELECTORS_MATCH_SUBSTRING  = 0x05, // *=
    MyCSS_SELECTORS_MATCH_LAST_ENTRY = 0x06
}
typedef mycss_selectors_match_t;

enum mycss_selectors_combinator {
    MyCSS_SELECTORS_COMBINATOR_UNDEF             = 0x00, // two compound selectors [key=val].foo
    MyCSS_SELECTORS_COMBINATOR_DESCENDANT        = 0x01, // '*' or '>>' or WHITESPACE
    MyCSS_SELECTORS_COMBINATOR_CHILD             = 0x02, // '>'
    MyCSS_SELECTORS_COMBINATOR_NEXT_SIBLING      = 0x03, // '+'
    MyCSS_SELECTORS_COMBINATOR_FOLLOWING_SIBLING = 0x04, // '~'
    MyCSS_SELECTORS_COMBINATOR_COLUMN            = 0x05, // '||'
    MyCSS_SELECTORS_COMBINATOR_LAST_ENTRY        = 0x06
}
typedef mycss_selectors_combinator_t;

enum mycss_selectors_mod {
    MyCSS_SELECTORS_MOD_UNDEF       = 0x00,
    MyCSS_SELECTORS_MOD_I           = 0x01
}
typedef mycss_selectors_mod_t;

enum mycss_selectors_flags {
    MyCSS_SELECTORS_FLAGS_UNDEF         = 0x00,
    MyCSS_SELECTORS_FLAGS_SELECTOR_BAD  = 0x01
}
typedef mycss_selectors_flags_t;

enum mycss_selectors_type {
    MyCSS_SELECTORS_TYPE_UNDEF             = 0x000, // tag name <div> = div in css
    MyCSS_SELECTORS_TYPE_ELEMENT           = 0x001, // tag name <div> = div in css
    MyCSS_SELECTORS_TYPE_ID                = 0x002, // #hash
    MyCSS_SELECTORS_TYPE_CLASS             = 0x003, // .class
    MyCSS_SELECTORS_TYPE_ATTRIBUTE         = 0x004, // [key=val], in html <div key="val">
    MyCSS_SELECTORS_TYPE_FUNCTION          = 0x005, // :function(...) or ::function(...)
    MyCSS_SELECTORS_TYPE_PSEUDO_CLASS      = 0x006, // :pseudo or ::pseudo
    MyCSS_SELECTORS_TYPE_LAST_ENTRY        = 0x007
}
typedef mycss_selectors_type_t;

enum mycss_selectors_sub_type {
    MyCSS_SELECTORS_SUB_TYPE_UNDEF                     = 0x000,
    MyCSS_SELECTORS_SUB_TYPE_UNKNOWN                   = 0x001,
    
    /* functions */
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_CURRENT          = 0x002,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_DIR              = 0x003,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_DROP             = 0x004,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_HAS              = 0x005,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_LANG             = 0x006,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_MATCHES          = 0x007,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NOT              = 0x008,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_CHILD        = 0x009,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_COLUMN       = 0x00a,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_CHILD   = 0x00b,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_COLUMN  = 0x00c,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_OF_TYPE = 0x00d,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_OF_TYPE      = 0x00e,
    MyCSS_SELECTORS_SUB_TYPE_FUNCTION_LAST_ENTRY       = 0x00f,
    
    /* pseudo */
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ACTIVE             = 0x010,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ANY_LINK           = 0x011,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_BLANK              = 0x012,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CHECKED            = 0x013,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CURRENT            = 0x014,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_DEFAULT            = 0x015,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_DISABLED           = 0x016,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_DROP               = 0x017,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_EMPTY              = 0x018,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ENABLED            = 0x019,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_FIRST_CHILD        = 0x01a,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_FIRST_OF_TYPE      = 0x01b,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_FOCUS              = 0x01c,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_FUTURE             = 0x01d,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_HOVER              = 0x01e,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_IN_RANGE           = 0x01f,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_INDETERMINATE      = 0x020,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_INVALID            = 0x021,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_LAST_CHILD         = 0x022,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_LAST_OF_TYPE       = 0x023,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_LINK               = 0x024,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ONLY_CHILD         = 0x025,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ONLY_OF_TYPE       = 0x026,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_OPTIONAL           = 0x027,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_OUT_OF_RANGE       = 0x028,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_PAST               = 0x029,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_PLACEHOLDER_SHOWN  = 0x02a,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_READ_ONLY          = 0x02b,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_READ_WRITE         = 0x02c,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_REQUIRED           = 0x02d,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ROOT               = 0x02e,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_SCOPE              = 0x02f,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_TARGET             = 0x030,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_USER_ERROR         = 0x031,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_VALID              = 0x032,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_VISITED            = 0x033,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_LAST_ENTRY         = 0x034
}
typedef mycss_selectors_sub_type_t;

enum mycss_selectors_function_drop_type {
    MyCSS_SELECTORS_FUNCTION_DROP_TYPE_UNDEF   = 0x000,
    MyCSS_SELECTORS_FUNCTION_DROP_TYPE_ACTIVE  = 0x001,
    MyCSS_SELECTORS_FUNCTION_DROP_TYPE_VALID   = 0x002,
    MyCSS_SELECTORS_FUNCTION_DROP_TYPE_INVALID = 0x004,
}
typedef mycss_selectors_function_drop_type_t;

struct mycss_selectors {
    // refs
    mycss_entry_t* entry;
    mycss_result_t* result;
};

struct mycss_selectors_entry {
    mycss_selectors_type_t type;
    mycss_selectors_sub_type_t sub_type;
    mycss_selectors_flags_t flags;
    
    size_t ns; /* namespace */
    
    myhtml_string_t* key;
    void* value;
    
    mycss_selectors_combinator_t combinator;
    
    mycss_selectors_entry_t* next;
    mycss_selectors_entry_t* prev;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyHTML_MyCSS_SELECTORS_MYOSI_H */
