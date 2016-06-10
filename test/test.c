/*
 Copyright (C) 2015-2016 Alexander Borisov
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 
 Author: lex.borisov@gmail.com (Alexander Borisov)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>

#include <myhtml/myhtml.h>
#include <myhtml/myosi.h>
#include <myhtml/mystring.h>

#include <mycss/mycss.h>
#include <mycss/myosi.h>
#include <mycss/selectors/myosi_resource.h>
#include <mycss/selectors/init.h>
#include <mycss/namespace/init.h>
#include <mycss/result.h>
#include <mycss/selectors/value.h>

struct res_html {
    char  *html;
    size_t size;
};

struct res_html load_html_file(const char* filename)
{
    FILE *fh = fopen(filename, "rb");
    if(fh == NULL) {
        fprintf(stderr, "Can't open html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    fseek(fh, 0L, SEEK_END);
    long size = ftell(fh);
    fseek(fh, 0L, SEEK_SET);
    
    char *html = (char*)malloc(size + 1);
    if(html == NULL) {
        fprintf(stderr, "Can't allocate mem for html file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    
    size_t nread = fread(html, 1, size, fh);
    if (nread != size) {
        fprintf(stderr, "could not read %ld bytes (%zu bytes done)\n", size, nread);
        exit(EXIT_FAILURE);
    }
    
    fclose(fh);
    
    if(size < 0)
        size = 0;
        
        struct res_html res = {html, (size_t)size};
        return res;
}

void test_print_selector_value_attribute(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, myhtml_string_t* str)
{
    myhtml_string_append(str, " ns=\"", strlen(" ns=\""));
    if(selector->ns) {
        size_t length;
        const char *name = mycss_namespace_name_by_id(selectors->result->ns, selector->ns, &length);
        
        if(length)
            myhtml_string_append(str, name, length);
    }
    myhtml_string_append(str, "\"", strlen("\""));
    
    if(selector->value == NULL) {
        myhtml_string_append(str, ">\n", 2);
        return;
    }
    
    if(mycss_selector_value_attribute(selector->value)->value)
    {
        myhtml_string_append(str, " value=\"", strlen(" value=\""));
        myhtml_string_append(str, mycss_selector_value_attribute(selector->value)->value->data,
                             mycss_selector_value_attribute(selector->value)->value->length);
        myhtml_string_append(str, "\"", strlen("\""));
    }
    
    if(mycss_selector_value_attribute(selector->value)->mod)
    {
        myhtml_string_append(str, " mod=\"", strlen(" mod=\""));
        if(mycss_selector_value_attribute(selector->value)->mod)
            myhtml_string_append(str, "i", 1);
        myhtml_string_append(str, "\"", strlen("\""));
    }
    
    myhtml_string_append(str, ">\n", 2);
}

void test_print_selector_value_element(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, myhtml_string_t* str)
{
    myhtml_string_append(str, " ns=\"", strlen(" ns=\""));
    if(selector->ns) {
        size_t length;
        const char *name = mycss_namespace_name_by_id(selectors->result->ns, selector->ns, &length);
        
        if(length)
            myhtml_string_append(str, name, length);
    }
    myhtml_string_append(str, "\"", strlen("\""));
    
    myhtml_string_append(str, ">\n", 2);
}

void test_print_selector(mycss_selectors_t* selectors, mycss_selectors_entry_t* selector, myhtml_string_t* str, bool is_last)
{
    myhtml_string_append(str, "<selector type=\"", strlen("<selector type=\""));
    
    switch(selector->type) {
        case MyCSS_SELECTORS_TYPE_ELEMENT: {
            myhtml_string_append(str, "element", strlen("element"));
            break;
        }
        case MyCSS_SELECTORS_TYPE_ATTRIBUTE: {
            myhtml_string_append(str, "attribute", strlen("attribute"));
            break;
        }
        case MyCSS_SELECTORS_TYPE_ID: {
            myhtml_string_append(str, "id", strlen("id"));
            break;
        }
        case MyCSS_SELECTORS_TYPE_CLASS: {
            myhtml_string_append(str, "class", strlen("class"));
            break;
        }
        default: {
            myhtml_string_append(str, "undef", strlen("undef"));
            break;
        }
    };
    myhtml_string_append(str, "\"", strlen("\""));
    
    if(selector->key && selector->key->length) {
        myhtml_string_append(str, " key=\"", strlen(" key=\""));
        myhtml_string_append(str, selector->key->data, selector->key->length);
        myhtml_string_append(str, "\"", strlen("\""));
    }
    else
        myhtml_string_append(str, " key=\"\"", strlen(" key=\"\""));
    
    myhtml_string_append(str, " comb=\"", strlen(" comb=\""));
    myhtml_string_append(str, mycss_selectors_resource_combinator_names_map[ selector->combinator ],
                         strlen(mycss_selectors_resource_combinator_names_map[ selector->combinator ]));
    myhtml_string_append(str, "\"", strlen("\""));
    
    switch(selector->type) {
        case MyCSS_SELECTORS_TYPE_ELEMENT:
            test_print_selector_value_element(selectors, selector, str);
            break;
        case MyCSS_SELECTORS_TYPE_CLASS:
            test_print_selector_value_element(selectors, selector, str);
            break;
        case MyCSS_SELECTORS_TYPE_ID:
            test_print_selector_value_element(selectors, selector, str);
            break;
        case MyCSS_SELECTORS_TYPE_ATTRIBUTE:
            test_print_selector_value_attribute(selectors, selector, str);
            break;
            
        case MyCSS_SELECTORS_TYPE_FUNCTION:
            myhtml_string_append(str, ">\n", 2);
            break;
            
        default:
            myhtml_string_append(str, ">\n", 2);
            break;
    }
}

void test_result_entry_print(mycss_result_t* result, mycss_result_entry_t* res_entry, myhtml_string_t* str)
{
    while(res_entry) {
        for(size_t i = 0; i < res_entry->selector_list_length; i++) {
            
            mycss_selectors_entry_t* selector = res_entry->selector_list[i];
            
            while(selector) {
                test_print_selector(result->selectors, selector, str, (selector->next == NULL));
                selector = selector->next;
            }
            
            if((i + 1) != res_entry->selector_list_length)
                myhtml_string_append(str, ", ", 2);
        }
        
        if(res_entry->next)
            myhtml_string_append(str, "\n", 1);
        
        res_entry = res_entry->next;
    }
}

const char * test_node_attr_value(myhtml_tree_node_t *node, const char* key, myhtml_string_t* str)
{
    myhtml_tree_attr_t *attr = myhtml_attribute_by_key(node, key, strlen(key));
    
    if(attr == NULL)
        return NULL;
    
    myhtml_string_append(str, " ", 1);
    myhtml_string_append(str, key, strlen(key));
    
    if(attr->value_length && attr->entry.data) {
        myhtml_string_append(str, "=\"", 2);
        myhtml_string_append(str, &attr->entry.data[attr->value_begin], attr->value_length);
        myhtml_string_append(str, "\"", 1);
        
        return attr->entry.data;
    }
    
    myhtml_string_append(str, "=\"\"", 3);
    
    return NULL;
}

void test_print_tag_selector(myhtml_tree_node_t* node, myhtml_string_t* str, bool is_last)
{
    myhtml_string_append(str, "<selector", strlen("<selector"));
    
    test_node_attr_value(node, "type", str);
    test_node_attr_value(node, "key", str);
    test_node_attr_value(node, "comb", str);
    test_node_attr_value(node, "ns", str);
    test_node_attr_value(node, "value", str);
    test_node_attr_value(node, "mod", str);
    
    myhtml_string_append(str, ">\n", 2);
}

bool test_cmp(mycss_result_t *css_result, myhtml_tree_t* tree, myhtml_tree_node_t *node, myhtml_string_t* css_res, myhtml_string_t* res_res)
{
    myhtml_collection_t *collection = myhtml_get_nodes_by_name_in_scope(tree, NULL, node, "selector", 8, NULL);
    
    test_result_entry_print(css_result, css_result->result_entry_first, css_res);
    
    for(size_t i = 0; i < collection->length; i++) {
        test_print_tag_selector(collection->list[i], res_res, ((i+1) == collection->length));
    }
    
    myhtml_collection_destroy(collection);
    
    return true;
}

size_t test_data(myhtml_tree_t* tree, size_t count_of_files, size_t* bad_res)
{
    myhtml_collection_t* collection = myhtml_get_nodes_by_name(tree, NULL, "entry", 5, NULL);
    
    myhtml_string_t css_res, res_res;
    myhtml_string_init(tree->mchar, tree->mchar_node_id, &css_res, 9048);
    myhtml_string_init(tree->mchar, tree->mchar_node_id, &res_res, 9048);
    
    mycss_t *mycss = mycss_create();
    mycss_status_t status = mycss_init(mycss);
    
    mycss_entry_t *entry = mycss_entry_create();
    status = mycss_entry_init(mycss, entry);
    
    size_t count = 0;
    
    for(size_t i = 0; i < collection->length; i++)
    {
        myhtml_collection_t *col_data = myhtml_get_nodes_by_name_in_scope(tree, NULL, collection->list[i], "data", 4, NULL);
        
        myhtml_tree_node_t *data_node = col_data->list[0]->child;
        myhtml_string_t *data_str = &data_node->token->my_str_tm;
        
        mycss_parse(entry, MyHTML_ENCODING_UTF_8, data_str->data, data_str->length);
        
        test_cmp(entry->result, tree, collection->list[i], &css_res, &res_res);
        
        mycss_result_destroy(entry->result, true);
        
        count++;
        printf("\t%zu: ", count);
        if(css_res.length != res_res.length || strncmp(css_res.data, res_res.data, res_res.length)) {
            printf("bad\n");
            
            printf("\tAfter Parsing:\n");
            printf("%s\n\n", css_res.data);
            
            printf("\tExpected Result:\n");
            printf("%s\n\n", res_res.data);
            
            (*bad_res)++;
        }
        else
            printf("good\n");
        
        myhtml_string_clean(&css_res);
        myhtml_string_clean(&res_res);
        myhtml_collection_destroy(col_data);
    }
    
    myhtml_collection_destroy(collection);
    
    mycss_entry_destroy(entry, true);
    mycss_destroy(mycss, true);
    
    return count;
}

size_t test_dir(const char* dir_path, const char* test_name, size_t *bad_count)
{
    setbuf(stdout, NULL);
    
    *bad_count = 0;
    
    printf("Test: %s\n", test_name);
    
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    
    char path[(4096 * 4)];
    char full_dir_path[(4096 * 4)];
    
    sprintf(full_dir_path, "%s/%s", dir_path, test_name);
    sprintf(path, "%s", full_dir_path);
    
    size_t dir_path_len = strlen(full_dir_path);
    size_t count_files = 0;
    
    DIR *dir;
    struct dirent *ent;
    struct stat path_stat;
    
    size_t count_test = 0;
    size_t count_test_bad = 0;
    
    if((dir = opendir(full_dir_path)) != NULL)
    {
        while((ent = readdir(dir)) != NULL)
        {
            sprintf(&path[dir_path_len], "/%s", ent->d_name);
            
            stat(path, &path_stat);
            
            if(ent->d_name[0] == '.' || S_ISDIR(path_stat.st_mode))
                continue;
            
            struct res_html res = load_html_file(path);
            
            count_files++;
            printf("%zu) %s:\n", count_files, ent->d_name);
            
            myhtml_parse(tree, MyHTML_ENCODING_UTF_8, res.html, res.size);
            
            size_t bad = 0;
            count_test += test_data(tree, count_files, &bad);
            count_test_bad += bad;
            
            free(res.html);
        }
        
        closedir (dir);
    }
    
    printf("\tTests count: %zu; Good: %zu; Bad: %zu\n", count_test, (count_test - count_test_bad), count_test_bad);
    
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
    
    (*bad_count) = count_test_bad;
    return count_test;
}

int main(int argc, const char * argv[])
{
    if (argc < 3) {
        printf("Bad ARGV!\nUse: test <path_to_dir_test> <test_name> [ <test_name>]*\n");
        exit(EXIT_FAILURE);
    }
    
    printf("\nDirectory: %s\n", argv[1]);
    
    size_t bad_count = 0;
    size_t all_count = 0;
    
    for(size_t i = 2; i < argc; i++) {
        all_count += test_dir(argv[1], argv[i], &bad_count);
    }
    
//    all_count += test_dir("/new/C-git/mycss/test", "Selectors", &bad_count);
    
    printf("\nTotal count: %zu; Good: %zu; Bad: %zu\n", all_count, (all_count - bad_count), bad_count);
    
    return (bad_count ? EXIT_FAILURE : EXIT_SUCCESS);
}




