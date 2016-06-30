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

#include <mycss/myosi.h>
#include <mycss/mystring.h>

typedef bool (*mycss_selectors_state_f)(mycss_result_t* result, mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, mycss_token_t* token);
typedef void (*mycss_callback_selector_done_f)(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector);

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
    MyCSS_SELECTORS_TYPE_UNDEF                   = 0x000, // tag name <div> = div in css
    MyCSS_SELECTORS_TYPE_ELEMENT                 = 0x001, // tag name <div> = div in css
    MyCSS_SELECTORS_TYPE_ID                      = 0x002, // #hash
    MyCSS_SELECTORS_TYPE_CLASS                   = 0x003, // .class
    MyCSS_SELECTORS_TYPE_ATTRIBUTE               = 0x004, // [key=val], in html <div key="val">
    MyCSS_SELECTORS_TYPE_PSEUDO_CLASS_FUNCTION   = 0x005, // :function(...)
    MyCSS_SELECTORS_TYPE_PSEUDO_CLASS            = 0x006, // :pseudo
    MyCSS_SELECTORS_TYPE_PSEUDO_ELEMENT_FUNCTION = 0x007, // ::function(...)
    MyCSS_SELECTORS_TYPE_PSEUDO_ELEMENT          = 0x008, // ::pseudo
    MyCSS_SELECTORS_TYPE_LAST_ENTRY              = 0x009
}
typedef mycss_selectors_type_t;

enum mycss_selectors_sub_type {
    MyCSS_SELECTORS_SUB_TYPE_UNDEF   = 0x000,
    MyCSS_SELECTORS_SUB_TYPE_UNKNOWN = 0x001,
    
    /* pseudo classes functions */
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_CURRENT          = 0x0002,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_DIR              = 0x0003,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_DROP             = 0x0004,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_HAS              = 0x0005,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_LANG             = 0x0006,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_MATCHES          = 0x0007,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NOT              = 0x0008,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_CHILD        = 0x0009,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_COLUMN       = 0x000a,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_CHILD   = 0x000b,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_COLUMN  = 0x000c,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_LAST_OF_TYPE = 0x000d,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_NTH_OF_TYPE      = 0x000e,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUNCTION_LAST_ENTRY       = 0x000f,
    
    /* pseudo classes */
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_ACTIVE                    = 0x0040,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_ANY_LINK                  = 0x0041,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_BLANK                     = 0x0042,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_CHECKED                   = 0x0043,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_CURRENT                   = 0x0044,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_DEFAULT                   = 0x0045,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_DISABLED                  = 0x0046,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_DROP                      = 0x0047,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_EMPTY                     = 0x0048,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_ENABLED                   = 0x0049,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FIRST_CHILD               = 0x004a,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FIRST_OF_TYPE             = 0x004b,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FOCUS                     = 0x004c,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_FUTURE                    = 0x004d,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_HOVER                     = 0x004e,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_IN_RANGE                  = 0x004f,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_INDETERMINATE             = 0x0050,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_INVALID                   = 0x0051,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_LAST_CHILD                = 0x0052,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_LAST_OF_TYPE              = 0x0053,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_LINK                      = 0x0054,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_ONLY_CHILD                = 0x0055,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_ONLY_OF_TYPE              = 0x0056,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_OPTIONAL                  = 0x0057,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_OUT_OF_RANGE              = 0x0058,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_PAST                      = 0x0059,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_PLACEHOLDER_SHOWN         = 0x005a,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_READ_ONLY                 = 0x005b,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_READ_WRITE                = 0x005c,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_REQUIRED                  = 0x005d,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_ROOT                      = 0x005e,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_SCOPE                     = 0x005f,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_TARGET                    = 0x0060,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_USER_ERROR                = 0x0061,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_VALID                     = 0x0062,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_VISITED                   = 0x0063,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_CLASS_LAST_ENTRY                = 0x0064,
    
    /* pseudo element */
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_AFTER                   = 0x1400,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_BACKDROP                = 0x1401,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_BEFORE                  = 0x1402,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_FIRST_LETTER            = 0x1403,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_FIRST_LINE              = 0x1404,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_GRAMMAR_ERROR           = 0x1405,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_MARKER                  = 0x1406,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_PLACEHOLDER             = 0x1407,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_SELECTION               = 0x1408,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_SPELLING_ERROR          = 0x1409,
    MyCSS_SELECTORS_SUB_TYPE_PSEUDO_ELEMENT_LAST_ENTRY              = 0x140a,
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
