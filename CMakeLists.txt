cmake_minimum_required(VERSION 3.14)
project(licode)

set(CMAKE_CXX_STANDARD 14)

###check macos
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

### https://juejin.im/post/5d5faa175188255d827ce7d8
# 是否使用自己的 MathFunctions 库
# 这里设置的变量 USE_MYMATH、中间的提示文字、默认值，在 ccmake 命令中会展示
option (USE_MYMATH
        "Use provided math implementation"
        ON
        )

##########
if (MACOSX)
add_definitions(
        -D_DEBUG
        -D_USEMAC
)
endif()
##

####${boost_LIBRARIES}
##/usr/local/Cellar/boost/1.71.0
message("==boost===  ${boost_LIBRARIES}  ")
###自己设置好了，这样最快
set(boost_INCLUDE_DIRS "/usr/local/Cellar/boost/1.71.0/include")
set(Boost_INCLUDE_DIRS "/usr/local/Cellar/boost/1.71.0/include")
set(Boost_LIBRARY_DIRS "/usr/local/Cellar/boost/1.71.0/lib")

find_package(Boost 1.71.0 COMPONENTS thread regex system REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    message(STATUS "----------find boost 1.71.0-----------")
    ##--  Boost_LIBRARIES ===Boost::thread;Boost::regex;Boost::system
    message(STATUS " Boost_LIBRARIES ===${Boost_LIBRARIES}")

    #add_executable(10_file_io ${SOURCE_FILES})
    #target_link_libraries(your_project_name ${Boost_LIBRARIES})
endif()

include_directories(${Boost_INCLUDE_DIRS}})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories("/usr/local/include")

    include_directories("/usr/local/opt/openssl/include")
    link_directories("/usr/local/opt/openssl/lib")

    include_directories("/usr/local/Cellar/libevent/2.1.11_1/include")
    include_directories("/usr/local/Cellar/libevent/2.1.11_1/include/event2")
    link_directories("/usr/local/Cellar/libevent/2.1.11_1/lib")

    ##for zlib
    #For compilers to find zlib you may need to set:
#export LDFLAGS="-L/usr/local/opt/zlib/lib"
#export CPPFLAGS="-I/usr/local/opt/zlib/include"

#For pkg-config to find zlib you may need to set:
#export PKG_CONFIG_PATH="/usr/local/opt/zlib/lib/pkgconfig"
    include_directories("/usr/local/opt/zlib/include")
    link_directories("/usr/local/opt/zlib/lib")

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#### erzio

set (ERIZO_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/erizo)
message(STATUS "===${ERIZO_SOURCE}")
set(ERIZO_LIB ${ERIZO_SOURCE}/build/debug/erzio)
link_directories(${ERIZO_LIB})

## spdlog
include_directories(zhangbin/thirdparty/spdlog/include)

##json
include_directories(zhangbin/thirdparty/nlohmann/)
include_directories(zhangbin/thirdparty/)

## tclap

include_directories(zhangbin/thirdparty/tclap-1.2.2/include)

##webrtc
include_directories(${ERIZO_SOURCE}/src/third_party/webrtc/src)

####zlib
include_directories("/usr/local/opt/zlib/include")
link_directories("/usr/local/opt/zlib/lib")

####incude zhangbin
include_directories(zhangbin/)
include_directories(zhangbin/common/)
include_directories(zhangbin/common/OSSupport)

include_directories(zhangbin/include/)
include_directories(zhangbin/include/OSSupport)
include_directories(zhangbin/server/)
include_directories(zhangbin/server/Protocol)

include_directories(zhangbin/thirdparty/polarssl-1.3.9/include)
include_directories(zhangbin/thirdparty/jsoncpp-1.6.5/include)

file(GLOB_RECURSE ZB_JSON_CPP zhangbin/thirdparty/jsoncpp-1.6.5/src/*.cpp)
file(GLOB_RECURSE ZB_COMM_CPP zhangbin/common/*.cpp zhangbin/common/OSSupport/*.cpp)
file(GLOB_RECURSE ZB_CLI_CPP zhangbin/client/*.cpp)
file(GLOB_RECURSE ZB_SVR_CPP zhangbin/server/*.cpp zhangbin/server/OSSupport/*.cpp
        zhangbin/server/Protocol/*.cpp zhangbin/server/PolarSSL++/*.cpp
        zhangbin/server/Channel/*.cpp
        )

set(ZB_FULL_PATH ${CMAKE_CURRENT_SOURCE_DIR})
##这个是关于dump的windows用的
list(REMOVE_ITEM ZB_COMM_CPP ${ZB_FULL_PATH}/zhangbin/common/StackWalker.cpp)
##这个好像写的有点麻烦
list(REMOVE_ITEM ZB_COMM_CPP ${ZB_FULL_PATH}/zhangbin/server/main.cpp)

##这个需要
#list(REMOVE_ITEM ZB_COMM_CPP ${ZB_FULL_PATH}/zhangbin/common/OSSupport/StackTrace.cpp)

message(STATUS "---------------------")
message(STATUS "===${ZB_FULL_PATH}")
message(STATUS "===${ZB_COMM_CPP}")
message(STATUS "---------------------")

add_executable(zhangbin-svr
${ZB_COMM_CPP}
${ZB_SVR_CPP}
${ZB_JSON_CPP}
)
####必须加这个才能检测到boost future 命名空间
set(ERIZO_CMAKE_CXX_FLAGS "${ERIZO_CMAKE_CXX_FLAGS} -DBOOST_THREAD_PROVIDES_FUTURE -DBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION -DBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY")


##z 就是lib啦
target_link_libraries(zhangbin-svr z event event_core event_pthreads erizo)
##############################
#if(${ERIZO_BUILD_TYPE} STREQUAL "debug")
#    message("Generating DEBUG project")
#    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 ${ERIZO_CMAKE_CXX_FLAGS}")
#elseif(${ERIZO_BUILD_TYPE} STREQUAL "sanitizer")
#    message("Generating SANITIZER project")
#    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 ${ERIZO_CMAKE_CXX_FLAGS} -O1 -fno-omit-frame-pointer -fsanitize=address -fno-optimize-sibling-calls")
#else()
#    message("Generating RELEASE project")
#    set(CMAKE_CXX_FLAGS "-g -Wall -O3 -std=c++11 ${ERIZO_CMAKE_CXX_FLAGS}")
#endif()

message("Generating DEBUG project")
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 ${ERIZO_CMAKE_CXX_FLAGS}")

#global variable
set(THIRD_PARTY_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../../build/libdeps/build/include/")
set(THIRD_PARTY_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../../build/libdeps/build/lib/")



##########################
#file(GLOB_RECURSE ERIZO_SOURCES_FILES ${ERIZO_SOURCE}/*.cpp ${ERIZO_SOURCE}/*.h ${ERIZO_TEST}/*.cpp)
#add_custom_target(lint
#        ${ERIZO_ROOT_DIR}/utils/cpplint.py --filter=-legal/copyright,-build/include --linelength=120 ${ERIZO_SOURCES_FILES}
#        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
#        COMMENT "Executing lint"
#        )
#



## Depencencies
#
## Add /usr/local/include
#IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#    include_directories("/usr/local/include")
#ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#
## GLIB
#find_package(Glib REQUIRED)
#include_directories(${GLIB_INCLUDE_DIRS})
#
## BOOST
###boost默认已经安装，路径是啥？？？
##include(${CMAKE_CURRENT_SOURCE_DIR}/../conan_paths.cmake)
#set (BOOST_LIBS thread regex system)
#find_package(boost)
#include_directories(${boost_INCLUDE_DIRS})
#
#
#message(STATUS "---------------------")
#message(STATUS "Boost_FOUND: ${Boost_FOUND}")
#message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
#message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
#message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
#message(STATUS "---------------------")
#
set(ERIZO_CMAKE_CXX_FLAGS "${ERIZO_CMAKE_CXX_FLAGS} -DBOOST_THREAD_PROVIDES_FUTURE -DBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION -DBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY")

## GTHREAD
#find_library(GTHREAD gthread-2.0 HINTS "${THIRD_PARTY_LIB}")
#test_lib(${GTHREAD})
#
## SRTP
#find_library(SRTP srtp2 HINTS "${THIRD_PARTY_LIB}")
#test_lib(${SRTP})
#
## NICE
#find_library(NICE nice HINTS "${THIRD_PARTY_LIB}")
#test_lib(${NICE})

# OPENSSL
##export LDFLAGS=-L/usr/local/opt/openssl/lib
## export CPPFLAGS=-I/usr/local/opt/openssl/include
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories("/Users/zhangbin/tet/licodelllcode/licode/erizo/openssl-1.0.1u-OSX/include")
    link_directories("/Users/zhangbin/tet/licodelllcode/licode/erizo/openssl-1.0.1u-OSX/lib")
    #include_directories("/usr/local/opt/openssl/include")
    #link_directories("/usr/local/opt/openssl/lib")
    #link_directories("/usr/lib")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#
#find_library(SSL ssl HINTS "${THIRD_PARTY_LIB}")
#test_lib(${SSL})
#find_library(CRYPTO crypto HINTS "${THIRD_PARTY_LIB}")
#test_lib(${CRYPTO})
#message("==OPENSSL=1==  ${SSL} ${CRYPTO} ")

##to link our own openssl
##/usr/lib/libssl.dylib /usr/lib/libcrypto.dylib
set(SSL "/Users/zhangbin/tet/licodelllcode/licode/erizo/openssl-1.0.1u-OSX/lib/libssl.a")
set(CRYPTO "/Users/zhangbin/tet/licodelllcode/licode/erizo/openssl-1.0.1u-OSX/lib/libcrypto.a")
message("==OPENSSL==2=  ${SSL} ${CRYPTO} ")



#
## Libav
#find_library(AVUTIL avutil HINTS "${THIRD_PARTY_LIB}")
#test_lib(${AVUTIL})
#set (LIBS ${AVUTIL})
#find_library(AVCODEC avcodec HINTS "${THIRD_PARTY_LIB}")
#test_lib(${AVCODEC})
#set (LIBS ${LIBS} ${AVCODEC})
#find_library(AVFORMAT avformat HINTS "${THIRD_PARTY_LIB}")
#test_lib(${AVFORMAT})
#set (LIBS ${LIBS} ${AVFORMAT})
#
## LOG4CXX
#find_library(LOG log4cxx)
#test_lib(${LOG})

# WEBRTC
# 先不编译webrtc
#include("${CMAKE_CURRENT_SOURCE_DIR}/third_party/webrtc.cmake")

# NICER
# 先不编译nicer
#include("${CMAKE_CURRENT_SOURCE_DIR}/third_party/nicer.cmake")

## Erizo
#先不编译erzio
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/erizo")



set(GMOCK_BUILD "${CMAKE_CURRENT_BINARY_DIR}/libdeps/gmock")
set(GMOCK_VERSION "1.8.0")

##外部工程是gtest
#先不编译gtest
#ExternalProject_Add(gtest
#        URL "https://github.com/google/googletest/archive/release-${GMOCK_VERSION}.tar.gz"
#        PREFIX ${GMOCK_BUILD}
#        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${GMOCK_BUILD} ${SANITIZER_OPTION}
#        )



###############
add_subdirectory(zhangbin/nrappkit)
include_directories(erizo/src/erzio)
include_directories(zhangbin/nrappkit/src)

include_directories(build/libdeps/nvm/test)
include_directories(build/libdeps/nvm/test/fast)
include_directories(build/libdeps/nvm/test/fast/Aliases)
include_directories("build/libdeps/nvm/test/fast/Listing paths")
include_directories("build/libdeps/nvm/test/fast/Listing versions")
include_directories(build/libdeps/nvm/test/slow)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/libplatform)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/aix-gcc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/aix64-gcc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/BSD-x86)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/BSD-x86_64)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/darwin-i386-cc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/darwin64-x86_64-cc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-aarch64)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-armv4)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-elf)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-ppc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-ppc64)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-x32)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-x86_64)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux32-s390x)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux64-s390x)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/solaris-x86-gcc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/solaris64-x86_64-gcc)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/VC-WIN32)
include_directories(build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/VC-WIN64A)
include_directories(erizo/build/debug/libdeps/gmock/include)
include_directories(erizo/build/debug/libdeps/gmock/include/gmock)
include_directories(erizo/build/debug/libdeps/gmock/include/gmock/internal)
include_directories(erizo/build/debug/libdeps/gmock/include/gmock/internal/custom)
include_directories(erizo/build/debug/libdeps/gmock/include/gtest)
include_directories(erizo/build/debug/libdeps/gmock/include/gtest/internal)
include_directories(erizo/build/debug/libdeps/gmock/include/gtest/internal/custom)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/custom)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/include)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/custom)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/src)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/test)
include_directories(erizo/build/debug/libdeps/gmock/src/gtest/googletest/xcode/Samples/FrameworkSample)
include_directories(erizo/build/debug/libdeps/nicer/include)
include_directories(erizo/build/debug/libdeps/nicer/include/crypto)
include_directories(erizo/build/debug/libdeps/nicer/include/event)
include_directories(erizo/build/debug/libdeps/nicer/include/ice)
include_directories(erizo/build/debug/libdeps/nicer/include/log)
include_directories(erizo/build/debug/libdeps/nicer/include/net)
include_directories(erizo/build/debug/libdeps/nicer/include/net/test)
include_directories(erizo/build/debug/libdeps/nicer/include/plugin)
include_directories(erizo/build/debug/libdeps/nicer/include/plugin/api)

##for sever include of
#In file included from /Users/zhangbin/tet/licodelllcode/licode/zhangbin/server/OSSupport/Socket.cpp:5:
#In file included from /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk/usr/include/sys/ioctl.h:72:
#/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/port/linux/include/sys/ttycom.h:36:10: fatal error: 'asm/ioctls.h' fi
#

#include_directories(erizo/build/debug/libdeps/nicer/include/plugin/internal)
#include_directories(erizo/build/debug/libdeps/nicer/include/port)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/darwin)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/darwin/include)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/darwin/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/generic)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/generic/include)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/generic/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/linux)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/linux/include)
#include_directories(erizo/build/debug/libdeps/nicer/include/port/linux/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/include/registry)
include_directories(erizo/build/debug/libdeps/nicer/include/share)
include_directories(erizo/build/debug/libdeps/nicer/include/stats)
include_directories(erizo/build/debug/libdeps/nicer/include/stun)
include_directories(erizo/build/debug/libdeps/nicer/include/stun/test)
include_directories(erizo/build/debug/libdeps/nicer/include/util)
include_directories(erizo/build/debug/libdeps/nicer/include/util/libekr)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/crypto)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/net)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test)
include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer/src/util)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/event)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/log)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/plugin)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/plugin/api)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/plugin/internal)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/darwin)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/darwin/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/darwin/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/generic)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/generic/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/generic/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/share)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/stats)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/event)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/log)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/api)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/internal)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/darwin)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/darwin/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/darwin/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/generic)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/generic/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/generic/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux/include)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux/include/sys)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/stats)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util)
#include_directories(erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr)
include_directories(erizo/src/erizo)
include_directories(erizo/src/erizo/bandwidth)
include_directories(erizo/src/erizo/dtls)
include_directories(erizo/src/erizo/lib)
include_directories(erizo/src/erizo/media)
include_directories(erizo/src/erizo/media/codecs)
include_directories(erizo/src/erizo/media/mixers)
include_directories(erizo/src/erizo/pipeline)
include_directories(erizo/src/erizo/rtp)
include_directories(erizo/src/erizo/stats)
include_directories(erizo/src/erizo/thread)
include_directories(erizo/src/examples)
include_directories(erizo/src/examples/pc)
include_directories(erizo/src/test/utils)
include_directories(erizo/src/third_party/webrtc/src/webrtc)
include_directories(erizo/src/third_party/webrtc/src/webrtc/base)
include_directories(erizo/src/third_party/webrtc/src/webrtc/base/analytics)
include_directories(erizo/src/third_party/webrtc/src/webrtc/common_video)
include_directories(erizo/src/third_party/webrtc/src/webrtc/common_video/h264)
include_directories(erizo/src/third_party/webrtc/src/webrtc/common_video/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/include/mock)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/pacing)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/test)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/video_coding)
include_directories(erizo/src/third_party/webrtc/src/webrtc/modules/video_coding/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include)
include_directories(erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source)
include_directories(erizoAPI)
include_directories(erizoAPI/lib)
include_directories(node_modules/nan)

add_executable(licode
        "build/libdeps/nvm/test/fast/Aliases/Running \"nvm alias <aliasname>\" should list but one alias."
        "build/libdeps/nvm/test/fast/Aliases/Running \"nvm alias\" should list all aliases."
        "build/libdeps/nvm/test/fast/Listing paths/Running \"nvm which 0.0.2\" should display only version 0.0.2."
        "build/libdeps/nvm/test/fast/Listing versions/Running \"nvm ls 0.0.2\" should display only version 0.0.2."
        "build/libdeps/nvm/test/fast/Listing versions/Running \"nvm ls 0.2\" should display only 0.2.x versions."
        "build/libdeps/nvm/test/fast/Listing versions/Running \"nvm ls\" should display all installed versions."
        "build/libdeps/nvm/test/fast/Listing versions/Running \"nvm ls\" with nounset should not fail."
        "build/libdeps/nvm/test/fast/Running \"nvm alias\" should create a file in the alias directory."
        "build/libdeps/nvm/test/fast/Running \"nvm current\" should display current nvm environment."
        "build/libdeps/nvm/test/fast/Running \"nvm deactivate\" should unset the nvm environment variables."
        "build/libdeps/nvm/test/fast/Running \"nvm unalias\" should remove the alias file."
        "build/libdeps/nvm/test/fast/Running \"nvm uninstall\" should remove the appropriate directory."
        "build/libdeps/nvm/test/fast/Running \"nvm unload\" should unset all function and variables."
        "build/libdeps/nvm/test/fast/Running \"nvm use\" should drop CR char automatically."
        "build/libdeps/nvm/test/fast/Sourcing nvm.sh should make the nvm command available."
        "build/libdeps/nvm/test/slow/Running \"nvm current\" should display current nvm environment."
        build/libdeps/nvm/versions/node/v6.14.4/include/node/libplatform/libplatform.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/aix-gcc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/aix64-gcc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/BSD-x86/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/BSD-x86_64/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/darwin-i386-cc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/darwin64-x86_64-cc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-aarch64/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-armv4/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-elf/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-ppc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-ppc64/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-x32/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-x86_64/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux32-s390x/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux64-s390x/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/solaris-x86-gcc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/solaris64-x86_64-gcc/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/VC-WIN32/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/VC-WIN64A/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/aes.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/asn1.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/asn1_mac.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/asn1t.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/bio.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/blowfish.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/bn.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/buffer.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/camellia.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/cast.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/cmac.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/cms.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/comp.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/conf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/conf_api.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/crypto.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/des.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/des_old.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/dh.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/dsa.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/dso.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/dtls1.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/e_os2.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ebcdic.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ec.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ecdh.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ecdsa.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/engine.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/err.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/evp.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/hmac.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/idea.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/krb5_asn.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/kssl.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/lhash.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/md4.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/md5.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/mdc2.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/modes.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/obj_mac.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/objects.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ocsp.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/opensslconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/opensslv.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ossl_typ.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/pem.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/pem2.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/pkcs12.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/pkcs7.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/pqueue.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/rand.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/rc2.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/rc4.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ripemd.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/rsa.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/safestack.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/seed.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/sha.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/srp.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/srtp.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ssl.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ssl2.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ssl23.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ssl3.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/stack.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/symhacks.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/tls1.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ts.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/txt_db.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ui.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/ui_compat.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/whrlpool.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/x509.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/x509_vfy.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/x509v3.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/android-ifaddrs.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/ares.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/ares_build.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/ares_rules.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/ares_version.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/nameser.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/node.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/node_api.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/node_api_types.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/node_buffer.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/node_object_wrap.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/node_version.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/pthread-barrier.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/stdint-msvc2008.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/tree.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-aix.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-bsd.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-darwin.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-errno.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-linux.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-os390.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-posix.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-sunos.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-threadpool.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-unix.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-version.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv-win.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/uv.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-debug.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-experimental.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-platform.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-profiler.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-testing.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-util.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8-version.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/v8config.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/zconf.h
        build/libdeps/nvm/versions/node/v6.14.4/include/node/zlib.h
        build/libdeps/nvm/versions/node/v6.14.4/lib/node_modules/node-gyp/gyp/data/win/large-pdb-shim.cc
        build/libdeps/nvm/versions/node/v6.14.4/lib/node_modules/node-gyp/src/win_delay_load_hook.cc
        build/libdeps/nvm/versions/node/v6.14.4/lib/node_modules/npm/node_modules/node-gyp/gyp/data/win/large-pdb-shim.cc
        build/libdeps/nvm/versions/node/v6.14.4/lib/node_modules/npm/node_modules/node-gyp/src/win_delay_load_hook.cc
        build/libdeps/nvm/versions/node/v6.14.4/lib/node_modules/npm/node_modules/request/node_modules/node-uuid/benchmark/benchmark-native.c
        erizo/build/debug/libdeps/gmock/include/gmock/internal/custom/gmock-generated-actions.h
        erizo/build/debug/libdeps/gmock/include/gmock/internal/custom/gmock-matchers.h
        erizo/build/debug/libdeps/gmock/include/gmock/internal/custom/gmock-port.h
        erizo/build/debug/libdeps/gmock/include/gmock/internal/gmock-generated-internal-utils.h
        erizo/build/debug/libdeps/gmock/include/gmock/internal/gmock-internal-utils.h
        erizo/build/debug/libdeps/gmock/include/gmock/internal/gmock-port.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-actions.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-cardinalities.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-generated-actions.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-generated-function-mockers.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-generated-matchers.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-generated-nice-strict.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-matchers.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-more-actions.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-more-matchers.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock-spec-builders.h
        erizo/build/debug/libdeps/gmock/include/gmock/gmock.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/custom/gtest-port.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/custom/gtest-printers.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/custom/gtest.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-death-test-internal.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-filepath.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-internal.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-linked_ptr.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-param-util-generated.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-param-util.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-port-arch.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-port.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-string.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-tuple.h
        erizo/build/debug/libdeps/gmock/include/gtest/internal/gtest-type-util.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-death-test.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-message.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-param-test.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-printers.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-spi.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-test-part.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest-typed-test.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest_pred_impl.h
        erizo/build/debug/libdeps/gmock/include/gtest/gtest_prod.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/custom/gmock-generated-actions.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/custom/gmock-matchers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/custom/gmock-port.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/gmock-generated-internal-utils.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/gmock-internal-utils.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/gmock-port.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-actions.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-cardinalities.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-generated-actions.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-generated-function-mockers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-generated-matchers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-generated-nice-strict.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-matchers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-more-actions.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-more-matchers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock-spec-builders.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/gmock.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock-all.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock-cardinalities.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock-internal-utils.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock-matchers.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock-spec-builders.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/src/gmock_main.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-actions_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-cardinalities_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-generated-actions_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-generated-function-mockers_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-generated-internal-utils_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-generated-matchers_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-internal-utils_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-matchers_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-more-actions_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-nice-strict_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-port_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock-spec-builders_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_all_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_ex_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_leak_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_link2_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_link_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_link_test.h
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_output_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_stress_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test/gmock_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/codegear/gtest_all.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/codegear/gtest_link.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/custom/gtest-port.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/custom/gtest-printers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/custom/gtest.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-death-test-internal.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-filepath.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-internal.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-linked_ptr.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-param-util-generated.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-param-util.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-port-arch.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-port.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-string.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-tuple.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/gtest-type-util.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-death-test.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-message.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-param-test.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-printers.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-spi.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-test-part.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest-typed-test.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest_pred_impl.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/gtest_prod.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/prime_tables.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample1.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample1.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample10_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample1_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample2.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample2.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample2_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample3-inl.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample3_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample4.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample4.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample4_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample5_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample6_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample7_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample8_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples/sample9_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-all.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-death-test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-filepath.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-internal-inl.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-port.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-printers.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-test-part.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest-typed-test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/src/gtest_main.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-death-test_ex_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-death-test_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-filepath_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-linked_ptr_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-listener_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-message_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-options_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-param-test2_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-param-test_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-param-test_test.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-port_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-printers_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-test-part_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-tuple_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-typed-test2_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-typed-test_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-typed-test_test.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest-unittest-api_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_all_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_break_on_failure_unittest_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_catch_exceptions_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_color_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_env_var_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_environment_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_filter_unittest_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_help_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_list_tests_unittest_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_main_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_no_test_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_output_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_pred_impl_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_premature_exit_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_prod_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_repeat_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_shuffle_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_sole_header_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_stress_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_throw_on_failure_ex_test.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_throw_on_failure_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_uninitialized_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_unittest.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_xml_outfile1_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_xml_outfile2_test_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/gtest_xml_output_unittest_.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/production.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/test/production.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/xcode/Samples/FrameworkSample/widget.cc
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/xcode/Samples/FrameworkSample/widget.h
        erizo/build/debug/libdeps/gmock/src/gtest/googletest/xcode/Samples/FrameworkSample/widget_test.cc
        erizo/build/debug/libdeps/nicer/include/crypto/nr_crypto.h
        erizo/build/debug/libdeps/nicer/include/crypto/nr_crypto_openssl.h
        erizo/build/debug/libdeps/nicer/include/event/async_timer.h
        erizo/build/debug/libdeps/nicer/include/event/async_wait.h
        erizo/build/debug/libdeps/nicer/include/event/async_wait_int.h
        erizo/build/debug/libdeps/nicer/include/ice/codewords.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_candidate.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_candidate_pair.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_codeword.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_component.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_ctx.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_handler.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_media_stream.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_peer_ctx.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_reg.h
        erizo/build/debug/libdeps/nicer/include/ice/ice_socket.h
        erizo/build/debug/libdeps/nicer/include/log/r_log.h
        erizo/build/debug/libdeps/nicer/include/net/test/nr_socket_evil.h
        erizo/build/debug/libdeps/nicer/include/net/test/nr_socket_nat.h
        erizo/build/debug/libdeps/nicer/include/net/local_addr.h
        erizo/build/debug/libdeps/nicer/include/net/nr_interface_prioritizer.h
        erizo/build/debug/libdeps/nicer/include/net/nr_proxy_tunnel.h
        erizo/build/debug/libdeps/nicer/include/net/nr_resolver.h
        erizo/build/debug/libdeps/nicer/include/net/nr_socket.h
        erizo/build/debug/libdeps/nicer/include/net/nr_socket_local.h
        erizo/build/debug/libdeps/nicer/include/net/nr_socket_multi_tcp.h
        erizo/build/debug/libdeps/nicer/include/net/nr_socket_wrapper.h
        erizo/build/debug/libdeps/nicer/include/net/transport_addr.h
        erizo/build/debug/libdeps/nicer/include/net/transport_addr_reg.h
        erizo/build/debug/libdeps/nicer/include/plugin/api/api_force.h
        erizo/build/debug/libdeps/nicer/include/plugin/internal/nr_plugin_int.h
        erizo/build/debug/libdeps/nicer/include/plugin/nr_plugin.h
        erizo/build/debug/libdeps/nicer/include/port/darwin/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/include/port/darwin/include/csi_platform.h
        erizo/build/debug/libdeps/nicer/include/port/generic/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/include/port/linux/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/include/port/linux/include/sys/ttycom.h
        erizo/build/debug/libdeps/nicer/include/port/linux/include/csi_platform.h
        erizo/build/debug/libdeps/nicer/include/port/linux/include/linux_funcs.h
        erizo/build/debug/libdeps/nicer/include/port/nr_extattr.h
        erizo/build/debug/libdeps/nicer/include/registry/c2ru.h
        erizo/build/debug/libdeps/nicer/include/registry/registry.h
        erizo/build/debug/libdeps/nicer/include/registry/registry_int.h
        erizo/build/debug/libdeps/nicer/include/registry/registry_vtbl.h
        erizo/build/debug/libdeps/nicer/include/registry/regrpc.h
        erizo/build/debug/libdeps/nicer/include/registry/regrpc_client.h
        erizo/build/debug/libdeps/nicer/include/share/nr_api.h
        erizo/build/debug/libdeps/nicer/include/share/nr_common.h
        erizo/build/debug/libdeps/nicer/include/share/nr_dynlib.h
        erizo/build/debug/libdeps/nicer/include/share/nr_reg_keys.h
        erizo/build/debug/libdeps/nicer/include/share/nr_startup.h
        erizo/build/debug/libdeps/nicer/include/stats/nrstats.h
        erizo/build/debug/libdeps/nicer/include/stats/nrstats_int.h
        erizo/build/debug/libdeps/nicer/include/stun/test/stun_test_util.h
        erizo/build/debug/libdeps/nicer/include/stun/test/udp.h
        erizo/build/debug/libdeps/nicer/include/stun/addrs.h
        erizo/build/debug/libdeps/nicer/include/stun/ifaddrs-android.h
        erizo/build/debug/libdeps/nicer/include/stun/nr_socket_buffered_stun.h
        erizo/build/debug/libdeps/nicer/include/stun/nr_socket_turn.h
        erizo/build/debug/libdeps/nicer/include/stun/stun.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_build.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_client_ctx.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_codec.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_hint.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_msg.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_proc.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_reg.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_server_ctx.h
        erizo/build/debug/libdeps/nicer/include/stun/stun_util.h
        erizo/build/debug/libdeps/nicer/include/stun/turn_client_ctx.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/assoc.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/debug.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_assoc.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_bitfield.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_common.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_crc32.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_data.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_defaults.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_errors.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_includes.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_list.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_macros.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_memory.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_thread.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_time.h
        erizo/build/debug/libdeps/nicer/include/util/libekr/r_types.h
        erizo/build/debug/libdeps/nicer/include/util/byteorder.h
        erizo/build/debug/libdeps/nicer/include/util/cb_args.h
        erizo/build/debug/libdeps/nicer/include/util/escape.h
        erizo/build/debug/libdeps/nicer/include/util/getopt.h
        erizo/build/debug/libdeps/nicer/include/util/hex.h
        erizo/build/debug/libdeps/nicer/include/util/ice_util.h
        erizo/build/debug/libdeps/nicer/include/util/mbslen.h
        erizo/build/debug/libdeps/nicer/include/util/mem_util.h
        erizo/build/debug/libdeps/nicer/include/util/mutex.h
        erizo/build/debug/libdeps/nicer/include/util/p_buf.h
        erizo/build/debug/libdeps/nicer/include/util/ssl_util.h
        erizo/build/debug/libdeps/nicer/include/util/util.h
        erizo/build/debug/libdeps/nicer/include/util/util_db.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/crypto/nr_crypto.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/crypto/nr_crypto.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/crypto/nr_crypto_openssl.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/crypto/nr_crypto_openssl.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/test/ice_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/codewords.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_candidate.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_candidate.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_candidate_pair.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_candidate_pair.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_codeword.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_codeword.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_component.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_component.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_ctx.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_ctx.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_handler.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_media_stream.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_media_stream.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_parser.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_peer_ctx.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_peer_ctx.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_reg.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_socket.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice/ice_socket.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/evil_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/nr_socket_evil.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/nr_socket_evil.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/nr_socket_nat.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/nr_socket_nat.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/nr_socket_nat_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/nr_socket_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test/transport_addr_reg_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/local_addr.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/local_addr.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_interface_prioritizer.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_interface_prioritizer.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_proxy_tunnel.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_proxy_tunnel.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_resolver.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_resolver.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket_local.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket_local.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket_multi_tcp.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket_multi_tcp.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket_wrapper.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/nr_socket_wrapper.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/transport_addr.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/transport_addr.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/transport_addr_reg.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/transport_addr_reg.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/client.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/dup_addr_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/odd_test_vectors.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/remi_vector_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/server.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/stun_loopback_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/stun_test_util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/stun_test_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/tlsServer.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/turn_loopback_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/turn_test_client.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/turn_test_echo_server.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/turn_test_server.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/turn_test_server_util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/udp.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test/udp.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/addrs.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/addrs.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/ifaddrs-android.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/ifaddrs-android.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/nr_socket_buffered_stun.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/nr_socket_buffered_stun.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/nr_socket_turn.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/nr_socket_turn.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_build.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_build.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_client_ctx.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_client_ctx.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_codec.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_codec.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_hint.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_hint.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_msg.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_msg.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_proc.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_proc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_reg.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_server_ctx.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_server_ctx.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/stun_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/turn_client_ctx.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/turn_client_ctx.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/CallHandler.cxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/CallHandler.hxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/CommandLineParser.cxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/CommandLineParser.hxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/testUA.cxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/UserAgent.cxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua/UserAgent.hxx
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/util/cb_args.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/util/cb_args.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/util/ice_util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/util/ice_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/util/mbslen.c
        erizo/build/debug/libdeps/nicer/src/project_nicer/src/util/mbslen.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/event/async_timer.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/event/async_wait.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/event/async_wait_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/log/r_log.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/plugin/api/api_force.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/plugin/internal/nr_plugin_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/plugin/nr_plugin.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/darwin/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/darwin/include/csi_platform.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/generic/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux/include/sys/ttycom.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux/include/csi_platform.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/linux/include/linux_funcs.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/port/nr_extattr.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry/c2ru.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry/registry.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry/registry_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry/registry_vtbl.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry/regrpc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/registry/regrpc_client.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/share/nr_api.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/share/nr_common.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/share/nr_dynlib.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/share/nr_reg_keys.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/share/nr_startup.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/stats/nrstats.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/stats/nrstats_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/assoc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/debug.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_assoc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_bitfield.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_common.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_crc32.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_data.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_defaults.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_errors.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_includes.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_list.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_macros.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_memory.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_thread.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_time.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/libekr/r_types.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/byteorder.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/escape.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/getopt.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/hex.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/mem_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/mutex.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/p_buf.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/ssl_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/include/util/util_db.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/event/async_timer.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/event/async_timer.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/event/async_wait.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/event/async_wait.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/event/async_wait_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/log/r_log.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/log/r_log.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/log/r_log_plugin.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/api/api_force.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/api/api_force.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/internal/nr_plugin_int.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/internal/nr_plugin_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/plugin/nr_plugin.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/darwin/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/darwin/include/csi_platform.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/generic/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/impl/extattr/bsd/bsd_extattr.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/impl/extattr/xattr/xattr_extattr.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux/include/sys/queue.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux/include/sys/ttycom.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux/include/csi_platform.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/linux/include/linux_funcs.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/port/nr_extattr.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/mod_registry/mod_registry.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/c2ru.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/c2ru.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/nrregctl.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/nrregistryctl.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registry.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registry.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registry_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registry_local.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registry_plugin.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registry_vtbl.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registrycb.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/registryd.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_client.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_client.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_client_cb.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_clnt.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_server.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_svc.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/registry/regrpc_xdr.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nr_api.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nr_common.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nr_dynlib.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nr_reg_keys.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nr_startup.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nr_startup.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/share/nrappkit_static_plugins.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/stats/nrstats.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/stats/nrstats.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/stats/nrstats_app.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/stats/nrstats_int.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/stats/nrstats_memory.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/threads/pthreads/pthread.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/assoc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/debug.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/debug.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_assoc.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_assoc.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_assoc_test.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_bitfield.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_bitfield.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_common.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_crc32.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_crc32.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_data.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_data.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_defaults.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_errors.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_errors.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_includes.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_list.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_list.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_macros.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_memory.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_memory.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_replace.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_thread.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_time.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_time.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/libekr/r_types.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/byteorder.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/byteorder.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/escape.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/escape.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/filename.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/getopt.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/getopt.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/hex.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/hex.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/mem_util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/mem_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/mutex.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/mutex.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/p_buf.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/p_buf.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/ssl_util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/ssl_util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/util.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/util.h
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/util_db.c
        erizo/build/debug/libdeps/nicer/src/project_nicer-build/third_party/nrappkit/src/project_nrappkit/src/util/util_db.h
        erizo/src/erizo/bandwidth/BandwidthDistributionAlgorithm.h
        erizo/src/erizo/bandwidth/ConnectionQualityCheck.cpp
        erizo/src/erizo/bandwidth/ConnectionQualityCheck.h
        erizo/src/erizo/bandwidth/MaxVideoBWDistributor.cpp
        erizo/src/erizo/bandwidth/MaxVideoBWDistributor.h
        erizo/src/erizo/bandwidth/TargetVideoBWDistributor.cpp
        erizo/src/erizo/bandwidth/TargetVideoBWDistributor.h
        erizo/src/erizo/dtls/bf_dwrap.c
        erizo/src/erizo/dtls/bf_dwrap.h
        erizo/src/erizo/dtls/DtlsClient.cpp
        erizo/src/erizo/dtls/DtlsSocket.cpp
        erizo/src/erizo/dtls/DtlsSocket.h
        erizo/src/erizo/lib/Clock.h
        erizo/src/erizo/lib/ClockUtils.h
        erizo/src/erizo/lib/LibNiceInterface.h
        erizo/src/erizo/lib/LibNiceInterfaceImpl.cpp
        erizo/src/erizo/lib/NicerInterface.cpp
        erizo/src/erizo/lib/NicerInterface.h
        erizo/src/erizo/lib/TokenBucket.cpp
        erizo/src/erizo/lib/TokenBucket.h
        erizo/src/erizo/media/codecs/AudioCodec.cpp
        erizo/src/erizo/media/codecs/AudioCodec.h
        erizo/src/erizo/media/codecs/Codecs.h
        erizo/src/erizo/media/codecs/VideoCodec.cpp
        erizo/src/erizo/media/codecs/VideoCodec.h
        erizo/src/erizo/media/mixers/VideoMixer.cpp
        erizo/src/erizo/media/mixers/VideoMixer.h
        erizo/src/erizo/media/mixers/VideoUtils.cpp
        erizo/src/erizo/media/mixers/VideoUtils.h
        erizo/src/erizo/media/Depacketizer.cpp
        erizo/src/erizo/media/Depacketizer.h
        erizo/src/erizo/media/ExternalInput.cpp
        erizo/src/erizo/media/ExternalInput.h
        erizo/src/erizo/media/ExternalOutput.cpp
        erizo/src/erizo/media/ExternalOutput.h
        erizo/src/erizo/media/MediaProcessor.cpp
        erizo/src/erizo/media/MediaProcessor.h
        erizo/src/erizo/media/OneToManyTranscoder.cpp
        erizo/src/erizo/media/OneToManyTranscoder.h
        erizo/src/erizo/media/SyntheticInput.cpp
        erizo/src/erizo/media/SyntheticInput.h
        erizo/src/erizo/pipeline/Handler.h
        erizo/src/erizo/pipeline/HandlerContext-inl.h
        erizo/src/erizo/pipeline/HandlerContext.h
        erizo/src/erizo/pipeline/HandlerManager.h
        erizo/src/erizo/pipeline/Pipeline-inl.h
        erizo/src/erizo/pipeline/Pipeline.cpp
        erizo/src/erizo/pipeline/Pipeline.h
        erizo/src/erizo/pipeline/Service.h
        erizo/src/erizo/pipeline/ServiceContext-inl.h
        erizo/src/erizo/pipeline/ServiceContext.h
        erizo/src/erizo/rtp/BandwidthEstimationHandler.cpp
        erizo/src/erizo/rtp/BandwidthEstimationHandler.h
        erizo/src/erizo/rtp/FakeKeyframeGeneratorHandler.cpp
        erizo/src/erizo/rtp/FakeKeyframeGeneratorHandler.h
        erizo/src/erizo/rtp/FecReceiverHandler.cpp
        erizo/src/erizo/rtp/FecReceiverHandler.h
        erizo/src/erizo/rtp/LayerBitrateCalculationHandler.cpp
        erizo/src/erizo/rtp/LayerBitrateCalculationHandler.h
        erizo/src/erizo/rtp/LayerDetectorHandler.cpp
        erizo/src/erizo/rtp/LayerDetectorHandler.h
        erizo/src/erizo/rtp/PacketBufferService.cpp
        erizo/src/erizo/rtp/PacketBufferService.h
        erizo/src/erizo/rtp/PacketCodecParser.cpp
        erizo/src/erizo/rtp/PacketCodecParser.h
        erizo/src/erizo/rtp/PeriodicPliHandler.cpp
        erizo/src/erizo/rtp/PeriodicPliHandler.h
        erizo/src/erizo/rtp/PliPacerHandler.cpp
        erizo/src/erizo/rtp/PliPacerHandler.h
        erizo/src/erizo/rtp/PliPriorityHandler.cpp
        erizo/src/erizo/rtp/PliPriorityHandler.h
        erizo/src/erizo/rtp/QualityFilterHandler.cpp
        erizo/src/erizo/rtp/QualityFilterHandler.h
        erizo/src/erizo/rtp/QualityManager.cpp
        erizo/src/erizo/rtp/QualityManager.h
        erizo/src/erizo/rtp/RtcpAggregator.cpp
        erizo/src/erizo/rtp/RtcpAggregator.h
        erizo/src/erizo/rtp/RtcpFeedbackGenerationHandler.cpp
        erizo/src/erizo/rtp/RtcpFeedbackGenerationHandler.h
        erizo/src/erizo/rtp/RtcpForwarder.cpp
        erizo/src/erizo/rtp/RtcpForwarder.h
        erizo/src/erizo/rtp/RtcpNackGenerator.cpp
        erizo/src/erizo/rtp/RtcpNackGenerator.h
        erizo/src/erizo/rtp/RtcpProcessor.h
        erizo/src/erizo/rtp/RtcpProcessorHandler.cpp
        erizo/src/erizo/rtp/RtcpProcessorHandler.h
        erizo/src/erizo/rtp/RtcpRrGenerator.cpp
        erizo/src/erizo/rtp/RtcpRrGenerator.h
        erizo/src/erizo/rtp/RtpExtensionProcessor.cpp
        erizo/src/erizo/rtp/RtpExtensionProcessor.h
        erizo/src/erizo/rtp/RtpH264Parser.cpp
        erizo/src/erizo/rtp/RtpH264Parser.h
        erizo/src/erizo/rtp/RtpHeaders.h
        erizo/src/erizo/rtp/RtpPacketQueue.cpp
        erizo/src/erizo/rtp/RtpPacketQueue.h
        erizo/src/erizo/rtp/RtpPaddingGeneratorHandler.cpp
        erizo/src/erizo/rtp/RtpPaddingGeneratorHandler.h
        erizo/src/erizo/rtp/RtpPaddingManagerHandler.cpp
        erizo/src/erizo/rtp/RtpPaddingManagerHandler.h
        erizo/src/erizo/rtp/RtpPaddingRemovalHandler.cpp
        erizo/src/erizo/rtp/RtpPaddingRemovalHandler.h
        erizo/src/erizo/rtp/RtpRetransmissionHandler.cpp
        erizo/src/erizo/rtp/RtpRetransmissionHandler.h
        erizo/src/erizo/rtp/RtpSink.cpp
        erizo/src/erizo/rtp/RtpSink.h
        erizo/src/erizo/rtp/RtpSlideShowHandler.cpp
        erizo/src/erizo/rtp/RtpSlideShowHandler.h
        erizo/src/erizo/rtp/RtpSource.cpp
        erizo/src/erizo/rtp/RtpSource.h
        erizo/src/erizo/rtp/RtpTrackMuteHandler.cpp
        erizo/src/erizo/rtp/RtpTrackMuteHandler.h
        erizo/src/erizo/rtp/RtpUtils.cpp
        erizo/src/erizo/rtp/RtpUtils.h
        erizo/src/erizo/rtp/RtpVP8Fragmenter.cpp
        erizo/src/erizo/rtp/RtpVP8Fragmenter.h
        erizo/src/erizo/rtp/RtpVP8Parser.cpp
        erizo/src/erizo/rtp/RtpVP8Parser.h
        erizo/src/erizo/rtp/RtpVP9Parser.cpp
        erizo/src/erizo/rtp/RtpVP9Parser.h
        erizo/src/erizo/rtp/SenderBandwidthEstimantionHandler.cpp
        erizo/src/erizo/rtp/SenderBandwidthEstimationHandler.h
        erizo/src/erizo/rtp/SequenceNumberTranslator.cpp
        erizo/src/erizo/rtp/SequenceNumberTranslator.h
        erizo/src/erizo/rtp/SRPacketHandler.cpp
        erizo/src/erizo/rtp/SRPacketHandler.h
        erizo/src/erizo/rtp/StatsHandler.cpp
        erizo/src/erizo/rtp/StatsHandler.h
        erizo/src/erizo/stats/StatNode.cpp
        erizo/src/erizo/stats/StatNode.h
        erizo/src/erizo/thread/IOThreadPool.cpp
        erizo/src/erizo/thread/IOThreadPool.h
        erizo/src/erizo/thread/IOWorker.cpp
        erizo/src/erizo/thread/IOWorker.h
        erizo/src/erizo/thread/Scheduler.cpp
        erizo/src/erizo/thread/Scheduler.h
        erizo/src/erizo/thread/ThreadPool.cpp
        erizo/src/erizo/thread/ThreadPool.h
        erizo/src/erizo/thread/Worker.cpp
        erizo/src/erizo/thread/Worker.h
        erizo/src/erizo/DefaultValues.h
        erizo/src/erizo/DtlsTransport.cpp
        erizo/src/erizo/DtlsTransport.h
        erizo/src/erizo/IceConnection.cpp
        erizo/src/erizo/IceConnection.h
        erizo/src/erizo/LibNiceConnection.cpp
        erizo/src/erizo/LibNiceConnection.h
        erizo/src/erizo/logger.h
        erizo/src/erizo/MediaDefinitions.h
        erizo/src/erizo/MediaStream.cpp
        erizo/src/erizo/MediaStream.h
        erizo/src/erizo/NicerConnection.cpp
        erizo/src/erizo/NicerConnection.h
        erizo/src/erizo/OneToManyProcessor.cpp
        erizo/src/erizo/OneToManyProcessor.h
        erizo/src/erizo/SdpInfo.cpp
        erizo/src/erizo/SdpInfo.h
        erizo/src/erizo/SrtpChannel.cpp
        erizo/src/erizo/SrtpChannel.h
        erizo/src/erizo/Stats.cpp
        erizo/src/erizo/Stats.h
        erizo/src/erizo/StringUtil.cpp
        erizo/src/erizo/StringUtil.h
        erizo/src/erizo/Transport.h
        erizo/src/erizo/WebRtcConnection.cpp
        erizo/src/erizo/WebRtcConnection.h
        erizo/src/examples/pc/Observer.cpp
        erizo/src/examples/pc/Observer.h
        erizo/src/examples/pc/PCSocket.cpp
        erizo/src/examples/pc/PCSocket.h
        erizo/src/examples/pc/SDPReceiver.h
        erizo/src/examples/hsam.cpp
        erizo/src/examples/Test.cpp
        erizo/src/examples/Test.h
        erizo/src/test/bandwidth/MaxVideoBWDistributor.cpp
        erizo/src/test/bandwidth/TargetVideoBWDistributor.cpp
        erizo/src/test/fallback/ConnectionQualityCheckTest.cpp
        erizo/src/test/lib/TokenBucketTest.cpp
        erizo/src/test/media/DepacketizerTest.cpp
        erizo/src/test/media/SyntheticInputTest.cpp
        erizo/src/test/rtp/BandwidthEstimationHandlerTest.cpp
        erizo/src/test/rtp/FakeKeyframeGeneratorHandlerTest.cpp
        erizo/src/test/rtp/FecReceiverHandlerTest.cpp
        erizo/src/test/rtp/LayerBitrateCalculationHandlerTest.cpp
        erizo/src/test/rtp/LayerDetectorHandlerTest.cpp
        erizo/src/test/rtp/PeriodicPliHandlerTest.cpp
        erizo/src/test/rtp/PliPacerHandlerTest.cpp
        erizo/src/test/rtp/PliPriorityHandlerTest.cpp
        erizo/src/test/rtp/QualityManagerTest.cpp
        erizo/src/test/rtp/RtcpFeedbackGenerationHandlerTest.cpp
        erizo/src/test/rtp/RtcpNackGeneratorTest.cpp
        erizo/src/test/rtp/RtcpProcessorHandlerTest.cpp
        erizo/src/test/rtp/RtcpRrGeneratorTest.cpp
        erizo/src/test/rtp/RtpExtensionProcessorTest.cpp
        erizo/src/test/rtp/RtpPaddingGeneratorHandlerTest.cpp
        erizo/src/test/rtp/RtpPaddingManagerHandlerTest.cpp
        erizo/src/test/rtp/RtpRetransmissionHandlerTest.cpp
        erizo/src/test/rtp/RtpSlideShowHandlerTest.cpp
        erizo/src/test/rtp/RtpTrackMuteHandlerTest.cpp
        erizo/src/test/rtp/SenderBandwidthEstimationTest.cpp
        erizo/src/test/rtp/SequenceNumberTranslatorTest.cpp
        erizo/src/test/rtp/SRPacketHandlerTest.cpp
        erizo/src/test/rtp/StatsHandlerTest.cpp
        erizo/src/test/stats/MovingAverageStatTest.cpp
        erizo/src/test/stats/MovingIntervalRateStatTest.cpp
        erizo/src/test/stats/StatNodeTest.cpp
        erizo/src/test/thread/SchedulerTest.cpp
        erizo/src/test/utils/Matchers.h
        erizo/src/test/utils/Mocks.h
        erizo/src/test/utils/Tools.h
        erizo/src/test/DtlsSocketTest.cpp
        erizo/src/test/LibNiceConnectionTest.cpp
        erizo/src/test/main.cpp
        erizo/src/test/NicerConnectionTest.cpp
        erizo/src/test/OneToManyProcessorTest.cpp
        erizo/src/test/PacketTest.cpp
        erizo/src/test/SdpTest.cpp
        erizo/src/test/WebRtcConnectionTest.cpp
        erizo/src/third_party/webrtc/src/webrtc/base/analytics/exp_filter.cc
        erizo/src/third_party/webrtc/src/webrtc/base/analytics/exp_filter.h
        erizo/src/third_party/webrtc/src/webrtc/base/analytics/percentile_filter.h
        erizo/src/third_party/webrtc/src/webrtc/base/array_view.h
        erizo/src/third_party/webrtc/src/webrtc/base/arraysize.h
        erizo/src/third_party/webrtc/src/webrtc/base/atomicops.h
        erizo/src/third_party/webrtc/src/webrtc/base/basictypes.h
        erizo/src/third_party/webrtc/src/webrtc/base/bitbuffer.cc
        erizo/src/third_party/webrtc/src/webrtc/base/bitbuffer.h
        erizo/src/third_party/webrtc/src/webrtc/base/buffer.h
        erizo/src/third_party/webrtc/src/webrtc/base/bytebuffer.cc
        erizo/src/third_party/webrtc/src/webrtc/base/bytebuffer.h
        erizo/src/third_party/webrtc/src/webrtc/base/byteorder.h
        erizo/src/third_party/webrtc/src/webrtc/base/callback.h
        erizo/src/third_party/webrtc/src/webrtc/base/checks.cc
        erizo/src/third_party/webrtc/src/webrtc/base/checks.h
        erizo/src/third_party/webrtc/src/webrtc/base/common.cc
        erizo/src/third_party/webrtc/src/webrtc/base/common.h
        erizo/src/third_party/webrtc/src/webrtc/base/constructormagic.h
        erizo/src/third_party/webrtc/src/webrtc/base/copyonwritebuffer.cc
        erizo/src/third_party/webrtc/src/webrtc/base/copyonwritebuffer.h
        erizo/src/third_party/webrtc/src/webrtc/base/criticalsection.cc
        erizo/src/third_party/webrtc/src/webrtc/base/criticalsection.h
        erizo/src/third_party/webrtc/src/webrtc/base/deprecation.h
        erizo/src/third_party/webrtc/src/webrtc/base/event.cc
        erizo/src/third_party/webrtc/src/webrtc/base/event.h
        erizo/src/third_party/webrtc/src/webrtc/base/event_tracer.cc
        erizo/src/third_party/webrtc/src/webrtc/base/event_tracer.h
        erizo/src/third_party/webrtc/src/webrtc/base/ignore_wundef.h
        erizo/src/third_party/webrtc/src/webrtc/base/logging.cc
        erizo/src/third_party/webrtc/src/webrtc/base/logging.h
        erizo/src/third_party/webrtc/src/webrtc/base/onetimeevent.h
        erizo/src/third_party/webrtc/src/webrtc/base/optional.cc
        erizo/src/third_party/webrtc/src/webrtc/base/optional.h
        erizo/src/third_party/webrtc/src/webrtc/base/platform_thread.cc
        erizo/src/third_party/webrtc/src/webrtc/base/platform_thread.h
        erizo/src/third_party/webrtc/src/webrtc/base/platform_thread_types.h
        erizo/src/third_party/webrtc/src/webrtc/base/random.h
        erizo/src/third_party/webrtc/src/webrtc/base/rate_limiter.cc
        erizo/src/third_party/webrtc/src/webrtc/base/rate_limiter.h
        erizo/src/third_party/webrtc/src/webrtc/base/rate_statistics.cc
        erizo/src/third_party/webrtc/src/webrtc/base/rate_statistics.h
        erizo/src/third_party/webrtc/src/webrtc/base/refcount.h
        erizo/src/third_party/webrtc/src/webrtc/base/refcountedobject.h
        erizo/src/third_party/webrtc/src/webrtc/base/safe_compare.h
        erizo/src/third_party/webrtc/src/webrtc/base/safe_conversions.h
        erizo/src/third_party/webrtc/src/webrtc/base/safe_conversions_impl.h
        erizo/src/third_party/webrtc/src/webrtc/base/sanitizer.h
        erizo/src/third_party/webrtc/src/webrtc/base/scoped_ref_ptr.h
        erizo/src/third_party/webrtc/src/webrtc/base/sequenced_task_checker.h
        erizo/src/third_party/webrtc/src/webrtc/base/sequenced_task_checker_impl.h
        erizo/src/third_party/webrtc/src/webrtc/base/stringencode.cc
        erizo/src/third_party/webrtc/src/webrtc/base/stringencode.h
        erizo/src/third_party/webrtc/src/webrtc/base/stringutils.cc
        erizo/src/third_party/webrtc/src/webrtc/base/stringutils.h
        erizo/src/third_party/webrtc/src/webrtc/base/thread_annotations.h
        erizo/src/third_party/webrtc/src/webrtc/base/thread_checker.h
        erizo/src/third_party/webrtc/src/webrtc/base/thread_checker_impl.cc
        erizo/src/third_party/webrtc/src/webrtc/base/thread_checker_impl.h
        erizo/src/third_party/webrtc/src/webrtc/base/timeutils.cc
        erizo/src/third_party/webrtc/src/webrtc/base/timeutils.h
        erizo/src/third_party/webrtc/src/webrtc/base/trace_event.h
        erizo/src/third_party/webrtc/src/webrtc/base/type_traits.h
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/h264_common.cc
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/h264_common.h
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/pps_parser.cc
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/pps_parser.h
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/sps_parser.cc
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/sps_parser.h
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/sps_vui_rewriter.cc
        erizo/src/third_party/webrtc/src/webrtc/common_video/h264/sps_vui_rewriter.h
        erizo/src/third_party/webrtc/src/webrtc/common_video/include/video_frame_buffer.h
        erizo/src/third_party/webrtc/src/webrtc/common_video/rotation.h
        erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/include/mock/mock_bitrate_controller.h
        erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/include/bitrate_controller.h
        erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/send_side_bandwidth_estimation.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/send_side_bandwidth_estimation.h
        erizo/src/third_party/webrtc/src/webrtc/modules/include/module.h
        erizo/src/third_party/webrtc/src/webrtc/modules/include/module_common_types.h
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/alr_detector.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/alr_detector.h
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/bitrate_prober.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/bitrate_prober.h
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/paced_sender.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/paced_sender.h
        erizo/src/third_party/webrtc/src/webrtc/modules/pacing/packet_router.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/include/bwe_defines.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/include/remote_bitrate_estimator.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/include/send_time_history.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/test/bwe_test_logging.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/test/bwe_test_logging.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/aimd_rate_control.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/aimd_rate_control.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/bwe_defines.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/inter_arrival.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/inter_arrival.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/overuse_detector.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/overuse_detector.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/overuse_estimator.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/overuse_estimator.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/remote_bitrate_estimator_abs_send_time.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/remote_bitrate_estimator_abs_send_time.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/remote_bitrate_estimator_single_stream.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/remote_bitrate_estimator_single_stream.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/remote_estimator_proxy.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/remote_estimator_proxy.h
        erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/send_time_history.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/flexfec_sender.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/receive_statistics.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/rtp_cvo.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/rtp_payload_registry.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/rtp_rtcp.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/rtp_rtcp_defines.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include/ulpfec_receiver.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet/common_header.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet/common_header.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet/rtpfb.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet/rtpfb.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet/transport_feedback.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet/transport_feedback.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/byte_io.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/fec_private_tables_bursty.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/fec_private_tables_random.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/flexfec_header_reader_writer.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/flexfec_header_reader_writer.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/forward_error_correction.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/forward_error_correction.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/forward_error_correction_internal.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/forward_error_correction_internal.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/receive_statistics_impl.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/receive_statistics_impl.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_h264.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_h264.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_video_generic.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_video_generic.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_vp8.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_vp8.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_vp9.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_format_vp9.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_header_extension.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_header_extension.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_header_extensions.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_header_extensions.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_packet.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_packet.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_packet_to_send.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_payload_registry.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_receiver_strategy.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_receiver_strategy.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_receiver_video.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_receiver_video.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_rtcp_config.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_utility.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtp_utility.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/time_util.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/time_util.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/ulpfec_generator.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/ulpfec_header_reader_writer.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/ulpfec_header_reader_writer.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/ulpfec_receiver_impl.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/ulpfec_receiver_impl.h
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/vp8_partition_aggregator.cc
        erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/vp8_partition_aggregator.h
        erizo/src/third_party/webrtc/src/webrtc/modules/video_coding/include/video_coding_defines.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/aligned_malloc.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/clock.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/critical_section_wrapper.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/event_wrapper.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/field_trial.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/field_trial_default.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/file_wrapper.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/logging.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/metrics.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/metrics_default.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/ntp_time.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/rw_lock_wrapper.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/static_instance.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include/trace.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/clock.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/field_trial_default.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/file_impl.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/logging.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/metrics_default.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/rw_lock.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/rw_lock_posix.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/rw_lock_posix.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/trace_impl.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/trace_impl.h
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/trace_posix.cc
        erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source/trace_posix.h
        erizo/src/third_party/webrtc/src/webrtc/common_types.cc
        erizo/src/third_party/webrtc/src/webrtc/common_types.h
        erizo/src/third_party/webrtc/src/webrtc/config.h
        erizo/src/third_party/webrtc/src/webrtc/logging_mac.cc
        erizo/src/third_party/webrtc/src/webrtc/typedefs.h
        erizo/src/third_party/webrtc/src/webrtc/video_frame.h
        erizoAPI/lib/json.hpp
        erizoAPI/addon.cc
        erizoAPI/AsyncPromiseWorker.cc
        erizoAPI/AsyncPromiseWorker.h
        erizoAPI/ConnectionDescription.cc
        erizoAPI/ConnectionDescription.h
        erizoAPI/ExternalInput.cc
        erizoAPI/ExternalInput.h
        erizoAPI/ExternalOutput.cc
        erizoAPI/ExternalOutput.h
        erizoAPI/FuturesManager.cc
        erizoAPI/FuturesManager.h
        erizoAPI/IOThreadPool.cc
        erizoAPI/IOThreadPool.h
        erizoAPI/MediaDefinitions.h
        erizoAPI/MediaStream.cc
        erizoAPI/MediaStream.h
        erizoAPI/OneToManyProcessor.cc
        erizoAPI/OneToManyProcessor.h
        erizoAPI/OneToManyTranscoder.cc
        erizoAPI/OneToManyTranscoder.h
        erizoAPI/SyntheticInput.cc
        erizoAPI/SyntheticInput.h
        erizoAPI/ThreadPool.cc
        erizoAPI/ThreadPool.h
        erizoAPI/WebRtcConnection.cc
        erizoAPI/WebRtcConnection.h
        node_modules/fsevents/src/async.cc
        node_modules/fsevents/src/constants.cc
        node_modules/fsevents/src/methods.cc
        node_modules/fsevents/src/storage.cc
        node_modules/fsevents/src/thread.cc
        node_modules/fsevents/fsevents.cc
        node_modules/nan/nan.h
        node_modules/nan/nan_callbacks.h
        node_modules/nan/nan_callbacks_12_inl.h
        node_modules/nan/nan_callbacks_pre_12_inl.h
        node_modules/nan/nan_converters.h
        node_modules/nan/nan_converters_43_inl.h
        node_modules/nan/nan_converters_pre_43_inl.h
        node_modules/nan/nan_define_own_property_helper.h
        node_modules/nan/nan_implementation_12_inl.h
        node_modules/nan/nan_implementation_pre_12_inl.h
        node_modules/nan/nan_json.h
        node_modules/nan/nan_maybe_43_inl.h
        node_modules/nan/nan_maybe_pre_43_inl.h
        node_modules/nan/nan_new.h
        node_modules/nan/nan_object_wrap.h
        node_modules/nan/nan_persistent_12_inl.h
        node_modules/nan/nan_persistent_pre_12_inl.h
        node_modules/nan/nan_private.h
        node_modules/nan/nan_string_bytes.h
        node_modules/nan/nan_typedarray_contents.h
        node_modules/nan/nan_weak.h
        node_modules/node-uuid/benchmark/benchmark-native.c docs/cpptt/test_std_promise.cpp zhangbin/ej/EjConnection.cpp zhangbin/ej/EjConnection.h zhangbin/ej/EjWebrtcConn.cpp zhangbin/ej/EjWebrtcConn.h zhangbin/ej/EjMediaStream.cpp zhangbin/ej/EjMediaStream.h zhangbin/ej/EjType.h zhangbin/ej/EjThreadPool.cpp zhangbin/ej/EjThreadPool.h zhangbin/ej/EjSingleton.h)
