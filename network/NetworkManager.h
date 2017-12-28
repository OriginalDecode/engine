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

#include <map>
#include <vector>

#include "standard_datatypes.h"
#include <DL_Debug/DL_Debug.h>
#include <functional>
#include <CommonLib/DataStructures/GrowingArray.h>
#ifdef _WIN32
typedef s32 Socket_Type;
#else
typedef __socket_type Socket_Type;
#endif
typedef s32 Socket;
typedef s32 Socket_Protocol;
typedef s32 IP_Version;
namespace std
{
	class thread;
};

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
	GUID m_GUID;
};
struct NetReplicate;
namespace network
{
	void CreateGUID(GUID* pGUID);
	class NetworkManager
	{
	public:
		NetworkManager();
		bool HasStarted() { return m_HasStarted; }
		void CleanUp();

		s32 Connect(const char* ip, s16 port);
		s32 Host(s16 port);

		template<typename T>
		void Send(T message, sockaddr_in target);

		template<typename T>
		void Send(T message, bool exclude_sender = true);

		void Receive(Buffer& buffer_out);

		bool IsHost() const { return m_IsHost; }

		Socket GetSocket() const { return m_Socket; }
		Socket_Type GetSocketType() const { return m_SocketType; }
		Socket_Protocol GetSocketProtocol() const { return m_SocketProtocol; }
		IP_Version GetIPVersion() const { return m_IPVersion; }
		void AddConnection(Connection& client);
		s32 Connections() const { return (s32)m_Connections.size(); }
		static s32 ReadType(s8 type_char);
		static s32 ReadType(const Buffer& buffer);
		const GUID& GetGUID() const { return m_GUID; }

		void Update();

		void AddReplicant(GUID guid, NetReplicate* to_replicate);

	private:
		void HandleConnectionRequest(Buffer& buffer);

		u8 m_CurrentBuffer = 0;
		volatile bool m_IsDone = false;
		std::thread* m_Recieve = nullptr;

		CU::GrowingArray<Buffer> m_Messages[2];
		
		struct Replica
		{
			GUID guid;
			std::vector<NetReplicate*> m_ToReplicate;
		};

		std::vector<Replica> m_Replicants;



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
		std::vector<Connection> m_Connections;
	};

#define NO_FLAGS 0

	template<typename T>
	void NetworkManager::Send(T message, sockaddr_in target)
	{
		const size_t size = sizeof(T) + message.m_Stream.size();
		const char* name = typeid(T).name();
		LOG_NETWORK("Sending %s message. message size : %llu", name, size);

		message.PackMessage();
		s32 message_length = (s32)message.m_Stream.size();
		assert(message_length > 0 && "Message Length was 0");
		sendto(m_Socket, &message.m_Stream[0], message_length, NO_FLAGS, (sockaddr*)&target, sizeof(target));
	}

	template<typename T>
	void NetworkManager::Send(T message, bool exclude_sender)
	{

		const size_t size = sizeof(T) + message.m_Stream.size();
		const char* name = typeid(T).name();
		LOG_NETWORK("Sending %s message. message size : %llu", name, size);


		message.PackMessage();
		s32 message_length = (s32)message.m_Stream.size();
		assert(message_length > 0 && "Message Length was 0");

		for (Connection& connection : m_Connections)
		{
			if (exclude_sender && message.m_GUID == connection.m_GUID)
				continue;

			sendto(m_Socket, &message.m_Stream[0], message_length, NO_FLAGS, (sockaddr*)&connection.m_Connection, sizeof(connection.m_Connection));
		}
	}
};
