//
// Created by zhangbin on 2019/11/24.
//

#ifndef LICODE_EJWEBRTCCONN_H
#define LICODE_EJWEBRTCCONN_H

#include <boost/variant.hpp>
#include <WebRtcConnection.h>
#include "FuturesManager.h"
#include "MediaDefinitions.h"
#include "OneToManyProcessor.h"
#include "ConnectionDescription.h"
#include "EjType.h"

//结果变量
typedef boost::variant<std::string, std::shared_ptr<erizo::SdpInfo>> ResultVariant;

//结果对 TODO 这个怎么实现？？？
//typedef std::pair<Nan::Persistent<v8::Promise::Resolver> *, ResultVariant> ResultPair;


namespace EJ
{
//封装erzio的webrtcconnection的类
class EjWebrtcConn  : public erizo::WebRtcConnectionEventListener {
public:
  DECLARE_LOGGER();
  std::shared_ptr<erizo::WebRtcConnection> me_; //封装的这个
  std::queue<int> event_status;
  std::queue<std::string> event_messages;
  //TODO 结果存队列，依次执行么？
  //std::queue<ResultPair> futures;
  FuturesManager futures_manager_;//这个牛

  boost::mutex mutex;

private:
  EjWebrtcConn();
  ~EjWebrtcConn();

  std::string toLog();
  void close();
  bool closed_;
  std::string id_;

  //todo 这个怎么搞
//  Nan::Callback *event_callback_;
//  uv_async_t *async_;
//  uv_async_t *future_async_;

   //实现的纯虚
  virtual void notifyEvent(erizo::WebRTCEvent event,
                           const std::string& message = "");

  //todo 这个怎么搞
//  virtual void notifyFuture(Nan::Persistent<v8::Promise::Resolver> *persistent,
//                            ResultVariant result = ResultVariant());

//下面是一些对外的api


  /*
   * Constructor.
   * Constructs an empty WebRtcConnection without any configuration.
   */
bool New();

  /*
   * Closes the webRTC connection.
   * The object cannot be used after this call.
   */
void Close();
  /*
    * Inits the WebRtcConnection and passes the callback to get Events.
    * Returns true if the candidates are gathered.
    */
bool Init();
  /*
   * Creates an SDP Offer
   * Param: No params.
   * Returns true if the process has started successfully.
   */
bool CreateOffer();

  /*
   * Sets the SDP of the remote peer.
   * Param: the SDP.
   * Returns true if the SDP was received correctly.
   */

bool SetRemoteDescription();
  /*
   * Gets the SDP of the local peer.
   * Param: the SDP.
   * Returns true if the SDP was received correctly.
   */
bool  GetLocalDescription();

  /*
   * Sets the SDP of the remote peer.
   * Param: the SDP.
   * Returns true if the SDP was received correctly.
   */
bool SetRemoteSdp();
};
}



#endif //LICODE_EJWEBRTCCONN_H
