#include "stdafx.h"
#include "Peer.h"
#include "Room.h"
#include <json/json.h>
#include <OneToManyProcessor.h>
#include <ThreadPool.h>

#define  MAX_PORT 59999
#define  MIN_PORT 10000

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";
// Sye Bye
const char kConnectSyeBye[] = "BYE";


cPeer::WebRtcEventListener::WebRtcEventListener(int id, cPeer*a_Participant)
	: id(id)
	, peer(a_Participant)
{

}

void cPeer::WebRtcEventListener::notifyEvent(erizo::WebRTCEvent newEvent, const AString& message)
{
	if (state == newEvent)
	{
		return;
	}

	state = newEvent;

	switch (newEvent)
	{
	case erizo::CONN_INITIAL:
		break;
	case erizo::CONN_STARTED:
		break;
	case erizo::CONN_SDP:
	case erizo::CONN_GATHERED:
	{
		AString localsdp = message;
		Json::StyledWriter writer;
		Json::Value jmessage;
		jmessage[kSessionDescriptionSdpName] = localsdp;
		jmessage[kSessionDescriptionTypeName] = peer->getId() == id ? "answer" : "offer";
		peer->SendMsgToPeer(id, writer.write(jmessage));

//		::MessageBoxA(nullptr, localsdp.c_str(), "", MB_OK);
	}
	break;
	case erizo::CONN_READY:
	{
		if (peer->getId() == id)
		{
			peer->onJoinedChannel();
		}
		else
		{
			peer->onSubscribedSuccess(id);
		}
	}
		break;
	case erizo::CONN_FINISHED:
		delete this;
		break;
	case erizo::CONN_CANDIDATE:
	{
		peer->SendMsgToPeer(id, message);
	}
	break;
	case erizo::CONN_FAILED:
		break;
	default:
		break;
	}
}

cPeer::cPeer(const int &id, cListener *listener)
	: id(id)
	, listener(listener)
{
	muxer = new erizo::OneToManyProcessor();
}


cPeer::~cPeer()
{
	for (auto listener : eventListeners)
	{
		if (listener.first != id)
		{
			muxer->removeSubscriber(Printf("%d", listener.first));
			delete listener.second;
		}
	}

	eventListeners.clear();

	delete muxer;
	muxer = nullptr;
}

int cPeer::getId()
{
	return id;
}

bool cPeer::isReady()
{
	return ready;
}

bool cPeer::doJoin()
{
	erizo::IceConfig iceConfig;
	iceConfig.turn_server = "admin.mice007.com";
	iceConfig.turn_username = "username1";
	iceConfig.turn_pass = "password1";
	iceConfig.stun_server = "admin.mice007.com";
	iceConfig.stun_port = 3478;
	iceConfig.turn_port = 3478;
	iceConfig.min_port = MIN_PORT;
	iceConfig.max_port = MAX_PORT;
	eventListener = new WebRtcEventListener(id, this);
	//创建一个WebRtcConnection，现在参数多了 TODO
  std::shared_ptr<erizo::IOWorker> io_worker;
  io_worker = std::make_shared<erizo::IOWorker>();
  io_worker->start();


  std::shared_ptr<erizo::SimulatedClock> simulated_clock;
  std::shared_ptr<erizo::SimulatedWorker> simulated_worker;

  simulated_clock = std::make_shared<erizo::SimulatedClock>();
  simulated_worker = std::make_shared<erizo::SimulatedWorker>(simulated_clock);
  simulated_worker->start();




  publisher = new erizo::WebRtcConnection(simulated_worker,io_worker,Printf("%d", id),iceConfig,
    std::vector<erizo::RtpMap>(),
      std::vector<erizo::ExtMap>(),
        true,eventListener);
//////////

//TODO 这个是原来 老的
 // publisher = new erizo::WebRtcConnection(Printf("%d", id), true, true, iceConfig, eventListener);

  //这俩也是原来老的，现在怎么办  TODO
#if true
  publisher->
  publisher->setAudioSink(muxer);//音频接收者
	publisher->setVideoSink(muxer);//视频接收者
	muxer->setPublisher(publisher);
#else

  //一个source 是publisher，一个sink是subscriber
  //subscriber = std::make_shared<MockSubscriber>();
  //这样就可以设置给muxer了
  muxer->setPublisher(publisher);
  muxer->addSubscriber(subscriber, kArbitraryPeerId);

  erizo::MediaSource* ms = dynamic_cast<erizo::MediaSource*>(publisher);

#endif
	return publisher->init();
}

bool cPeer::doLeave()
{
	ready = false;
	return true;
}

void cPeer::onJoinedChannel()
{
	ready = true;

	listener->BroadcastJoinChannel(id);
}

void cPeer::onSubscribedSuccess(const int &a_TargetID)
{
// 	auto it = subscribers.find(a_TargetID);
// 	if (it != subscribers.end())
// 	{
// 		erizo::WebRtcConnection *newConn = it->second;
// 		if (newConn)
// 		{
// 			muxer->addSubscriber(newConn, Printf("%d", a_TargetID));
// 		}
// 	}
}

bool cPeer::onSubscribed(const int &a_TargetID)
{
	erizo::IceConfig iceConfig;
	iceConfig.turn_server = "admin.mice007.com";
	iceConfig.turn_username = "username1";
	iceConfig.turn_pass = "password1";
	iceConfig.stun_server = "admin.mice007.com";
	iceConfig.stun_port = 3478;
	iceConfig.turn_port = 3478;
	iceConfig.min_port = MIN_PORT;
	iceConfig.max_port = MAX_PORT;
	WebRtcEventListener *listener = new WebRtcEventListener(a_TargetID, this);

	 //add begin
  ThreadPool* thread_pool ;

  IOThreadPool* io_thread_pool
  std::shared_ptr<erizo::Worker> worker = thread_pool->me->getLessUsedWorker();
  std::shared_ptr<erizo::IOWorker> io_worker = io_thread_pool->me->getLessUsedIOWorker();
	//add end
	erizo::WebRtcConnection *newConn = new erizo::WebRtcConnection(Printf("%d", a_TargetID), true, true, iceConfig, listener);
	//解决音频没声音问题;
	newConn->localSdp_.setOfferSdp(publisher->localSdp_);

	//新增新的订阅者和事件观测者
	subscribers[a_TargetID] = newConn;
	eventListeners[a_TargetID] = listener;
	newConn->init();
	
	muxer->addSubscriber(newConn, Printf("%d", a_TargetID));
	newConn->createOffer();
	return true;
}

bool cPeer::onUnSubscribed(const int &a_TargetID)
{
	if (muxer)
	{
		muxer->removeSubscriber(Printf("%d", a_TargetID));
	}

	//移除存在的订阅者
	auto itSub = subscribers.find(a_TargetID);
	if (itSub != subscribers.end())
	{
		subscribers.erase(itSub);
	}

	//移除存在的事件观测者
	auto it = eventListeners.find(a_TargetID);
	if (it != eventListeners.end())
	{
		delete it->second;
		eventListeners.erase(it);
	}

	return true;
}

void cPeer::onMsgFromPeer(const int &a_TargetID, const AString &msg)
{
	ASSERT(!msg.empty());

	if (msg.compare(kConnectSyeBye) == 0)
	{
		return;
	}

	
	Json::Reader reader;
	Json::Value jmessage;
	if (!reader.parse(msg, jmessage)) {
		LOGWARNING("Received unknown message. ", msg.c_str());
		return;
	}

	AString type = jmessage[kSessionDescriptionTypeName].asString();

	if (type == "subscribe")
	{
		int clientID = jmessage["clientID"].asInt();
		onSubscribed(clientID);
	}
	else if (type == "offer" || type == "answer") 
	{
		AString sdp = jmessage[kSessionDescriptionSdpName].asString();
		if (sdp.empty()) {
			LOGWARNING("Can't parse received session description message.");
			return;
		}

		erizo::WebRtcConnection * newConn = getConnectionById(a_TargetID);
		if (!newConn)
		{
			return;
		}

		newConn->setRemoteSdp(sdp);
	}
	else 
	{
		AString sdp_mid = jmessage[kCandidateSdpMidName].asString();
		int sdp_mlineindex = jmessage[kCandidateSdpMlineIndexName].asInt();
		AString sdp = jmessage[kCandidateSdpName].asString();
		if (sdp_mid.empty() || sdp.empty()) {
			LOGWARNING("Can't parse received message.");
			return;
		}

		erizo::WebRtcConnection * newConn = getConnectionById(a_TargetID);
		if (!newConn)
		{
			return;
		}

		newConn->addRemoteCandidate(sdp_mid, sdp_mlineindex, sdp);
	}
}

void cPeer::SendMsgToPeer(const int &a_TargetID, const AString &a_Msg)
{
	listener->SendMsgToPeer(id, a_TargetID, a_Msg);
}

erizo::WebRtcConnection *cPeer::getConnectionById(const int &a_TargetID)
{
	if (a_TargetID == id)
	{
		return publisher;
	}
	else
	{
		auto it = subscribers.find(a_TargetID);
		if (it != subscribers.end())
		{
			return it->second;
		}
	}

	return nullptr;
}
