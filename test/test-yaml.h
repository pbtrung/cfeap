/***************************************************************************
 * Copyright (C) 2017 Trung Pham <me@trungbpham.com>                       *           
 * All rights reserved.                                                    *           
 *                                                                         *           
 * Redistribution and use in source and binary forms, with or without      *           
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 *                                                                         *           
 *    * Redistributions of source code must retain the above copyright     *
 *    notice, this list of conditions and the following disclaimer.        *                
 *                                                                         *           
 *    * Redistributions in binary form must reproduce the above copyright  *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *                      
 *                                                                         *           
 *    * Neither the name of cfeap nor the names of its contributors may    *
 *    be used to endorse or promote products derived from this software    *
 *    without specific prior written permission.                           *                         
 *                                                                         *           
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     *
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       *   
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   *
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT    * 
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,  *  
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        *
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   *
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   * 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     * 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *   
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    *           
 ***************************************************************************/

#include <stdio.h>

#include "yaml/yaml.h"

static void test_yaml();
static void test_yaml_case_a();
static void test_yaml_case_b();

static void print_yaml_document(yaml_document_t *document);
void print_yaml_node(yaml_document_t *document, yaml_node_t *node);

static void test_yaml()
{
    test_yaml_case_a();
    test_yaml_case_b();
}

static void test_yaml_case_a()
{
    FILE *fp = fopen("../../test/test-yaml.yml", "r");
    yaml_parser_t parser;
    yaml_token_t  token;

    /* Initialize parser */
    if(!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
    }
    if(fp == NULL) {
        fputs("Failed to open file!\n", stderr);
    }

    /* Set input file */
    yaml_parser_set_input_file(&parser, fp);
    
    int state = 0;
    int count_id = 0;
    int expected_num_id = 3;
    char *tk;
    
    do {
        yaml_parser_scan(&parser, &token);
        switch(token.type) {
            case YAML_KEY_TOKEN:     
                state = 0;
                break;
            case YAML_VALUE_TOKEN:   
                state = 1; 
                break;
            case YAML_SCALAR_TOKEN:
                tk = (char *)token.data.scalar.value;
                if(state == 0) {
                    if(!strcmp(tk, "id")) {
                        count_id += 1;
                    }
                } 
                break;
           default: 
               break;
        }
        if(token.type != YAML_STREAM_END_TOKEN) {
            yaml_token_delete(&token);
        }
    } while(token.type != YAML_STREAM_END_TOKEN);
        
    sput_fail_if(count_id != expected_num_id, "Case a: Numbers of id");
    
    yaml_token_delete(&token);
    yaml_parser_delete(&parser);
    fclose(fp);
}

static void test_yaml_case_b()
{
    FILE *fp = fopen("../../test/test-yaml.yml", "r");
    yaml_parser_t parser;
    yaml_document_t document;

    /* Initialize parser */
    if(!yaml_parser_initialize(&parser)) {
        fputs("Failed to initialize parser!\n", stderr);
    }
    if(fp == NULL) {
        fputs("Failed to open file!\n", stderr);
    }

    /* Set input file */
    yaml_parser_set_input_file(&parser, fp);
    
    int done = 0;
    while (!done)
    {
        if (!yaml_parser_load(&parser, &document)) {
            break;
        }

        done = (!yaml_document_get_root_node(&document));

        if (!done)
            print_yaml_document(&document);

        yaml_document_delete(&document);
    }
    
    yaml_parser_delete(&parser);
    fclose(fp);
}

static void print_yaml_document(yaml_document_t *document)
{
    puts("NEW DOCUMENT");

    print_yaml_node(document, yaml_document_get_root_node(document));

    puts("END DOCUMENT");
}

void print_yaml_node(yaml_document_t *document, yaml_node_t *node)
{
    char *node_value;
    yaml_node_t *material;
    yaml_node_t *next_node_p;
    
    switch (node->type) {
        case YAML_NO_NODE:
            break;
        case YAML_SCALAR_NODE:
            break;
        case YAML_SEQUENCE_NODE:
            break;
        case YAML_MAPPING_NODE:;
            yaml_node_pair_t *i_node_p;
            for(i_node_p = node->data.mapping.pairs.start; i_node_p < node->data.mapping.pairs.top; i_node_p++) {
                next_node_p = yaml_document_get_node(document, i_node_p->key);
                if(next_node_p) {
                    node_value = (char *)next_node_p->data.scalar.value;
                    if(!strcmp(node_value, "material")) {
                        printf("material\n");
                        material = next_node_p;
                    }
                }
            }
            break;
        default:
            fputs("Unknown node type\n", stderr);
            break;
    }
}
