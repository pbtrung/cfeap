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

static void test_yaml()
{
    test_yaml_case_a();
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
    int found_id = 0;
    char *tk;
    
    do {
        yaml_parser_scan(&parser, &token);
        switch(token.type) {
            case YAML_KEY_TOKEN:     
                state = 0; 
                found_id = 0;
                break;
            case YAML_VALUE_TOKEN:   
                state = 1; 
                break;
            case YAML_SCALAR_TOKEN:
                tk = (char *)token.data.scalar.value;
                if(state == 0) {
                    if(!strcmp(tk, "id")) {
                        found_id = 1;
                    }
                } else {
                    if(found_id == 1) {
                        printf("Found id: %s\n", tk);                        
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
    
    yaml_token_delete(&token);
    yaml_parser_delete(&parser);
    fclose(fp);
}
