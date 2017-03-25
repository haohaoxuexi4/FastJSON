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
#include <string.h>

#define ISDIGIT(ch) ((ch)>='0'&& (ch)<='9')
#define ISDIGIT19(ch) ((ch)>='1'&& (ch)<='9')
FastJson::FastJson(const char* json)
{
    jsoncontex.json=json;
    value_.u.s.addr=NULL;
    value_.u.s.len=0;
    value_.u.number=0.0;
    value_.type=FAST_null;
}

FastJson::~FastJson()
{
    printf("~fastjson()\n");

    
}

void FastJson::valuefree()
{
    if (value_.type==FAST_string && value_.u.s.addr!=nullptr) {
        
        delete []value_.u.s.addr;
        value_.u.s.addr=NULL;
        value_.u.s.len=0;
        value_.u.number=0.0;
        
    }
    value_.type=FAST_null;
}


void FastJson::contexfree()
{
    //jsoncontex.json=nullptr;
    jsoncontex.contexstring.clear();
    
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
    
    value_.u.number= strtod(json, NULL);
    if (errno == ERANGE && (value_.u.number == HUGE_VAL || value_.u.number == -HUGE_VAL))
        return PARSE_NUMBER_TOO_BIG;
    value_.type = FAST_number;
    json = p;
    jsoncontex.json=json;
    return PARSE_OK;

}

double FastJson::fastjson_getnumber()
{
    return value_.u.number;
}


//----------string--------///
char* FastJson::fastjson_getstring()
{

    return value_.u.s.addr;
}
int  FastJson::fastjson_getstringlen()
{
    return value_.u.s.len;
}
void FastJson::fastjson_set_string()
{
    
    value_.u.s.addr=new char [jsoncontex.contexstring.size()];
    memcpy(value_.u.s.addr, &(*jsoncontex.contexstring.begin()), jsoncontex.contexstring.size()+1);
    value_.u.s.addr[jsoncontex.contexstring.size()]='\0';          //以‘\0’结尾
    value_.u.s.len=jsoncontex.contexstring.size()+1;
    value_.type=FAST_string;
}
int FastJson::fastjson_parse_string(const char* json)
{
    assert(json!=nullptr);
    const char* p=json;
    p++;   //跳过‘“’ 字符
    jsoncontex.json++;
    char c;
    while(1)
    {
        c=*p++;
        printf("ddd%c\n",c);
    
        switch (c) {
            case '\"':
                 fastjson_set_string();
                jsoncontex.json=p;
                return PARSE_OK;
                
            case '\0':
                return 999;
            default: jsoncontex.contexstring.push_back(c);
                
                
                
        }
    }
    
    return PARSE_OK;
}
//----------array---------//


jsonValue FastJson::fastjson_getarrayone(int index)
{

    return value_.u.a.val[index];
}
int  FastJson::fastjson_getarraylen()
{

    return value_.u.a.size;
}
int FastJson::fastjson_parse_array(const char* json)
{

    printf("array\n");
    json++;// 跳过‘【’
    jsoncontex.json=json;
    if(*json==']')
    {
        //空array
        value_.u.a.val=NULL;
        value_.u.a.size=0;
        value_.type=FAST_array;
        return PARSE_OK;
    }
    
    for(;;)
    {
        int back=fastjson_parse_value(jsoncontex.json);
        if (back!=PARSE_OK) {
            return back;
        }
        //放入数组
       // int size=sizeof(jsonValue);
        //memcpy(value_.u.a.val,&value_,size);
        jsoncontex.jsonvaluecontex.push_back(value_);
        if(*jsoncontex.json==',')
        {
            printf(".........\n");
            //json++;
            jsoncontex.json++;
            //return back;
        }
        else if(*jsoncontex.json==']')
        {
            printf("]]]]]]]]\n");
        
            //结束
            value_.u.a.val=new jsonValue[jsoncontex.jsonvaluecontex.size()];
            memcpy(value_.u.a.val, &(*jsoncontex.jsonvaluecontex.begin()), sizeof(jsonValue)*jsoncontex.jsonvaluecontex.size());
            
            return back;
        }
        
        
    
    }
    //return 1;
}

//--------object--------//
int FastJson::fastjson_getobjectsize()
{
    return value_.u.o.size;
}
int FastJson::fastjson_getobjectkeysize(int index)
{
    return value_.u.o.member[index].ksize;
}
char* FastJson::fastjson_getobjectkey(int index)
{
    return value_.u.o.member[index].k;
}
jsonValue FastJson::fastjson_getobjectone(int index)
{
    return *value_.u.o.member[index].vk;
}
int FastJson::fastjson_parse_string_objectkey(const char* json,char* keystring)  //解析object  key
{
    assert(json!=nullptr);
    const char* p=json;
    p++;   //跳过‘“’ 字符
    jsoncontex.json++;
    char c;
    while(1)
    {
        c=*p++;
        printf("ddd%c\n",c);
        
        switch (c) {
            case '\"':
                jsoncontex.json=p;
                
                return 999;
                
            case '\0':
                printf("dagdadga\n");
                break;
               // return 999;
            default: jsoncontex.contexstring.push_back(c);
                
                
                
        }
    }
    
    jsoncontex.json=p;
    //return PARSE_OK;
}
int FastJson::fastjson_parse_object(const char* json)
{

    printf("object\n");
    jsoncontex.json++; //跳过‘}’
    
    if(*jsoncontex.json=='}')
    {
        //空白 object
        value_.type=FAST_object;
        value_.u.o.member->k=NULL;
        value_.u.o.member->ksize=0;
        value_.u.o.member->vk=NULL;
        value_.u.o.member=NULL;
        value_.u.o.size=0;
        return PARSE_OK;
    }
    jsonmember mem;
    char* keystring=NULL;
    for (;;) {
        
        printf("object  \n");
        //mem 初始化
        /*
        for (int i=0; i<value_.u.o.size; i++) {
            value_.u.o.member[i]
        }
        value_.u.o.member->k=NULL;
        value_.u.o.member->ksize=0;
        value_.u.o.member->vk=NULL;
        value_.u.o.member=NULL;
        //value_.u.o.size=0;
        */
        
        if (*jsoncontex.json!='"') {
            
            return  9999;
        }
        //解析  key  放入到mem中
        fastjson_parse_string_objectkey(jsoncontex.json,keystring);
        mem.k=new char[jsoncontex.contexstring.size()];
        memcpy(mem.k,&(*jsoncontex.contexstring.begin()), jsoncontex.contexstring.size());

       // mem.k=keystring;
       // mem.ksize=sizeof(keystring);
        printf("keystring=%s\n",mem.k);
        if(*jsoncontex.json==':')
        {
            jsoncontex.json++;
            //return 9999;
        }
        printf("jsoncontse.json=%s\n",jsoncontex.json);
        int back=fastjson_parse_value(jsoncontex.json);
        if (back!=PARSE_OK) {
            printf("back!=parse_ok\n");
            return back;
        }
        //得到value   放入到mem中，
        memcpy(mem.vk, &value_, sizeof(jsonValue));
        
        jsoncontex.jsonmembercontex.push_back(mem);
        
        if (*jsoncontex.json==',') {
            printf(",,,,,,,\n");
            jsoncontex.json++;
        }
        else if(*jsoncontex.json=='}')
        {
            printf("}}}}}}}}\n");
            //结束
            value_.type=FAST_object;
            value_.u.o.size=jsoncontex.jsonmembercontex.size();
            value_.u.o.member=new jsonmember[jsoncontex.jsonmembercontex.size()];
            memcpy(value_.u.o.member, &(*jsoncontex.jsonmembercontex.begin()), value_.u.o.size*sizeof(jsonValue));
            
            return PARSE_OK;
        }
        
        
    }
    return 0;
}
/////////////////////////
int FastJson::fastjson_parse()
{

    
    parse_whitespace(jsoncontex.json);
    
    printf("fasjson=%s\n",jsoncontex.json);
    int backcode=fastjson_parse_value(jsoncontex.json);
    
    if (backcode==PARSE_OK)
    {
        parse_whitespace(jsoncontex.json);
        if (jsoncontex.json[0]!='\0') {
            printf("fasjson2=%s\n",jsoncontex.json);
            return PARSE_SPACEVALUE;
        }
    }
    
    printf("fasjson3=%s\n",jsoncontex.json);
    contexfree();
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
            
        case '"': return fastjson_parse_string(fastjson);    //string
        case '[': return fastjson_parse_array(fastjson);    //array
        case '{': return fastjson_parse_object(fastjson);   //object
        default: return PARSE_INVALID;
            
    }
    
}