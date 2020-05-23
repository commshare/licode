
//  Created by zhangbin on 2019/8/21.
//  Copyright © 2019 Govern. All rights reserved.
//
#include <iostream>
#include <vector>
#include "HLog.hpp"
#include "spdlog/logger.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#if defined(_MSC_VER)
#include "spdlog/sinks/msvc_sink.h"
#elif defined(__unix__) || defined(__APPLE__)
#include "spdlog/sinks/ansicolor_sink.h"
#ifdef SSF_ENABLE_SYSLOG
#define SPDLOG_ENABLE_SYSLOG
#include "spdlog/sinks/syslog_sink.h"
#endif
#endif  // defined(_MSC_VER)

namespace EJ
{
    //日志文件最大大小
    const static int LOG_FILE_SIZE_IN_MB = 100;
    //only one file
    const static int ROTATING_FILE_COUNT = 0;
    static std::string logname= "EJ";
    std::shared_ptr<spdlog::logger> HLog::s_logger_;
    bool HLog::Init(std::string log_full_path,bool debug,bool console)
    {
        bool init_log_ =true;
        try {
            spdlog::set_pattern("%^[%H:%M:%S %z][%L][%t] %v %$");
            std::vector<spdlog::sink_ptr> sinks;
            //默认不用
            if(console)
            {
#ifdef _WIN32
                auto console_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
                auto console_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

           console_sink->set_level(static_cast<spdlog::level::level_enum>(spdlog::level::trace));
                //同时输出到控制台
               sinks.push_back(std::move(console_sink));
            }
   
            auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_full_path, 1024 * 1024 * LOG_FILE_SIZE_IN_MB, ROTATING_FILE_COUNT);
        file_sink->set_level(static_cast<spdlog::level::level_enum>(spdlog::level::trace));
            sinks.push_back(std::move(file_sink));
            s_logger_= std::make_shared<spdlog::logger>("EduGR",begin(sinks),end(sinks));
            spdlog::register_logger(s_logger_);
            s_logger_->set_level(spdlog::level::trace);
            s_logger_->flush_on(spdlog::level::trace);
        } catch(const spdlog::spdlog_ex &ex) {
            std::cout << "SPDLOG INIT Failed: " << ex.what() << std::endl;
            init_log_ =false;
        }
        return init_log_;
    }
}
