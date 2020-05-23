#ifndef ERIZO_SRC_ERIZO_WEBRTCCONNECTION_H_
#define ERIZO_SRC_ERIZO_WEBRTCCONNECTION_H_

#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/thread/mutex.hpp>

#include <string>
#include <map>
#include <vector>

#include "./logger.h"
#include "./SdpInfo.h"
#include "./MediaDefinitions.h"
#include "./Transport.h"
#include "./Stats.h"
#include "bandwidth/BandwidthDistributionAlgorithm.h"
#include "bandwidth/ConnectionQualityCheck.h"
#include "pipeline/Pipeline.h"
#include "thread/Worker.h"
#include "thread/IOWorker.h"
#include "rtp/RtcpProcessor.h"
#include "rtp/RtpExtensionProcessor.h"
#include "lib/Clock.h"
#include "pipeline/Handler.h"
#include "pipeline/HandlerManager.h"
#include "pipeline/Service.h"
#include "rtp/PacketBufferService.h"

namespace erizo {

constexpr std::chrono::milliseconds kBitrateControlPeriod(100);
constexpr uint32_t kDefaultVideoSinkSSRC = 55543;
constexpr uint32_t kDefaultAudioSinkSSRC = 44444;

class Transport;
class TransportListener;
class IceConfig;
class MediaStream;

/**
 * WebRTC Events
 */
enum WebRTCEvent {
  CONN_INITIAL = 101, CONN_STARTED = 102, CONN_GATHERED = 103, CONN_READY = 104, CONN_FINISHED = 105,
  CONN_CANDIDATE = 201, CONN_SDP = 202, CONN_SDP_PROCESSED = 203,
  CONN_FAILED = 500
};

class WebRtcConnectionEventListener {
 public:
    virtual ~WebRtcConnectionEventListener() {
    }
    virtual void notifyEvent(WebRTCEvent newEvent, const std::string& message) = 0;
};

/**
 * A WebRTC Connection. This class represents a WebRTC Connection that can be established with other peers
 * via a SDP negotiation
 * it comprises all the necessary Transport components.
 */
class WebRtcConnection: public TransportListener, public LogContext, public HandlerManagerListener,
                        public std::enable_shared_from_this<WebRtcConnection>, public Service {
  DECLARE_LOGGER();

 public:
  typedef typename Handler::Context Context;

  /** 创建一个控的wc，没有任何配置的
   * Constructor.
   * Constructs an empty WebRTCConnection without any configuration.
   */
  WebRtcConnection(std::shared_ptr<Worker> worker, std::shared_ptr<IOWorker> io_worker,
      const std::string& connection_id, const IceConfig& ice_config,
      const std::vector<RtpMap> rtp_mappings, const std::vector<erizo::ExtMap> ext_mappings,
      bool enable_connection_quality_check, WebRtcConnectionEventListener* listener);
  /**
   * Destructor.
   */
  virtual ~WebRtcConnection();
  /** 通过开始ICE的候选人集合过程，初始化wc
   * Inits the WebConnection by starting ICE Candidate Gathering.
   * @return True if the candidates are gathered.
   */
  bool init();
  //关闭包括同步和异步
  void close();
  void syncClose();

  //这个要是设置给对端 sdp
  boost::future<void> setRemoteSdpInfo(std::shared_ptr<SdpInfo> sdp);
  /**
   * Sets the SDP of the remote peer.
   * @param sdp The SDP.
   * @return true if the SDP was received correctly.
   */
  boost::future<void> setRemoteSdp(const std::string &sdp);

  //offer是什么？
  boost::future<void> createOffer(bool video_enabled, bool audio_enabled, bool bundle);

  boost::future<void> addRemoteCandidate(std::string mid, int mLineIndex, std::string sdp);

  /** 同步的
   * Add new remote candidate (from remote peer).
   * @param sdp The candidate in SDP format.  候选人是用sdp 来代表的
   * @return true if the SDP was received correctly.
   */
  bool addRemoteCandidateSync(std::string mid, int mLineIndex, std::string sdp);

  /**
   * Obtains the local SDP.
   * @return The SDP as a SdpInfo.
   */
  boost::future<std::shared_ptr<SdpInfo>> getLocalSdpInfo();
  std::shared_ptr<SdpInfo> getLocalSdpInfoSync();
  /**
   * Copy some SdpInfo data to local SdpInfo
   */
  void copyDataToLocalSdpIndo(std::shared_ptr<SdpInfo> sdp_info);
  /**
   * Obtains the local SDP.
   * @return The SDP as a string.
   */
  std::string getLocalSdp();

  /**
   * Sets the Event Listener for this WebRtcConnection
   */
  void setWebRtcConnectionEventListener(WebRtcConnectionEventListener* listener);

  /** 这个wce代表的是ice链接的状态
   * Gets the current state of the Ice Connection
   * @return
   */
  WebRTCEvent getCurrentState();

  void onTransportData(std::shared_ptr<DataPacket> packet, Transport *transport) override;

  //更新某个传输的传输状态么？
  void updateState(TransportState state, Transport * transport) override;

  void onCandidate(const CandidateInfo& cand, Transport *transport) override;

  void setMetadata(std::map<std::string, std::string> metadata);

  void send(std::shared_ptr<DataPacket> packet);
  //boost的future和std的function一起用
  boost::future<void> asyncTask(std::function<void(std::shared_ptr<WebRtcConnection>)> f);

  bool isAudioMuted() { return audio_muted_; }
  bool isVideoMuted() { return video_muted_; }

  //增加或者删除ms，这些都是future操作
  boost::future<void> addMediaStream(std::shared_ptr<MediaStream> media_stream);
  boost::future<void> removeMediaStream(const std::string& stream_id);

  //遍历ms，有同步和异步
  void forEachMediaStream(std::function<void(const std::shared_ptr<MediaStream>&)> func);
  boost::future<void> forEachMediaStreamAsync(std::function<void(const std::shared_ptr<MediaStream>&)> func);
  void forEachMediaStreamAsyncNoPromise(std::function<void(const std::shared_ptr<MediaStream>&)> func);
  //测试用
  void setTransport(std::shared_ptr<Transport> transport);  // Only for Testing purposes

  std::shared_ptr<Stats> getStatsService() { return stats_; }

  RtpExtensionProcessor& getRtpExtensionProcessor() { return extension_processor_; }

  std::shared_ptr<Worker> getWorker() { return worker_; }

  inline std::string toLog() {
    return "id: " + connection_id_ + ", " + printLogContext();
  }

  bool isPipelineInitialized() { return pipeline_initialized_; }
  bool isRunning() { return pipeline_initialized_ && sending_; }
  Pipeline::Ptr getPipeline() { return pipeline_; }
  void read(std::shared_ptr<DataPacket> packet);
  void write(std::shared_ptr<DataPacket> packet);
  void notifyUpdateToHandlers() override;
  ConnectionQualityLevel getConnectionQualityLevel();
  //最近是否有丢包么
  bool werePacketLossesRecently();
  void getJSONStats(std::function<void(std::string)> callback);

 private:
  bool createOfferSync(bool video_enabled, bool audio_enabled, bool bundle);
  boost::future<void> processRemoteSdp();
  boost::future<void> setRemoteSdpsToMediaStreams();
  std::string getJSONCandidate(const std::string& mid, const std::string& sdp);
  void trackTransportInfo();
  //来自于传输的rtcp和remb
  void onRtcpFromTransport(std::shared_ptr<DataPacket> packet, Transport *transport);
  void onREMBFromTransport(RtcpHeader *chead, Transport *transport);
  void maybeNotifyWebRtcConnectionEvent(const WebRTCEvent& event, const std::string& message);
  //初始化pipeline
  void initializePipeline();

 protected:
  //wre代表状态，这个是全局状态的
  std::atomic<WebRTCEvent> global_state_;

 private:
  std::string connection_id_; //链接id？？？
  bool audio_enabled_;
  bool video_enabled_;
  bool trickle_enabled_;
  bool slide_show_mode_;
  bool sending_; //代表处理中
  int bundle_;
  WebRtcConnectionEventListener* conn_event_listener_;
  IceConfig ice_config_;
  std::vector<RtpMap> rtp_mappings_;
  RtpExtensionProcessor extension_processor_;
  boost::condition_variable cond_;
  //代表音视频传输
  std::shared_ptr<Transport> video_transport_, audio_transport_;

  std::shared_ptr<Stats> stats_;
  //保护状态更新
  boost::mutex update_state_mutex_;
  boost::mutex event_listener_mutex_;

  std::shared_ptr<Worker> worker_;
  std::shared_ptr<IOWorker> io_worker_;
  std::vector<std::shared_ptr<MediaStream>> media_streams_;
  std::shared_ptr<SdpInfo> remote_sdp_;
  std::shared_ptr<SdpInfo> local_sdp_;
  bool audio_muted_;
  bool video_muted_;
  bool first_remote_sdp_processed_;

  std::unique_ptr<BandwidthDistributionAlgorithm> distributor_;
  ConnectionQualityCheck connection_quality_check_;
  bool enable_connection_quality_check_;
  Pipeline::Ptr pipeline_;
  bool pipeline_initialized_;
  std::shared_ptr<HandlerManager> handler_manager_;
};

class ConnectionPacketReader : public InboundHandler {
 public:
  explicit ConnectionPacketReader(WebRtcConnection *connection) : connection_{connection} {}

  void enable() override {}
  void disable() override {}

  std::string getName() override {
    return "connection-reader";
  }

  void read(Context *ctx, std::shared_ptr<DataPacket> packet) override {
    connection_->read(std::move(packet));
  }

  void notifyUpdate() override {
  }

 private:
  WebRtcConnection *connection_;
};

class ConnectionPacketWriter : public OutboundHandler {
 public:
  explicit ConnectionPacketWriter(WebRtcConnection *connection) : connection_{connection} {}

  void enable() override {}
  void disable() override {}

  std::string getName() override {
    return "connection-writer";
  }

  void write(Context *ctx, std::shared_ptr<DataPacket> packet) override {
    connection_->write(std::move(packet));
  }

  void notifyUpdate() override {
  }

 private:
  WebRtcConnection *connection_;
};

}  // namespace erizo
#endif  // ERIZO_SRC_ERIZO_WEBRTCCONNECTION_H_
