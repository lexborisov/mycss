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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mycss/mycss.h>
#include <mycss/declaration/init.h>

int main(int argc, const char * argv[])
{
    const char *declaration = "width : 100%; height: 1.2em";
    
    // basic init
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    // current entry work init
    mycss_entry_t *entry = mycss_entry_create();
    status = mycss_entry_init(mycss, entry);
    
    // parse selectors
    fprintf(stdout, "Input data:\n");
    fprintf(stdout, "\t%s", declaration);
    fprintf(stdout, "\n");
    
    mycss_status_t out_status;
    mycss_declaration_entry_t *dec_entry = mycss_declaration_parse(entry->declaration, MyHTML_ENCODING_UTF_8,
                                                                   declaration, strlen(declaration), &out_status);
    
    /* print result */
    fprintf(stdout, "Result:\n\t");
    mycss_declaration_entries_print(entry->declaration, dec_entry, stdout);
    fprintf(stdout, "\n");
    
    // destroy all
    mycss_entry_destroy(entry, true);
    mycss_destroy(mycss, true);
    
    return 0;
}



