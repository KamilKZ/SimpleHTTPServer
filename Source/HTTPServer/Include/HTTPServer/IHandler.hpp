#pragma once

namespace HTTP
{
	class Request;
	class Response;
	class Server;

	class API_EXPORT IHandler
	{
	public:
		virtual bool operator()(const Request& request, Response& response, const Server& server) const = 0;
	};
};