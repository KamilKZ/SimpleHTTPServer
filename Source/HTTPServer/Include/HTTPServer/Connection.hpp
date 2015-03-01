#pragma once

// TCP SOCKET 9398
// http://sfml-dev.org/tutorials/2.1/network-socket.php
// http://sfml-dev.org/tutorials/2.1/network-packet.php
// http://sfml-dev.org/documentation/2.1/classsf_1_1TcpListener.php
// http://sfml-dev.org/documentation/2.1/classsf_1_1TcpSocket.php

// RFC2616 http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html

// MIME types: http://en.wikipedia.org/wiki/MIME_type#List_of_common_media_types
//	application/json
//	text/html

// HTTP Headers: http://en.wikipedia.org/wiki/List_of_HTTP_header_fields

// Buffer things in Server.mBuffer
// When requested, send and clear the buffer
// The client is updated, and the server starts to fill the buffer again
// until the client requests it once more
// Call it the 'Heartbeat'

#include <functional>
#include <mutex>
#include <SFML/Network.hpp>
#include "Common.hpp"
#include "Thread.hpp"

namespace HTTP
{
	class Server;
	class Request;
	class Response;

	class API_EXPORT Connection : public Thread
	{
	public:
		Connection(Server* server);
		virtual ~Connection();
		
		void* Run(void) override; // From Thread
		
		sf::Socket::Status GetStatus(void); // Thread safe
		bool IsOpen(void);

	protected:
		Request Receive(void);
		bool Send(Response& response);
		
	private:
		friend class Server;

		sf::TcpSocket mSocket;
		std::atomic<sf::Socket::Status> mStatus;
		
		Server* mpServer; // WHAT THE FUCK IS THIS DOING HERE, CONNECTION SHOULD NOT KNOW ABOUT SERVER... MMHHHH WHAT DO?
		
		//Thread safety
		std::mutex mMutex;
	};
};