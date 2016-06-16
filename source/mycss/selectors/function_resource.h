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

#ifndef MyHTML_MyCSS_SELECTORS_FUNCTION_RESOURCE_H
#define MyHTML_MyCSS_SELECTORS_FUNCTION_RESOURCE_H
#pragma once

static const mycss_selectors_function_index_t mycss_selectors_function_parser_map_by_sub_type[] = {
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_UNDEF
    {mycss_selectors_unknown_parser, mycss_selectors_unknown_parser}, // MyCSS_SELECTORS_SUB_TYPE_UNKNOWN
    {mycss_selectors_function_not_or_matches_parser, mycss_selectors_function_not_or_matches_parser}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_CURRENT
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_DIR
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_DROP
    {mycss_selectors_function_has_parser, mycss_selectors_function_has_parser}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_HAS
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_LANG
    {mycss_selectors_function_not_or_matches_parser, mycss_selectors_function_not_or_matches_parser}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_MATCHES
    {mycss_selectors_function_not_or_matches_parser, mycss_selectors_function_not_or_matches_parser}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NOT
    {mycss_selectors_function_nth_parser, mycss_selectors_function_nth_parser}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_CHILD
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_COLUMN
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_CHILD
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_COLUMN
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_LAST_OF_TYPE
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_FUNCTION_NTH_OF_TYPE
    {mycss_parser_token, mycss_parser_token}, // MyCSS_SELECTORS_SUB_TYPE_UNDEF
    {NULL, NULL}
};

static const mycss_selectots_function_begin_entry_t mycss_selectors_function_begin_map_index[] =
{
    {NULL, 0, NULL, 0, 0},
    {"dir", 3, mycss_selectors_function_begin_dir, 0, 1},
    {NULL, 0, NULL, 0, 0},
    {"nth-of-type", 11, mycss_selectors_function_begin_nth_of_type, 0, 3},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"nth-last-column", 15, mycss_selectors_function_begin_nth_last_column, 0, 13},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"current", 7, mycss_selectors_function_begin_current, 0, 19},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"matches", 7, mycss_selectors_function_begin_matches, 0, 23},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"has", 3, mycss_selectors_function_begin_has, 0, 28},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"not", 3, mycss_selectors_function_begin_not, 0, 34},
    {"nth-last-of-type", 16, mycss_selectors_function_begin_nth_last_of_type, 0, 35},
    {NULL, 0, NULL, 0, 0},
    {"lang", 4, mycss_selectors_function_begin_lang, 0, 37},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"nth-last-child", 14, mycss_selectors_function_begin_nth_last_child, 0, 44},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"nth-column", 10, mycss_selectors_function_begin_nth_column, 0, 47},
    {NULL, 0, NULL, 0, 0},
    {"nth-child", 9, mycss_selectors_function_begin_nth_child, 0, 49},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {NULL, 0, NULL, 0, 0},
    {"drop", 4, mycss_selectors_function_begin_drop, 0, 56},
    {NULL, 0, NULL, 0, 0},
};

#endif /* MyHTML_MyCSS_SELECTORS_FUNCTION_RESOURCE_H */

