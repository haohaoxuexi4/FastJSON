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
   
 
    const char* json="{\"title\": \"Design Patterns\",\"subtitle\": \"Elements of Reusable Object-Oriented Software\",\"author\": [\"Erich Gamma\",\"Richard Helm\",\"Ralph Johnson\",\"John Vlissides\"],\"year\": 2009,\"weight\": 1.8,\"hardcover\": true,\"publisher\": {\"Company\": \"Pearson Education\",\"Country\": \"India\"},\"website\": null}";
    /*
    FastJson json3("{\"title\": \"Design Patterns\",\"subtitle\": \"Elements of Reusable Object-Oriented Software\",\"author\": [\"Erich Gamma\",\"Richard Helm\",\"Ralph Johnson\",\"John Vlissides\"],\"year\": 2009,\"weight\": 1.8,\"hardcover\": true}");
     */
    FastJson json3(json);
    json3.fastjson_parse();
    
    int size=json3.fastjson_getobjectsize();
    printf("size=%d\n",size);
    for (int i=0; i<size; i++) {
        printf("i=%d,key=\"%s\",keysize=%d\n",i,json3.fastjson_getobjectkey(i),json3.fastjson_getobjectkeysize(i));
        jsonValue v=json3.fastjson_getobjectone(i);
        printf("value tyep=%d\n",v.type);
    }
    //printf("man stirng =%s",json3.fastjson_getobjectkey(0));
    
    json3.valuefree();
    
    return 0;
}
