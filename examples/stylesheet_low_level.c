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

int main(int argc, const char * argv[])
{
    const char *css =
"@namespace html \"http://www.w3.org/1999/xhtml\";\n\
div > html|span:has(~ [class~=best i]) {width: 30%; height: 120.46px}\n\
:not(h1, h2, h3, h4, h5) {}\n\
body > div[id*=mu i]:not(:nth-child(2n+1)) {width: 1.4%}";
    
    // basic init
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    // current entry work init
    mycss_entry_t *entry = mycss_entry_create();
    status = mycss_entry_init(mycss, entry);
    
    // parse selectors
    fprintf(stdout, "Input data:\n");
    fprintf(stdout, "%s", css);
    fprintf(stdout, "\n\n");
    
    status = mycss_parse(entry, MyHTML_ENCODING_UTF_8, css, strlen(css));
    
    /* print result */
    fprintf(stdout, "Result:\n");
    mycss_namespace_print_stylesheet(&entry->stylesheet->ns_stylesheet, stdout);
    mycss_stylesheet_print(entry->stylesheet, stdout);
    fprintf(stdout, "\n");
    
    // destroy all
    mycss_entry_destroy(entry, true);
    mycss_destroy(mycss, true);
    
    return 0;
}


