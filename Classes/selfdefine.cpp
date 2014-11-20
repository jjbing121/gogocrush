//
//  selfdefine.cpp
//  gogocrush
//
//  Created by Computer on 14/11/20.
//
//
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "selfdefine.h"
#include "base/base64.h"
#include "md5.h"

using namespace cocos2d;
using namespace Self_Define;
using namespace rapidjson;

std::string Leader_Data::get_data(std::string timestamp, int user_score, std::string user_id, std::string interface)
{
    std::string returnbuffer;
    
    rapidjson::Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    
    // combine data -> {user_score, user_id} -> buffer
    document.AddMember("user_score", user_score, allocator);
    document.AddMember("user_id", user_id.c_str(), allocator);
    document.Accept(writer);
    
    // all data string
    std::string before_md5 = timestamp+"+"+buffer.GetString()+"+"+interface;
    
    // calc md5
    MD5 md5(before_md5);
    std::string end_md5 = md5.md5();
    
    // combine all right data
    char before_base64[2000];
    memset(before_base64, 0, 2000);
    sprintf(before_base64, "timestamp=%s&data=%s&auth=%s", timestamp.c_str(), buffer.GetString(), end_md5.c_str());
    
    char *result;
    result = (char*)malloc(sizeof(char)*2000);
    memset(result, 0, sizeof(char)*2000);
    
    cocos2d::base64Encode((const unsigned char*)before_base64, strlen(before_base64), &result);
    returnbuffer = result;
    free(result);
    
    return returnbuffer;
}

int Leader_Data::get_incode(const char* returndata)
{
    rapidjson::Document d;
    d.Parse<0>(returndata);
    if (d.IsObject() && d.HasMember("incode")) {
        return atoi(d["incode"].GetString());
    }
    return 0;
}

std::string Leader_Data::get_leader_all()
{
    std::string returnbuffer;
    int user_device_id = UserDefault::getInstance()->getIntegerForKey("device_random_name");
    std::string timestamp = "1390013900";
    std::string interface = "UploadCommitSearch";
    
    rapidjson::Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    
    
    
    // combine data -> {user_id} -> buffer
    document.AddMember("user_id", user_device_id, allocator);
    document.Accept(writer);
    
    // all data string
    std::string before_md5 = timestamp+"+"+buffer.GetString()+"+"+interface;
    
    // calc md5
    MD5 md5(before_md5);
    std::string end_md5 = md5.md5();
    
    // combine all right data
    char before_base64[2000];
    memset(before_base64, 0, 2000);
    sprintf(before_base64, "timestamp=%s&data=%s&auth=%s", timestamp.c_str(), buffer.GetString(), end_md5.c_str());
    
    char *result;
    result = (char*)malloc(sizeof(char)*2000);
    memset(result, 0, sizeof(char)*2000);
    
    cocos2d::base64Encode((const unsigned char*)before_base64, strlen(before_base64), &result);
    returnbuffer = result;
    free(result);
    
    return returnbuffer;
}