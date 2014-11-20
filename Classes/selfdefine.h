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
        
        /* 获得服务端的执行返回值 */
        int get_incode(const char*);
        
        /* 用户上传积分 */
        std::string get_data(std::string, int, std::string, std::string);
        
        /* 用户获取最新排名情况 */
        std::string get_leader_all();
    };
};


#endif /* defined(__gogocrush__selfdefine__) */
