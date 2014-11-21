//
//  selfdefine.h
//  gogocrush
//
//  Created by Computer on 14/11/20.
//
//

#ifndef __gogocrush__selfdefine__
#define __gogocrush__selfdefine__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <map>

#include "cocos2d.h"

namespace Self_Define {
    class Leader_Data
    {
    public:
        Leader_Data() {};               // No-op initialized
        virtual ~Leader_Data() {};      // No-op destructor
        
        /* 获得服务端的执行返回值 <通用> */
        int get_incode(const char*);
        
        /* 用户上传积分 : 上传内容处理 */
        std::string get_data(std::string, int, std::string, std::string);
        
        /* 用户获取最新排名情况 : 上传内容处理 */
        std::string get_leader_all();
        
        /* 用户获取最新排名情况 : 返回内容处理 */
        std::vector< std::map<std::string, std::string> > get_leader_detail(const char*);
    };
};


#endif /* defined(__gogocrush__selfdefine__) */
