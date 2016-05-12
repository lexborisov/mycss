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

#include "myhtml/myhtml.h"
#include "mycss/mycss.h"

struct res_css {
    char *html;
    size_t size;
};

struct res_css load_css(const char* filename)
{
    FILE *f = fopen(filename, "rb");
    fseek(f, 0L, SEEK_END);
    
    long l = ftell(f);
    fseek(f, 0L, SEEK_SET);
    
    char *html = (char*)mymalloc(l);
    fread(html, 1, l, f);
    
    fclose(f);
    
    struct res_css res = {html, (size_t)l};
    return res;
}

int main(int argc, const char * argv[]) {
    // work data
    char *css = "\\72gba(0, 0, 0, 0.1);";
    
    //const char *path = "/new/C-git/test.css";
    //const char *path = "/new/C-git/bootstrap.css";
    const char *path = "/new/C-git/bootstrap_cp1251.css";
    
    struct res_css res = load_css(path);
    
    // base init
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    // currenr entry work init
    mycss_entry_t *entry = mycss_entry_create();
    status = mycss_entry_init(mycss, entry);
    
    uint64_t parse_start = myhtml_hperf_clock(NULL);
    
    for(size_t f = 0; f < 1; ++f) {
        //mycss_parse(entry, MyHTML_ENCODING_UTF_8, res.html, res.size);
        mycss_parse(entry, MyHTML_ENCODING_UTF_8, css, strlen(css));
    }
    
//    mycss_tokenizer_chunk(entry, css, strlen(css));
//    mycss_parse_chunk_end(entry);
    
    uint64_t parse_stop = myhtml_hperf_clock(NULL);
    
    printf("\n------------\n\nInformation:\n");
    printf("Timer (%llu ticks/sec):\n", (unsigned long long) myhtml_hperf_res(NULL));
    myhtml_hperf_print("\tParse css", parse_start, parse_stop, stdout);
    
    // release resurces
    mycss_entry_destroy(entry, true);
    mycss_destroy(mycss, true);
    
    return 0;
}


