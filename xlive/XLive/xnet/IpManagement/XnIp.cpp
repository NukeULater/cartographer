#include "stdafx.h"

#include "XnIp.h"
#include "..\..\Cryptography\Rc4.h"
#include "H2MOD\Modules\Config\Config.h"
#include "H2MOD\Modules\Achievements\Achievements.h"
#include "H2MOD\Modules\Networking\NetworkSession\NetworkSession.h"

CXnIp ipManager;
XECRYPT_RC4_STATE Rc4StateRand;

extern int WINAPI XSocketSendTo(SOCKET s, const char *buf, int len, int flags, sockaddr *to, int tolen);

/*
NOTE:
	Check inside Tweaks.cpp for removeXNetSecurity
*/

bool sockAddrInEqual(sockaddr_in* a1, sockaddr_in* a2)
{
	return (a1->sin_addr.s_addr == a2->sin_addr.s_addr && a1->sin_port == a2->sin_port);
}

bool sockAddrInIsNull(sockaddr_in* a1)
{
	return a1->sin_addr.s_addr == 0 && a1->sin_port == 0;
}

void CXnIp::GetStartupParamsAndUpdate(const XNetStartupParams* netStartupParams)
{
	memcpy(&startupParams, netStartupParams, sizeof(XNetStartupParams));

	if (startupParams.cfgSecRegMax == 0)
		startupParams.cfgSecRegMax = 32; // default 32 XNet connections

	if (XnIPs)
		delete[] XnIPs;

	XnIPs = new	XnIp[startupParams.cfgSecRegMax];
	SecureZeroMemory(XnIPs, startupParams.cfgSecRegMax * sizeof(XnIp));

	if (startupParams.cfgQosDataLimitDiv4 == 0)
		startupParams.cfgQosDataLimitDiv4 = 64; // 256 bytes as default
}

/*
	Use this to get the connection index from an XLive API call
*/
int CXnIp::getConnectionIndex(IN_ADDR connectionIdentifier)
{
	return connectionIdentifier.s_addr >> 24;
}

void CXnIp::setTimeConnectionInteractionHappened(IN_ADDR ina, int time)
{
	XnIp* xnIp = &ipManager.XnIPs[ipManager.getConnectionIndex(ina)];
	if (xnIp->bValid)
		xnIp->lastConnectionInteractionTime = time;
}

int CXnIp::handleRecvdPacket(XSocket* xsocket, sockaddr_in* lpFrom, WSABUF* lpBuffers, int bytesRecvdCount)
{
	XNetConnectionReqPacket* connectionPck = reinterpret_cast<XNetConnectionReqPacket*>(lpBuffers->buf);

	if (lpFrom->sin_addr.s_addr == H2Config_master_ip)
	{
		return ERROR_SUCCESS;
	}

	else if (bytesRecvdCount == sizeof(XNetConnectionReqPacket)
		&& connectionPck->ConnectPacketIdentifier == ipManager.connectPacketIdentifier)
	{
		// TODO: add more XNet request types (like disconnect, network pulse etc...)
		/*switch (xnetpacket->reqType) 
		{


		default:
			break;
		}*/

		LOG_TRACE_NETWORK("handleRecvdPacket() - Received secure packet with ip address {:x}, port: {}", htonl(lpFrom->sin_addr.s_addr), htons(lpFrom->sin_port));
		ipManager.HandleConnectionPacket(xsocket, connectionPck, lpFrom); // save NAT info and send back a connection packet

		SetLastError(WSAEWOULDBLOCK);
		return SOCKET_ERROR;
	}
	else
	{
		IN_ADDR ipIdentifier = ipManager.GetConnectionIdentifierByNat(lpFrom);

		/* Let the game know the packet received came from an unkown source */
		if (ipIdentifier.s_addr == 0)
		{
			WSASetLastError(WSAEWOULDBLOCK);
			return SOCKET_ERROR;
		}

		lpFrom->sin_addr = ipIdentifier;
		ipManager.setTimeConnectionInteractionHappened(ipIdentifier, timeGetTime());

		return ERROR_SUCCESS;
	}
}

void CXnIp::checkForLostConnections()
{
	for (int i = 0; i < GetMaxXnConnections(); i++)
	{
		XnIp* xnIp = &XnIPs[i];
		if (xnIp->bValid
			&& timeGetTime() - xnIp->lastConnectionInteractionTime > 30 * 1000)
		{
			ipManager.UnregisterXnIpIdentifier(xnIp->connectionIdentifier);
		}
	}
}

int CXnIp::sendConnectionRequest(XSocket* xsocket, IN_ADDR connectionIdentifier /* TODO: int reqType */)
{
	sockaddr_in sendToAddr;
	memset(&sendToAddr, 0, sizeof(sockaddr_in));
	
	XnIp* xnIp = &XnIPs[getConnectionIndex(connectionIdentifier)];

	if (xnIp->bValid
		&& xnIp->connectionIdentifier.s_addr == connectionIdentifier.s_addr)
	{
		sendToAddr.sin_family = AF_INET;
		sendToAddr.sin_addr = connectionIdentifier;

		XNetConnectionReqPacket connectionPacket;

		GetLocalXNAddr(&connectionPacket.xnaddr);
		getRegisteredKeys(&connectionPacket.xnkid, nullptr);
		connectionPacket.ConnectPacketIdentifier = /* reqType */ connectPacketIdentifier;

		xnIp->connectionPacketsSentCount++;

		int ret = XSocketSendTo((SOCKET)xsocket, (char*)&connectionPacket, sizeof(XNetConnectionReqPacket), 0, (sockaddr*)&sendToAddr, sizeof(sendToAddr));
		LOG_INFO_NETWORK("sendConnectionRequest() secure packet sent socket handle: {}, connection index: {}, connection identifier: {:x}", xsocket->winSockHandle, getConnectionIndex(connectionIdentifier), sendToAddr.sin_addr.s_addr);
		return ret;
	}
	else
	{
		LOG_ERROR_NETWORK("sendConnectionRequest() - connection index: {}, identifier: {:x} is invalid!", getConnectionIndex(connectionIdentifier), connectionIdentifier.s_addr);
		return -1;
	}
}

IN_ADDR CXnIp::GetConnectionIdentifierByNat(sockaddr_in* fromAddr)
{
	for (int i = 0; i < GetMaxXnConnections(); i++)
	{
		XnIp* xnIp = &XnIPs[i];
		if (sockAddrInEqual(fromAddr, &xnIp->NatAddrSocket1000) 
			|| sockAddrInEqual(fromAddr, &xnIp->NatAddrSocket1001))
		{
			return xnIp->connectionIdentifier;
		}
	}

	IN_ADDR addrInval;
	addrInval.s_addr = 0;
	return addrInval;
}

void CXnIp::SaveConnectionNatInfo(XSocket* xsocket, IN_ADDR connectionIdentifier, sockaddr_in* addr)
{
	/*
		In theory to handle multiple instance servers in the future what we can do is populate the port field of CreateUser,
		Then map the shit before we actually attempt a connection to the server here...

		That way we intercept it and don't even have to modify the game's engine.

		While we only have enough room for one port in the XNADDR struct we can just do, port+1.

		So,
		"connect" socket = 2001 if the XNADDR struct is 2000 which is the "join" socket.

		Then once TeamSpeak is in just do +6 because the game traditionally tries to map 1000-1006 so we'll just go 2007 = TS, etc.

		This should allow us to handle servers listening on any port without much effort or engine modification.
	*/

	LOG_INFO_NETWORK("SaveNatInfo() - socket: {}, connection index: {}, identifier: {:x}", xsocket->winSockHandle, getConnectionIndex(connectionIdentifier), connectionIdentifier.s_addr);
	int ipIndex = getConnectionIndex(connectionIdentifier);
	XnIp* xnIp = &this->XnIPs[ipIndex];


	if (xnIp->bValid 
		&& xnIp->connectionIdentifier.s_addr == connectionIdentifier.s_addr)
	{

		/* Store NAT data
		   First we look at our socket's intended port.
		   port 1000 is mapped to the receiving port pmap_a via the secure address.
		   port 1001 is mapped to the receiving port pmap_b via the secure address.
		*/
		// TODO: handle dynamically

		switch (xsocket->getNetworkSocketPort())
		{
		case 1000:
			//LOG_TRACE_NETWORK("XSocketRecvFrom() User.sockmap mapping port 1000 - port: %i, secure: %08X", htons(port), secure);
			xnIp->NatAddrSocket1000 = *addr;
			break;

		case 1001:
			//LOG_TRACE_NETWORK("XSocketRecvFrom() User.sockmap mapping port 1001 - port: %i, secure: %08X", htons(port), secure);
			xnIp->NatAddrSocket1001 = *addr;
			break;

		default:
			//LOG_ERROR_NETWORK("XSocketRecvFrom() User.sockmap[s] didn't match any ports!");
			break;

		}
	}
	else
	{
		LOG_ERROR_NETWORK("SaveNatInfo() - connection index: {} identifier: {:x} is invalid!", getConnectionIndex(connectionIdentifier), connectionIdentifier.s_addr);
	}
}

void CXnIp::HandleConnectionPacket(XSocket* xsocket, XNetConnectionReqPacket* connectReqPacket, sockaddr_in* addr)
{
	IN_ADDR connectionIdentifier;

	int ret = CreateXnIpIdentifier(&connectReqPacket->xnaddr, &connectReqPacket->xnkid, &connectionIdentifier, true);
	if (ret == ERROR_SUCCESS)
	{
		SaveConnectionNatInfo(xsocket, connectionIdentifier, addr);

		XnIp* xnIp = &ipManager.XnIPs[ipManager.getConnectionIndex(connectionIdentifier)];

		if (xnIp->bValid
			&& xnIp->xnetstatus < XNET_CONNECT_STATUS_CONNECTED)
		{
			ipManager.sendConnectionRequest(xsocket, xnIp->connectionIdentifier);
			ipManager.setTimeConnectionInteractionHappened(xnIp->connectionIdentifier, timeGetTime());

			// TODO: handle dynamically
			if (!sockAddrInIsNull(&xnIp->NatAddrSocket1000) 
				&& !sockAddrInIsNull(&xnIp->NatAddrSocket1001))
				xnIp->xnetstatus = XNET_CONNECT_STATUS_CONNECTED; // if we have the NAT data for each port, set the status to CONNECTED to prevent spamming xnet connection packets
			else
				xnIp->xnetstatus = XNET_CONNECT_STATUS_PENDING;

		}
	}
	else
	{
		LOG_TRACE_NETWORK("HandleConnectionPacket() - secure connection couldn't be established!");
	}
}

int CXnIp::CreateXnIpIdentifier(const XNADDR* pxna, const XNKID* xnkid, IN_ADDR* outIpIdentifier, bool handleFromConnectionPacket)
{
	/*
		This only happens for servers because we have all their data from the get go :)...

		- Update 1/20/2017 -
		This should now also be called when receiving the 'SecurityPacket' because we have the data on the first attempt to either establish a connection OR on the attempt to join a game,
		That should eliminate the need to talk to the Master server in order to get the XNADDR information from the secure address.
	*/

	int firstUnusedDataIndex = 0;
	bool firstUnusedDataIndexFound = false;

	XNADDR localXn;
	GetLocalXNAddr(&localXn);
	XNKID XnKid;
	getRegisteredKeys(&XnKid, nullptr);

	if (memcmp(xnkid, &XnKid, sizeof(XNKID)) != 0)
	{
		LOG_INFO_NETWORK("CreateXnIpIdentifier() - the specified XNKID is incorrect!");
		return WSAEINVAL;
	}

	// do not allow the connection if the received XNADDR is the same with the local one
	if (memcmp(&localXn.abEnet, pxna->abEnet, sizeof(((XNADDR*)0))->abEnet) == 0
		&& memcmp(&localXn.abOnline, pxna->abOnline, sizeof(((XNADDR*)0))->abOnline) == 0)
	{
		LOG_INFO_NETWORK("CreateXnIpIdentifier() - the specified XNADDR is the same with the local one, aborting connection.");
		return WSAEINVAL;
	}

	/*
		We check for connections that have not received any packets or XNetGetConnectStatus hasn't been called for some connections in the last 30 seconds, then proceed to create a new identifier
		to prevent people from failing to connect
	*/
	checkForLostConnections();

	// check if the user is already in the system
	for (int i = 0; i < GetMaxXnConnections(); i++)
	{
		if (XnIPs[i].bValid && memcmp(&XnIPs[i].xnaddr, pxna, sizeof(XNADDR)) == 0)
		{
			if (outIpIdentifier) {
				*outIpIdentifier = XnIPs[i].connectionIdentifier;
				LOG_INFO_NETWORK("CreateXnIpIdentifier() - already present connection index: {}, identifier: {:x}", i, XnIPs[i].connectionIdentifier.s_addr);
			}

			if (handleFromConnectionPacket) 
			{
				setTimeConnectionInteractionHappened(XnIPs[i].connectionIdentifier, timeGetTime());
			}

			return ERROR_SUCCESS;
		}

		if (XnIPs[i].bValid == false && firstUnusedDataIndexFound == false)
		{
			firstUnusedDataIndex = i; // save the first unused data index, in case the XnIp is not in the system, so we don't have to loop again to find a free spot
			firstUnusedDataIndexFound = true;
		}
	
		// if the ip is not in the system, add it
		if (i + 1 == GetMaxXnConnections())
		{	
			if (firstUnusedDataIndexFound)
			{
				std::mt19937 mt_rand(rd());
				std::uniform_int_distribution<int> dist(1, 255);

				XnIp* newXnIp = &XnIPs[firstUnusedDataIndex];
				memset(newXnIp, 0, sizeof(XnIp));

				newXnIp->xnkid = *xnkid;
				newXnIp->xnaddr = *pxna;

				int randIdentifier = dist(mt_rand);
				randIdentifier <<= 8;
				LOG_INFO_NETWORK("CreateXnIpIdentifier() - new connection index {}, identifier {:x}", firstUnusedDataIndex, htonl(firstUnusedDataIndex | randIdentifier));

				if (outIpIdentifier)
					outIpIdentifier->s_addr = htonl(firstUnusedDataIndex | randIdentifier);	

				newXnIp->xnetstatus = XNET_CONNECT_STATUS_IDLE;
				newXnIp->connectionIdentifier.s_addr = htonl(firstUnusedDataIndex | randIdentifier);
				newXnIp->bValid = true;

				setTimeConnectionInteractionHappened(newXnIp->connectionIdentifier, timeGetTime());

				if (handleFromConnectionPacket) 
				{

				}

				return ERROR_SUCCESS;
			}
			else 
			{
				LOG_TRACE_NETWORK("CreateXnIpIdentifier() - no more available connection spots!");
				return WSAENOMORE;
			}
		}
	}

	return WSAENOMORE;
}

void CXnIp::UnregisterXnIpIdentifier(const IN_ADDR ina)
{
	// TODO: let the other connection end know that the connection has to be closed

	int ipIndex = getConnectionIndex(ina);
	XnIp* xnIp = &this->XnIPs[ipIndex];
	if (xnIp->bValid
		&& xnIp->connectionIdentifier.s_addr == ina.s_addr)
	{
		LOG_INFO_NETWORK("UnregisterXnIpIdentifier() - Unregistered connection index: {}, identifier: {:x}", getConnectionIndex(ina), xnIp->connectionIdentifier.s_addr);
		memset(xnIp, 0, sizeof(XnIp));
	}
}

BOOL CXnIp::LocalUserLoggedIn() {
	return localUser.bValid;
}

void CXnIp::UnregisterLocalConnectionInfo()
{
	if (!localUser.bValid)
		return;

	SecureZeroMemory(&localUser, sizeof(XnIp));
}

void CXnIp::SetKeys(XNKID* xnkid, XNKEY* xnkey)
{
	if (xnkid)
		host_xnkid = *xnkid;

	if (xnkey)
		host_xnkey = *xnkey;
}

void CXnIp::EraseKeys()
{
	SecureZeroMemory(&this->host_xnkid, sizeof(XNKID));
	SecureZeroMemory(&this->host_xnkey, sizeof(XNKEY));
}

void CXnIp::getRegisteredKeys(XNKID* xnkid, XNKEY* xnkey)
{
	if (xnkid)
		*xnkid = host_xnkid;

	if (xnkey)
		*xnkey = host_xnkey;
}

void CXnIp::SetupLocalConnectionInfo(XNADDR* pxna) 
{
	SecureZeroMemory(&localUser, sizeof(XnIp));
	localUser.xnaddr = *pxna;
	localUser.bValid = true;
}

BOOL CXnIp::GetLocalXNAddr(XNADDR* pxna)
{
	SecureZeroMemory(pxna, sizeof(XNADDR));
	if (localUser.bValid)
	{
		*pxna = localUser.xnaddr;
		LOG_INFO_NETWORK("GetLocalXNAddr() - XNADDR: {:x}", pxna->ina.s_addr);
		return TRUE;
	}
	//LOG_INFO_NETWORK("GetLocalXNADDR(): Local user network information not populated yet.");

	return FALSE;
}

// #51: XNetStartup
int WINAPI XNetStartup(const XNetStartupParams *pxnsp)
{
	ipManager.GetStartupParamsAndUpdate(pxnsp);
	return 0;
}

// #53: XNetRandom
INT WINAPI XNetRandom(BYTE * pb, UINT cb)
{
	static bool Rc4CryptInitialized = false;

	LARGE_INTEGER key;

	if (Rc4CryptInitialized == false)
	{
		QueryPerformanceCounter(&key);
		XeCryptRc4Key(&Rc4StateRand, (BYTE*)&key, sizeof(key));
		Rc4CryptInitialized = true;
	}

	XeCryptRc4Ecb(&Rc4StateRand, pb, cb);
	return ERROR_SUCCESS;
}

// #54: XNetCreateKey
INT WINAPI XNetCreateKey(XNKID * pxnkid, XNKEY * pxnkey)
{
	LOG_INFO_NETWORK("XNetCreateKey()");
	if (pxnkid && pxnkey)
	{
		XNetRandom((BYTE*)pxnkid, sizeof(XNKID));
		XNetRandom((BYTE*)pxnkey, sizeof(XNKEY));
		pxnkid->ab[0] &= ~XNET_XNKID_MASK;
		pxnkid->ab[0] |= XNET_XNKID_SYSTEM_LINK_XPLAT;
	}

	return 0;
}

// #57: XNetXnAddrToInAddr
INT WINAPI XNetXnAddrToInAddr(const XNADDR *pxna, const XNKID *pxnkid, IN_ADDR *pina)
{
	LOG_INFO_NETWORK("XNetXnAddrToInAddr() - local-address: {:x}, online-address: {:x}", pxna->ina.s_addr, pxna->inaOnline.s_addr); // TODO

	if (pxna == nullptr 
		|| pxnkid == nullptr
		|| pina == nullptr)
		return WSAEINVAL;

	int ret = ipManager.CreateXnIpIdentifier(pxna, pxnkid, pina, false);
	 
	return ret;
}

// #60: XNetInAddrToXnAddr
INT WINAPI XNetInAddrToXnAddr(const IN_ADDR ina, XNADDR* pxna, XNKID* pxnkid)
{
	LOG_INFO_NETWORK("XNetInAddrToXnAddr() - connection index: {}, identifier {:x}", ipManager.getConnectionIndex(ina), ina.s_addr);
	
	if (pxna == nullptr
		|| pxnkid == nullptr)
		return WSAEINVAL;

	memset(pxna, 0, sizeof(XNADDR));
	memset(pxnkid, 0, sizeof(XNKID));

	XnIp* xnIp = &ipManager.XnIPs[ipManager.getConnectionIndex(ina)];
	
	if (xnIp->bValid 
		&& xnIp->connectionIdentifier.s_addr == ina.s_addr)
	{
		*pxna = xnIp->xnaddr;
		*pxnkid = xnIp->xnkid;

		return ERROR_SUCCESS;
	}

	LOG_ERROR_NETWORK("XNetInAddrToXnAddr() - connection index: {}, identifier: {:x} are invalid!", ipManager.getConnectionIndex(ina), ina.s_addr);

	return WSAEINVAL;
}

// #63: XNetUnregisterInAddr
int WINAPI XNetUnregisterInAddr(const IN_ADDR ina)
{
	LOG_INFO_NETWORK("XNetUnregisterInAddr() - connection index {}, identifier: {:x}", ipManager.getConnectionIndex(ina), ina.s_addr);
	ipManager.UnregisterXnIpIdentifier(ina);
	return ERROR_SUCCESS;
}

// #65: XNetConnect
int WINAPI XNetConnect(const IN_ADDR ina)
{
	LOG_INFO_NETWORK("XNetConnect() - connection index {}, identifier: {:x}", ipManager.getConnectionIndex(ina), ina.s_addr);

	XnIp* xnIp = &ipManager.XnIPs[ipManager.getConnectionIndex(ina)];
	if (xnIp->bValid
		&& xnIp->connectionIdentifier.s_addr == ina.s_addr)
	{
		if (xnIp->xnetstatus == XNET_CONNECT_STATUS_IDLE)
		{
			// TODO: handle dinamically, so it can be used by other games too
			extern XSocket* game_network_data_gateway_socket_1000; // used for game data
			extern XSocket* game_network_message_gateway_socket_1001; // used for messaging like connection requests
			ipManager.sendConnectionRequest(game_network_data_gateway_socket_1000, xnIp->connectionIdentifier);
			ipManager.sendConnectionRequest(game_network_message_gateway_socket_1001, xnIp->connectionIdentifier);
			
			xnIp->xnetstatus = XNET_CONNECT_STATUS_PENDING;
		}

		return ERROR_SUCCESS;
	}

	return WSAEINVAL;
}

// #66: XNetGetConnectStatus
int WINAPI XNetGetConnectStatus(const IN_ADDR ina)
{
	//LOG_INFO_NETWORK("XNetConnect(): connection index {}, identifier: {:x}", ipManager.getConnectionIndex(ina), ina.s_addr);
	XnIp* xnIp = &ipManager.XnIPs[ipManager.getConnectionIndex(ina)];
	if (xnIp->bValid
		&& xnIp->connectionIdentifier.s_addr == ina.s_addr)
	{
		/* 
			Mainly for H2v because it has P2P connection even on dedicated servers, if the connect status is checked by the game, it means the connection identifier is stil used
			This prevents connection info being cleared even if no data has been received from the connection (probably the ports were not forwarded/ no data is sent at all between the peers) 
		*/
		ipManager.setTimeConnectionInteractionHappened(ina, timeGetTime()); 
		return xnIp->xnetstatus;
	}
	return WSAEINVAL;
}

// #73: XNetGetTitleXnAddr
DWORD WINAPI XNetGetTitleXnAddr(XNADDR * pAddr)
{
	if (pAddr)
	{
		ipManager.GetLocalXNAddr(pAddr);
	}
	return XNET_GET_XNADDR_STATIC | XNET_GET_XNADDR_ETHERNET;
}


// #55: XNetRegisterKey //need #51
int WINAPI XNetRegisterKey(XNKID *pxnkid, XNKEY *pxnkey)
{
	LOG_INFO_NETWORK("XNetRegisterKey()");
	ipManager.SetKeys(pxnkid, pxnkey);
	return ERROR_SUCCESS;
}


// #56: XNetUnregisterKey // need #51
int WINAPI XNetUnregisterKey(const XNKID* pxnkid)
{
	LOG_INFO_NETWORK("XNetUnregisterKey()");
	ipManager.EraseKeys();
	return ERROR_SUCCESS;
}