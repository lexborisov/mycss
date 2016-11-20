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

#ifndef MyCSS_DECLARATION_DEFAULT_H
#define MyCSS_DECLARATION_DEFAULT_H
#pragma once

#include <mycss/declaration/myosi.h>
#include <mycss/values/values.h>

#ifdef __cplusplus
extern "C" {
#endif

mycss_declaration_entry_t * mycss_declaration_default_by_type(mycss_property_type_t property_type);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MyCSS_DECLARATION_DEFAULT_H */
