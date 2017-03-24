//
//  fastjson.hpp
//  fastjson
//
//  Created by 仙女 on 17/3/9.
//  Copyright © 2017年 仙女. All rights reserved.
//

#ifndef fastjson_hpp
#define fastjson_hpp

#include <stdio.h>
#include "noncopy.h"
#include <vector>

using namespace::std;
//---解析函数返回值----//
#define PARSE_OK 110
#define PARSE_ALLSPACE 111 //解析一个值时候都是空白，没有内容
#define PARSE_SPACEVALUE 112//一个值后，空白，后还有值
#define PARSE_INVALID  113 //错误的类型，
#define PARSE_NUMBER_TOO_BIG 114   //解析数字过大
//json类型，，null,(true,false),number,string,array,object
typedef enum{FAST_null,FAST_false,FAST_true,FAST_number,FAST_string,FAST_array,FAST_object} jsonType;


//json 解析的值
typedef struct{
    union{
        struct {
            char* addr;
            int len;
        }s;                             //string
        double number;                  //number;
    }u;
    jsonType type;
}jsonValue;

typedef struct{
    const  char* json;
    std::vector<char> contexstring;
    
}jsoncontex;
class FastJson:noncopy
{
public:
    FastJson(const char* json);
    ~FastJson();
    
    void  valuefree();
    void  contexfree();
    
    int fastjson_parse();
    //---通用----//
    jsonType fastjson_gettype();
    
    //---number---//
    double  fastjson_getnumber();
    
    //-----string----//
    char* fastjson_getstring();
    int  fastjson_getstringlen();
    
private:
    
    void parse_whitespace(const char* pjson);//去除掉空白(回车，空白等四种情况)，没用的片段
    
    int  fastjson_parse_value(const char* fastjson);
    
    //-----null------------//
    int fastjson_parse_null(const char* json);
    
    //-----true----------//
    int fastjson_parse_true(const char* json);
    
    //-----false---------//
    int  fastjson_parse_false(const char* json);
    
    //-----number--------//
    
    int  fastjson_parse_number(const char* json);
    //-----string------//
    
    int  fastjson_parse_string(const char* json);
    
    void fastjson_set_string();
    
    //-----array------//
   // int  fastjson_parse_array(const char* json);
    
    //-----object-----//
   // int  fastjson_parse_object(const char* json);
    
    
    jsoncontex jsoncontex;
    
    jsonValue value_;
};

#endif /* fastjson_hpp */
