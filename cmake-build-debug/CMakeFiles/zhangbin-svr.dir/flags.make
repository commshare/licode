# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# compile CXX with /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++
CXX_FLAGS = -g -Wall -std=c++11  -DBOOST_THREAD_PROVIDES_FUTURE -DBOOST_THREAD_PROVIDES_FUTURE_CONTINUATION -DBOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY -g -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.15.sdk   -std=gnu++14

CXX_DEFINES = -D_DEBUG -D_USEMAC

CXX_INCLUDES = -I/usr/local/include -I/usr/local/include} -I/usr/local/opt/openssl/include -I/usr/local/Cellar/libevent/2.1.11_1/include -I/usr/local/Cellar/libevent/2.1.11_1/include/event2 -I/usr/local/opt/zlib/include -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/thirdparty/spdlog/include -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/thirdparty/nlohmann -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/thirdparty -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/thirdparty/tclap-1.2.2/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/common -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/common/OSSupport -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/include -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/include/OSSupport -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/server -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/server/Protocol -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/thirdparty/polarssl-1.3.9/include -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/thirdparty/jsoncpp-1.6.5/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/openssl-1.0.1u-OSX/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erzio -I/Users/zhangbin/tet/licodelllcode/licode/zhangbin/nrappkit/src -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/test -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/test/fast -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/test/fast/Aliases -I"/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/test/fast/Listing paths" -I"/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/test/fast/Listing versions" -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/test/slow -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/libplatform -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/aix-gcc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/aix64-gcc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/BSD-x86 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/BSD-x86_64 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/darwin-i386-cc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/darwin64-x86_64-cc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-aarch64 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-armv4 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-elf -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-ppc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-ppc64 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-x32 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux-x86_64 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux32-s390x -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/linux64-s390x -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/solaris-x86-gcc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/solaris64-x86_64-gcc -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/VC-WIN32 -I/Users/zhangbin/tet/licodelllcode/licode/build/libdeps/nvm/versions/node/v6.14.4/include/node/openssl/archs/VC-WIN64A -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include/gmock -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include/gmock/internal -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include/gmock/internal/custom -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include/gtest -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include/gtest/internal -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/include/gtest/internal/custom -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googlemock/include/gmock/internal/custom -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googlemock/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/include/gtest/internal/custom -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/samples -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/src -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/gmock/src/gtest/googletest/xcode/Samples/FrameworkSample -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/crypto -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/event -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/ice -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/log -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/net -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/net/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/plugin -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/plugin/api -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/share -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/stats -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/stun -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/stun/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/util -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/include/util/libekr -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/crypto -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/ice -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/net -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/net/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/stun/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/build/debug/libdeps/nicer/src/project_nicer/src/testua -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/bandwidth -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/dtls -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/lib -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/media -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/media/codecs -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/media/mixers -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/pipeline -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/rtp -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/stats -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/erizo/thread -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/examples -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/examples/pc -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/test/utils -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/base -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/base/analytics -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/common_video -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/common_video/h264 -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/common_video/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/bitrate_controller/include/mock -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/pacing -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/remote_bitrate_estimator/test -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/rtp_rtcp/source/rtcp_packet -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/video_coding -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/modules/video_coding/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/system_wrappers/include -I/Users/zhangbin/tet/licodelllcode/licode/erizo/src/third_party/webrtc/src/webrtc/system_wrappers/source -I/Users/zhangbin/tet/licodelllcode/licode/erizoAPI -I/Users/zhangbin/tet/licodelllcode/licode/erizoAPI/lib -I/Users/zhangbin/tet/licodelllcode/licode/node_modules/nan 
