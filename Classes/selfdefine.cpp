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
    char user_device_name[20];
    memset(user_device_name, 0, 20);
    sprintf(user_device_name, "%d", user_device_id);
    
    std::string timestamp = "1390013900";
    std::string interface = "UploadCommitSearch";
    
    rapidjson::Document document;
    document.SetObject();
    Document::AllocatorType& allocator = document.GetAllocator();
    StringBuffer buffer;
    rapidjson::Writer<StringBuffer> writer(buffer);
    
    // combine data -> {user_id} -> buffer
    document.AddMember("user_id", user_device_name, allocator);
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

std::vector< std::map<std::string, std::string> > Leader_Data::get_leader_detail(const char* input_data)
{
    log("input_data => %s", input_data);
    // return result
    std::vector< std::map<std::string, std::string> > result;
    
    rapidjson::Document d;
    d.Parse<0>(input_data);
    
    // 判断到接收的内容是否有错
    if (d.HasParseError()) {
        return result;
    }
    // 判断接受code是否有问题
    if ( get_incode(input_data) != 200 ) {
        return result;
    }
    
    // 正式分析内容
    if ( d.IsObject() && d.HasMember("dataObject")) {
        // 解析第一层 dataObject
        rapidjson::Value &dataObject = d["dataObject"];
        if (dataObject.IsObject() && dataObject.HasMember("details")) {
            // 解析第二层 details
            rapidjson::Value &detail = dataObject["details"];
            if (detail.IsArray()) {
                
                // 实际分析内容
                for (int i = 0; i<detail.Size(); i++) {
                    std::map<std::string, std::string> tmp_user;
                    
                    // 获取每一项内容
                    rapidjson::Value &tmp = detail[i];
                    rapidjson::Document tinydic;
                    tinydic.Parse<0>(tmp.GetString());
                    
                    // 实际的解析每一项内容
                    // 解析排行榜
                    if (tinydic.IsObject() && tinydic.HasMember("rank")) {
                        tmp_user["rank"] = tinydic["rank"].GetString();
                    }
                    if (tinydic.IsObject() && tinydic.HasMember("tmp_user_id")) {
                         tmp_user["tmp_user_id"] = tinydic["tmp_user_id"].GetString();
                    }
                    if (tinydic.IsObject() && tinydic.HasMember("tmp_user_score")) {
                        tmp_user["tmp_user_score"] = tinydic["tmp_user_score"].GetString();
                    }
                    // 解析用户自身
                    if (tinydic.IsObject() && tinydic.HasMember("self_id")) {
                        tmp_user["self_id"] = tinydic["self_id"].GetString();
                    }
                    if (tinydic.IsObject() && tinydic.HasMember("self_rank")) {
                        tmp_user["self_rank"] = tinydic["self_rank"].GetString();
                    }
                    result.push_back(tmp_user);
                }
                return result;
            }
        }

    }
    return result;
}