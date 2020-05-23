
//
//  Created by zhangbin on 2019/8/21.
//  Copyright © 2019 Govern. All rights reserved.
//

#ifndef HLog_hpp
#define HLog_hpp
#include <memory>
#include <stdio.h>
#include "spdlog/spdlog.h"

namespace EJ{
 class HLog{
    public:
     static bool Init(std::string log_full_path,bool debug=true,
       bool console=false);
     inline static std::shared_ptr<spdlog::logger> &Getlog()
     {
        return s_logger_;
     }
     
   private:
    static std::shared_ptr<spdlog::logger> s_logger_;
 };
}
#define HLOGI(msg,...) ::EJ::HLog::Getlog()->info(msg,__VA_ARGS__)
#define HLOGE(msg,...) ::EJ::HLog::Getlog()->error(msg,__VA_ARGS__)

#define GLOGV(...) ::EJ::HLog::Getlog()->trace(__VA_ARGS__)
#endif /* HLog_hpp */
