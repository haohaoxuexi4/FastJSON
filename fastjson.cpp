//
//  fastjson.cpp
//  fastjson
//
//  Created by 仙女 on 17/3/9.
//  Copyright © 2017年 仙女. All rights reserved.
//

#include "fastjson.hpp"
#include <assert.h>
FastJson::FastJson(const char* json)
{
    jsoncontex.json=json;
    value_.type=FAST_null;
}

FastJson::~FastJson()
{

}
void FastJson::parse_whitespace(const char* pjson)//去除掉空白，没用的片段
{
    assert(pjson!=NULL);
    while (*pjson == ' ' || *pjson== '\t' || *pjson== '\n' || *pjson == '\r')
        pjson++;
    jsoncontex.json=pjson;
}

jsonType FastJson::fastjson_gettype()
{
    return value_.type;
}

//-----null------------//
int FastJson::fastjson_parse_null(const char* json)
{
    if(json[1]=='u' && json[2]=='l' && json[3]=='l')
    {
        value_.type=FAST_null;
        json+=4;
        jsoncontex.json=json;
        return PARSE_OK;
    }
    return PARSE_INVALID;
}

//-----true----------//
int FastJson::fastjson_parse_true(const char* json)
{
    if(json[1]=='r' && json[2]=='u' && json[3]=='e')
    {
        value_.type=FAST_true;
        json+=4;
        jsoncontex.json=json;
        return PARSE_OK;
    }
    return PARSE_INVALID;
}

//-----false---------//
int FastJson::fastjson_parse_false(const char* json)
{
    if(json[1]=='a' && json[2]=='l' && json[3]=='s' && json[4]=='e')
    {
        value_.type=FAST_false;
        json+=5;
        jsoncontex.json=json;
        return PARSE_OK;
    }
    return PARSE_INVALID;
}

int FastJson::fastjson_parse()
{

    
    parse_whitespace(jsoncontex.json);
    
    printf("fasjson=%s\n",jsoncontex.json);
    int backcode=fastjson_parse_value(jsoncontex.json);
    
    if (backcode==110)
    {
        parse_whitespace(jsoncontex.json);
        if (jsoncontex.json[0]!='\0') {
            printf("fasjson2=%s\n",jsoncontex.json);
            return PARSE_SPACEVALUE;
        }
    }
    
    printf("fasjson3=%s\n",jsoncontex.json);
    return backcode;
}
int FastJson::fastjson_parse_value(const char* fastjson)
{
    assert(fastjson!=NULL);
    switch(fastjson[0])
    {
        case 'n': return fastjson_parse_null(fastjson);
        case 'f': return fastjson_parse_false(fastjson);
        case 't':  return fastjson_parse_true(fastjson);
        case '\0':return PARSE_ALLSPACE;
        default:return PARSE_INVALID;
            
    }
    
}