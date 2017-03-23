//
//  fastjson.cpp
//  fastjson
//
//  Created by 仙女 on 17/3/9.
//  Copyright © 2017年 仙女. All rights reserved.
//

#include "fastjson.hpp"
#include <assert.h>
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */


#define ISDIGIT(ch) ((ch)>='0'&& (ch)<='9')
#define ISDIGIT19(ch) ((ch)>='1'&& (ch)<='9')
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
//-------number----//
int FastJson::fastjson_parse_number(const char* json)
{
    printf("number parse=%s\n",json);
    assert(json!=NULL);
    const char* p=json;
    //去除杂质，除去不合法的数字表达形式
    if (*p == '-') p++;
    if (*p == '0') p++;
    else {
        if (!ISDIGIT19(*p))  return PARSE_INVALID;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == '.') {
        
        p++;
        if (!ISDIGIT(*p)) return PARSE_INVALID;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p)) return PARSE_INVALID;
        for (p++; ISDIGIT(*p); p++);
    }
    errno = 0;
    value_.number= strtod(json, NULL);
    if (errno == ERANGE && (value_.number == HUGE_VAL || value_.number == -HUGE_VAL))
        return PARSE_NUMBER_TOO_BIG;
    value_.type = FAST_number;
    json = p;
    return PARSE_OK;

}

double FastJson::fastjson_getnumber()
{
    return value_.number;
}


//----------string--------///

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
        case '0':return fastjson_parse_number(fastjson);
        case '1':return fastjson_parse_number(fastjson);
        case '2':return fastjson_parse_number(fastjson);
        case '3':return fastjson_parse_number(fastjson);
        case '4':return fastjson_parse_number(fastjson);
        case '5':return fastjson_parse_number(fastjson);
        case '6':return fastjson_parse_number(fastjson);
        case '7':return fastjson_parse_number(fastjson);
        case '8':return fastjson_parse_number(fastjson);
        case '9':return fastjson_parse_number(fastjson);
            
      //  case '"': return fastjson_parse_string(fastjson);  //string
        //case '[': return fastjson_parse_array(fastjson);    //array
      //  case '{': return fastjson_parse_object(fastjson);   //object
        default: return PARSE_INVALID;
            
    }
    
}