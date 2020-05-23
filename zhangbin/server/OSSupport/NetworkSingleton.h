
// NetworkSingleton.h

// Declares the cNetworkSingleton class representing the storage for global data pertaining to network API
// such as a list of all connections, all listening sockets and the LibEvent dispatch thread.

// This is an internal header, no-one outside OSSupport should need to include it; use Network.h instead;
// the only exception being the main app entrypoint that needs to call Terminate before quitting.





#pragma once

#include "Network.h"
#include "CriticalSection.h"
#include "event.h" //libevent ？？





// fwd:
struct event_base;
struct evdns_base;
class cTCPLinkImpl;
typedef std::shared_ptr<cTCPLinkImpl> cTCPLinkImplPtr;
typedef std::vector<cTCPLinkImplPtr> cTCPLinkImplPtrs;
class cServerHandleImpl;
typedef std::shared_ptr<cServerHandleImpl> cServerHandleImplPtr;
typedef std::vector<cServerHandleImplPtr> cServerHandleImplPtrs;
class cHostnameLookup;
typedef std::shared_ptr<cHostnameLookup> cHostnameLookupPtr;
typedef std::vector<cHostnameLookupPtr> cHostnameLookupPtrs;
class cIPLookup;
typedef std::shared_ptr<cIPLookup> cIPLookupPtr;
typedef std::vector<cIPLookupPtr> cIPLookupPtrs;





class cNetworkSingleton
{
public:
	~cNetworkSingleton();

	/** Returns the singleton instance of this class */
	static cNetworkSingleton & Get(void);

	/** Terminates all network-related threads.
	To be used only on app shutdown.
	MSVC runtime requires that the LibEvent networking be shut down before the main() function is exitted; this is the way to do it. */
	void Terminate(void);

	/** Returns the main LibEvent handle for event registering. */
	event_base * GetEventBase(void) { return m_EventBase; }

	/** Returns the LibEvent handle for DNS lookups. */
	evdns_base * GetDNSBase(void) { return m_DNSBase; }

	/** Adds the specified hostname lookup to m_HostnameLookups.
	Used by the underlying lookup implementation when a new lookup is initiated. */
	void AddHostnameLookup(cHostnameLookupPtr a_HostnameLookup);

	/** Removes the specified hostname lookup from m_HostnameLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveHostnameLookup(const cHostnameLookup * a_HostnameLookup);

	/** Adds the specified IP lookup to M_IPLookups.
	Used by the underlying lookup implementation when a new lookup is initiated. */
	void AddIPLookup(cIPLookupPtr a_IPLookup);

	/** Removes the specified IP lookup from m_IPLookups.
	Used by the underlying lookup implementation when the lookup is finished. */
	void RemoveIPLookup(const cIPLookup * a_IPLookup);

	/** Adds the specified link to m_Connections.
	Used by the underlying link implementation when a new link is created. */
	void AddLink(cTCPLinkImplPtr a_Link);

	/** Removes the specified link from m_Connections.
	Used by the underlying link implementation when the link is closed / errored. */
	void RemoveLink(const cTCPLinkImpl * a_Link);

	/** Adds the specified link to m_Servers.
	Used by the underlying server handle implementation when a new listening server is created.
	Only servers that succeed in listening are added. */
	void AddServer(cServerHandleImplPtr a_Server);

	/** Removes the specified server from m_Servers.
	Used by the underlying server handle implementation when the server is closed. */
	void RemoveServer(const cServerHandleImpl * a_Server);

protected:
  //驱动事件循环的libevent 容器？
	/** The main LibEvent container for driving the event loop. */
	event_base * m_EventBase;

	//查询dns的？
	/** The LibEvent handle for doing DNS lookups. */
	evdns_base * m_DNSBase;

	//所有客户端链接的容器，包括等待中的链接？
	/** Container for all client connections, including ones with pending-connect. */
	cTCPLinkImplPtrs m_Connections;

	//所有的有效的服务器的容器
	/** Container for all servers that are currently active. */
	cServerHandleImplPtrs m_Servers;

	/** Container for all pending hostname lookups. */
	cHostnameLookupPtrs m_HostnameLookups;

	/** Container for all pending IP lookups. */
	cIPLookupPtrs m_IPLookups;

	//多线程访问用的互斥保护类
	/** Mutex protecting all containers against multithreaded access. */
	cCriticalSection m_CS;

	/** Set to true if Terminate has been called. */
	volatile bool m_HasTerminated;

	// 主 libevent 循环所运行的线程
	/** The thread in which the main LibEvent loop runs. */
	std::thread m_EventLoopThread;


	//这个是构造函数？
	/** Initializes the LibEvent internals. */
	cNetworkSingleton(void);

	/** Converts LibEvent-generated log events into log messages in MCS log. */
	static void LogCallback(int a_Severity, const char * a_Msg);

	// libevnet的event 分发循环运行在这里
	/** Implements the thread that runs LibEvent's event dispatcher loop. */
	static void RunEventLoop(cNetworkSingleton * a_Self);
};









