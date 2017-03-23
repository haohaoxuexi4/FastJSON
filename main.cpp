//
//  main.cpp
//  fastjson
//
//  Created by 仙女 on 17/3/9.
//  Copyright © 2017年 仙女. All rights reserved.
//

#include <iostream>
#include "fastjson.hpp"
int main(int argc, const char * argv[]) {
    // insert code here...
    
    FastJson json(" 34.88e3");
    //json.
    json.fastjson_parse();
    printf("null type=%d\n",json.fastjson_gettype());
    printf("%f\n",json.fastjson_getnumber());
    //json.fastjson_parse("false n");
   // printf("null type=%d\n",json.fastjson_gettype());

    return 0;
}
