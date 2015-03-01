#include <HTTPServer/Connection.hpp>
#include <HTTPServer/Request.hpp>
#include <HTTPServer/Response.hpp>
#include <HTTPServer/Server.hpp>

#include <iostream>
#include <regex>

// http://www.jmarshall.com/easy/http/#http1.1s5

namespace HTTP
{

Connection::Connection(Server* server) 
	: mpServer(server), Thread()
{
	mSocket.setBlocking(true); // why does this matter?
}
			
Connection::~Connection()
{
	mSocket.disconnect();
	mpServer->DestroyConnection(this);

	Thread::Stop(true);
}

void* Connection::Run(void)
{	
	//mThread.detach();
	mRunning = true;
	while(mRunning){ // The life of the connection... Recieving requests all day
		Request request = Receive(); // should probably be blocking
		
		switch(mStatus)
		{
			case sf::Socket::Status::Done: // Got a request, handle request
			{
				Response response = mpServer->HandleRequest(request); //static? no, pointer, why not?
				Send(response);
				break;
			}
			case sf::Socket::Status::Disconnected: // Connection closed:OK.
			case sf::Socket::Status::Error: // Or Error'd
			{
				Stop(false); // Clean up thread
				return 0;
			}
			default: // Not Ready, or unhandled
			{
				break;
			}
		}
	}
}

sf::Socket::Status Connection::GetStatus(void)
{
    std::lock_guard<std::mutex> guard(mMutex);
	return mStatus;
}

bool Connection::IsOpen(void)
{
    std::lock_guard<std::mutex> guard(mMutex);
	return ( mStatus == sf::Socket::Status::Done ) || ( mStatus == sf::Socket::Status::NotReady );
}

Request Connection::Receive(void)
{
	std::lock_guard<std::mutex> guard(mMutex);

	Byte* buffer = new Byte[1024];
	size_t buffer_size = 1024;
	size_t packet_size = 0;

	mStatus = mSocket.receive(reinterpret_cast<void*>(buffer), buffer_size, packet_size);

	if (mStatus == sf::Socket::Status::Done){
	
		char* data_char = (char*)( buffer );
		String data(data_char, packet_size);

		if (!data.empty())
		{
			std::cout << data << std::endl; //debug print packet
			return Request::Parse( data ); //parse data into Request
		}
	}
	
	return Request();
}

bool Connection::Send(Response& response)
{
	String responseStr = Response::Prepare(response);
	if (!responseStr.empty())
	{
		printf("\n\n\tResponse\n\n%s\n\n", responseStr.c_str());
		mSocket.send(responseStr.c_str(), responseStr.size());
		return true;
	}
	else
	{
		return false;
	}
}
};