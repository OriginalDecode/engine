#include "NetworkManager.h"

#include <sstream>
#include <string.h>

#ifdef _WIN32
#include <Objbase.h>
#else
#include <fcntl.h>
#include <unistd.h>
#define INVALID_SOCKET ~0
#endif

#include "ConnectMessage.h"


namespace network
{
	NetworkManager::NetworkManager()
	{
		network::CreateGUID(&m_GUID);
	}

	void NetworkManager::CleanUp()
	{
#ifdef _WIN32
		closesocket(m_Socket);
		WSACleanup();
#else
		close(m_Socket);
#endif
	}

	s32 NetworkManager::InitiateWSAData()
	{
#ifdef _WIN32
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
			return -2;
#endif
		return 0;
	}

	s32 NetworkManager::Connect(const char* ip, s16 port)
	{
		if (!m_IsHost && m_Connections.size() > 0)
			assert(false && "Unexpected behaviour. Tried to establish two connections as client!");

		s32 result = InitiateWSAData();
		if (result != 0)
			return -2;

		m_IPVersion = AF_INET;
		m_SocketType = SOCK_DGRAM;
		m_SocketProtocol = IPPROTO_UDP;
		m_Socket = (Socket)socket(m_IPVersion, m_SocketType, m_SocketProtocol);
#ifdef _WIN32
		//Setup nonblocking socket for windows.
#else
		fcntl(m_Socket, F_SETFL, O_NONBLOCK);
#endif

		if (m_Socket == INVALID_SOCKET)
			return m_Socket;


		Connection connection;
		memset(&connection.m_Connection, 0, sizeof(sockaddr_in));

		connection.m_Connection.sin_family = m_IPVersion;
		connection.m_Connection.sin_port = htons(port);
		connection.m_Connection.sin_addr.s_addr = inet_addr(ip);

		bool connected = false;
		bool has_respons = false;
		printf("\nSending connection requset to server...");
		Send(ConnectMessage(REQUEST_CONNECTION), connection.m_Connection);

		while (has_respons == false)
		{
			Buffer on_connect = Receive();
			if (on_connect.m_Length > 0)
			{
				ConnectMessage net_message;
				net_message.UnpackMessage(on_connect.m_Buffer, on_connect.m_Length);
				printf("\n%s", net_message.m_ConnectMessage.c_str());
				if (net_message.IsType(CONNECTION_ACCEPTED))
				{

					connected = true;
				}
				else if (net_message.IsType(CONNECTION_REJECTED))
				{
					connected = false;
				}
				has_respons = true;
			}
		}

		if (connected)
		{
			printf("\nConnection established to server IP : %s", ip);
			m_Connections.push_back(connection);
		}
		else
		{
			printf("\nFailed to establish connection to server IP : %s", ip);
			return -3;
		}

		return 0;
	}

	s32 NetworkManager::Host(s16 port)
	{
		m_IsHost = true;
		std::stringstream using_port;
		using_port << port;

		s32 result = InitiateWSAData();
		if (result == ~0)
			return -2;

		addrinfo* addr_res = nullptr;
		addrinfo hints;

		memset(&hints, 0, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		hints.ai_flags = AI_PASSIVE;

		result = getaddrinfo(NULL, using_port.str().c_str(), &hints, &addr_res);
		if (result != 0)
			return result;

		m_Socket = (Socket)socket(
			addr_res->ai_family
			, addr_res->ai_socktype
			, addr_res->ai_protocol);

		if (m_Socket == INVALID_SOCKET)
			return -1;

		result = bind(m_Socket, addr_res->ai_addr, (s32)addr_res->ai_addrlen);
		if (result == INVALID_SOCKET)
			return -1;

		return 0;
	}

	Buffer NetworkManager::Receive()
	{
		Buffer to_return;
		socklen_t size = sizeof(sockaddr_in);
		s8 buffer[512];
		memset(&buffer, 0, sizeof(s8) * 512);

		int length_of_packet = recvfrom(m_Socket, buffer, 512, 0, (sockaddr*)&to_return.m_Sender, &size);
		to_return.m_Length = length_of_packet;

		if (length_of_packet > 0)
			memcpy(&to_return.m_Buffer, &buffer[0], length_of_packet * sizeof(s8));

		return to_return;
	}

	s32 NetworkManager::ReadType(s8 type_char)
	{
		return (s32)type_char;
	}

	s32 NetworkManager::ReadType(const Buffer& buffer)
	{
		return (s32)buffer.m_Buffer[0];
	}

	void NetworkManager::RegisterCallback(GUID guid, callback* _callback)
	{
		auto it = m_Callbacks.find(guid);
		if (it != m_Callbacks.end())
		{
			it->second.push_back(_callback);
		}
	}

	void NetworkManager::AddConnection(Connection& client)
	{

		//client.m_GID = ++m_GID; //server will always have an ID of 0 and will not need to be assigned one ahead of time?
		m_Connections.push_back(client);
	}

	void CreateGUID(GUID* pGUID)
	{
#ifdef _WIN32
		HRESULT hr = CoCreateGuid(pGUID);
		assert(hr == S_OK && "Failed to create GUID!");
#endif
	}
};