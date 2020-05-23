//
// Created by zhangbin on 2019/11/24.
//

#include "EjWebrtcConn.h"


#include <future>  // NOLINT
#include <boost/thread/future.hpp>  // NOLINT
#include <boost/thread/mutex.hpp>  // NOLINT


namespace EJ{
EjWebrtcConn::EjWebrtcConn()
:closed_{false},id_{"undefined"}{

}
EjWebrtcConn::~EjWebrtcConn() {
  close();

}
void EjWebrtcConn::close() {

  if(closed_)
  {
    HLOGE("already closed webrtcconn");
    return;
  }
  if(me_)
  {
    me_->setWebRtcConnectionEventListener(nullptr);
    me_->close();
    me_.reset();
  }
  //boost::mutex::scoped_lock lock(mutex);


}
std::string EjWebrtcConn::toLog() {
  return "id:" + id_;
}

bool EjWebrtcConn::Init()
{

}
bool EjWebrtcConn::New() {
  std::string wrtcid ="";
  std::string stunServer = "";
  int stunPort =0;
  int minPort =0;
  int maxPort =0;
  bool trikele = false;
  bool user_nicer = false;
  bool enable_conneciton_quality_check = 0;
  std::string media_config_string ;
  json media_config = json::parse(media_config_string);//解析为json格式
  std::vector<erizo::RtpMap> rtp_mappings;

}
}