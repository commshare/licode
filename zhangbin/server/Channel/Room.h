#pragma once

class cPeer;
typedef std::shared_ptr<cPeer> cParticipantPtr;
typedef std::map<int,cParticipantPtr> cParticipantPtrs;
typedef std::map<int,cPeer*> cParticipants;

class cChannelObserver;

#include "Peer.h"

class cRoom : public cPeer::cListener
{
public:
	class Listener
	{
	public:
		virtual void onRoomClosed() = 0;
	};

public:
	cRoom(const AString &name, cChannelObserver *callback);
	~cRoom();
	AString getName();
	int getSize();

	//加退房间
	bool onPeerJoined(const int &a_ClientID);
	bool onPeerLeaved(const int &a_ClientID);	

	//收到对端？房间里面有几个对端？可能可以有多个，以id区分
	void onMsgFromPeer(const int &a_FromID, const int &a_TargetID, const AString &a_Msg);

	/* Pure virtual methods inherited from RTC::Peer::Listener. */
public:
  //这个是单播
	virtual void SendMsgToPeer(const int &a_FromID, const int &a_TargetID, const AString &a_Msg);
	//广播：通知所有的客户端么? TODO
	virtual void BroadcastJoinChannel(const int &a_ClientID);

private:
  //已经链接到了这台服务器，但是还没有分配给一个cWorld 的客户端。
	/** Clients that are connected to the server and not yet assigned to a cWorld. */
	cParticipantPtrs m_Clients;

	//一个channel就是一个房间
	AString m_ChannelName;
	//对这个房间的观察者
	cChannelObserver *m_Callback;
};

