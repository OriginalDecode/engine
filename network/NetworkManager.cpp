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


#include <thread>
#include <EntitySystem/BaseComponent.h>
#include <engine/Synchronizer.h>
#include <engine/Engine.h>
#include <Postmaster/EventManager.h>

#include "ConnectMessage.h"
#include "NetEntityData.h"
#include "NetCreateEntity.h"

namespace network
{
	NetworkManager::NetworkManager()
	{
		network::CreateGUID(&m_GUID);
		
		m_Messages[0].Init(2500);
		m_Messages[1].Init(2500);

		//constexpr int buffer_size = sizeof(Buffer);
		//constexpr int buffer_size = ((sizeof(Buffer) * 2500) / 1024) / 1024;
		//constexpr int msgsize = sizeof(m_Messages[0]);


		m_Recieve = new std::thread([&]() {
			Synchronizer* pSync = Engine::GetInstance()->GetSynchronizer();
			Buffer to_fill;
			while (!pSync->HasQuit())
			{
				Receive(to_fill);
				if (to_fill.m_Length > 0)
					m_Messages[m_CurrentBuffer ^ 1].Add(to_fill);
			}
		});
	}

	void NetworkManager::CleanUp()
	{
		m_Recieve->join();
		delete m_Recieve;
#ifdef _WIN32
		closesocket(m_Socket);
		WSACleanup();
#else
		close(m_Socket);
#endif
	}

	void NetworkManager::HandleConnectionRequest(Buffer& buffer)
	{
		if (!m_IsHost)
			return;

		printf("Connection Requested!\n");
		if (m_Connections.size() < 8)
		{
			ConnectMessage data;
			data.UnpackMessage(buffer.m_Buffer, buffer.m_Length);


			Connection new_connection;
			new_connection.m_Connection = buffer.m_Sender;
			new_connection.m_GUID = data.m_GUID;


			AddConnection(new_connection);
			Send(ConnectMessage(CONNECTION_ACCEPTED, "Welcome to the server!"), new_connection.m_Connection);
			printf("Connection Accepted\n");
		}
		else
		{
			Send(ConnectMessage(CONNECTION_REJECTED, "Server full"), buffer.m_Sender);
			printf("Connection Rejected\n");
		}
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
		DWORD non_blocking = 1;
		assert(ioctlsocket(m_Socket, FIONBIO, &non_blocking) == 0 && "failed to makke nonblocking");
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

		//bool connected = false;
		//bool has_respons = false;
		printf("\nSending connection request to server...");
		Send(ConnectMessage(REQUEST_CONNECTION, m_GUID), connection.m_Connection);
		m_Connections.push_back(connection);
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

	void NetworkManager::Receive(Buffer& buffer_out) /* This takes a nasty amount of memory */
	{
		const s32 max_len = 512;
		socklen_t size = sizeof(sockaddr_in);
		memset(&buffer_out.m_Buffer, 0, sizeof(s8) * max_len);

		int length_of_packet = recvfrom(m_Socket, buffer_out.m_Buffer, max_len, 0, (sockaddr*)&buffer_out.m_Sender, &size);
		buffer_out.m_Length = length_of_packet;

		//if (length_of_packet > 0)
			//memcpy(&buffer_out.m_Buffer, &buffer[0], length_of_packet * sizeof(s8));

	}

	s32 NetworkManager::ReadType(s8 type_char)
	{
		return (s32)type_char;
	}

	s32 NetworkManager::ReadType(const Buffer& buffer)
	{
		return (s32)buffer.m_Buffer[0];
	}

	void NetworkManager::Update()
	{
		for (Buffer message : m_Messages[m_CurrentBuffer])
		{
			const s32 type = ReadType(message);
			switch (type)
			{
				case eNetMessageType::REQUEST_CONNECTION:
				{
					HandleConnectionRequest(message);
				} break;


				case eNetMessageType::NET_ENTITY_DATA:
				{
					NetEntityData data;
					data.UnpackMessage(message.m_Buffer, message.m_Length);

					for (NetReplicate* replicants : m_Replicants[data.m_GUID])
					{
						CU::Vector3f pos(data.x, data.y, data.z);
						replicants->OnNotify((eNetMessageType)type, (void*)&pos);
					}

				} break;

				case eNetMessageType::NET_CREATE_ENTITY:
				{
					NetCreateEntity data;
					data.UnpackMessage(message.m_Buffer, message.m_Length);
					
					EventManager::GetInstance()->SendMessage("create_entity", &data.m_GUID);
				} break;

				case eNetMessageType::CONNECTION_ACCEPTED:
				{
				} break;

				case eNetMessageType::CONNECTION_REJECTED:
				{
					m_Connections.pop_back();
				} break;


			}
		}
		m_Messages[m_CurrentBuffer].RemoveAll();
		m_CurrentBuffer ^= 1;
		m_IsDone = true;
	}

	void NetworkManager::AddReplicant(GUID guid, NetReplicate* to_replicate)
	{
		auto it = m_Replicants.find(guid);
		if (it != m_Replicants.end())
		{
			it->second.push_back(to_replicate);
			return;
		}

		m_Replicants.emplace(guid, std::vector<NetReplicate*>());
		m_Replicants.at(guid).push_back(to_replicate);

	}

	void NetworkManager::AddConnection(Connection& client)
	{
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