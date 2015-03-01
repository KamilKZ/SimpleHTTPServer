#include <iostream>
#include <regex>

#include <HTTPServer/HTTPServer.hpp>

namespace HTTP
{

Server::Server(Map<String, String> mimeTypesMap) : 
				mMimeTypes(mimeTypesMap),
				Thread(),
				mExitCode(0)
{
	fileHandler = std::make_shared<FileHandler>();
	AddHandler<DefaultPageHandler>(String("^\\/$"));
};

Server::~Server()
{
	mRunning = false;

	mListener.close();

	for (auto c : mConnections)
	{
		c->Stop(mExitCode != 0);
	}

	Thread::Stop(true);
}

bool Server::Start(UInt port)
{
	mPort = port;
	mListener.listen(port);

	return Thread::Start();
}

void* Server::Run()
{
	mRunning = true;
	while (mRunning)
	{
		Connection* connection = new Connection(this);

		connection->mStatus = mListener.accept(connection->mSocket);
		if (connection->mStatus == sf::Socket::Status::Done) //this should be blocking :?
		{
			connection->Start();
			std::shared_ptr<Connection> strongPtrConnection(connection);
			mConnections.push_back(strongPtrConnection);
		}
		else
		{
			delete connection; // @46, making this redundant...
		}
	}
	return 0;
}

// Set regex and mimetype for files ( filenames matching regex will use that mimetype )
void Server::SetMimeType(const String& regex, const String& mimeType)
{
	mMimeTypes[regex] = mimeType;
}

// Get mimetype from map by regex
String Server::GetMimeType(const String& regex) const
{
	Map<String, String>::const_iterator it = mMimeTypes.find(regex);
	if (it == mMimeTypes.end())
	{
		return "";
	}
	else
	{
		return it->second;
	}
}

// Find out what mimetype this filename belongs to
String Server::FindMimeType(const String& filename) const
{
	for(Map<String, String>::const_iterator it = mMimeTypes.begin(); it != mMimeTypes.end(); ++it)
	{
		//check filename on regex
		if (std::regex_match(filename, std::regex(it->first)))
		{
			return it->second; //return mimetype that fit the regex
		}
	}
	return "";
}

Response Server::HandleRequest(Request& request) // Must be threadsafe!
{
	Response response; //passed around by reference

	Map<String, std::shared_ptr<IHandler>>::iterator it = mHandlerMap.begin();
	for (it; it != mHandlerMap.end(); ++it)
	{
		//check path on regex
		if (std::regex_match(request.GetPath().filePath, std::regex(it->first)))
		{
			if (it->second->operator()(request, response, *this)) //handler function fits regex
				return response; //handler processed successfully,
			//or returned false to continue
		}
	}

	if (it == mHandlerMap.end()) //nothing found, use default
	{
		fileHandler->operator()(request, response, *this);
	}

	return response;
}

void Server::DestroyConnection(Connection* connection)
{
	printf("!!! Requested DestroyConnection(%d) !!!", connection->GetThreadID());

	ArrayList<std::shared_ptr<Connection>>::iterator it = mConnections.begin();
	while (it != mConnections.end())
	{
		std::shared_ptr<Connection> c = ( *it );

		if (c->GetThreadID() == connection->GetThreadID())
		{
			mConnections.erase(it);
		}

		it++;
	}
}

};