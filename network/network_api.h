#pragma once
//#include "NetIncludes.h"

#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_
#endif


#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#endif

#include "NetMessage.h"
#include "ChatMessage.h"

#include <vector>

#include "standard_datatypes.h"

#ifdef _WIN32
typedef s32 Socket_Type;
#else
typedef __socket_type Socket_Type;
#endif
typedef s32 Socket;
typedef s32 Socket_Protocol;
typedef s32 IP_Version;

struct Buffer
{
	//Max packet size is 512 bytes.
	s8 m_Buffer[512];
	s32 m_Length;
	sockaddr_in m_Sender;
};

struct Connection
{
	sockaddr_in m_Connection;
	u32 m_GID;
};

class NetworkHandle
{
public:
	NetworkHandle() = default;
	bool HasStarted() { return m_HasStarted; }	
	void CleanUp();

	s32 Connect(const char* ip, s16 port);
	s32 Host(s16 port);
	
	template<typename T>
	void Send(T message, sockaddr_in target);

	template<typename T>
	void Send(T message, bool exclude_sender = true); 

	Buffer Receive();
	
	bool IsHost() const { return m_IsHost; }

	Socket GetSocket() const { return m_Socket; }
	Socket_Type GetSocketType() const { return m_SocketType; }
	Socket_Protocol GetSocketProtocol() const { return m_SocketProtocol; }
	IP_Version GetIPVersion() const { return m_IPVersion; }
	void AddConnection(Connection& client);
	s32 Connections() const { return (s32)m_Connections.size(); }
	static s32 ReadType(s8 type_char);
	static s32 ReadType(const Buffer& buffer);
private:
	s32 InitiateWSAData();
	bool m_IsHost = false;
	bool m_HasStarted = false;
	Socket m_Socket;
	Socket_Type m_SocketType;
	Socket_Protocol m_SocketProtocol;
	IP_Version m_IPVersion;
#ifdef _WIN32 
	GUID m_GUID;
#endif
	u32 m_GID = 0;
	std::vector<Connection> m_Connections;
	// sockaddr_in m_Connection; //List of these as host
};

#define NO_FLAGS 0
template<typename T>
void NetworkHandle::Send(T message, sockaddr_in target)
{
	message.PackMessage();
	s32 message_length = (s32)message.m_Stream.size();
	assert(message_length > 0 && "Message Length was 0");
	sendto(m_Socket, &message.m_Stream[0], message_length, NO_FLAGS, (sockaddr*)&target, sizeof(target));
}

template<typename T>
void NetworkHandle::Send(T message, bool exclude_sender)
{
	if(!m_IsHost)
		message.m_GID = m_GID;

	message.PackMessage();
	s32 message_length = (s32)message.m_Stream.size();
	assert(message_length > 0 && "Message Length was 0");
	for(s32 i = 0; i < m_Connections.size(); i++)
	{
		if(exclude_sender)
		{
			//the message does not contain the same sin_family as the actual connection.
			if(message.m_GID == m_Connections[i].m_GID)
				continue;
		}
		sendto(m_Socket, &message.m_Stream[0], message_length, NO_FLAGS, (sockaddr*)&m_Connections[i].m_Connection, sizeof(m_Connections[i].m_Connection));
	}
}
