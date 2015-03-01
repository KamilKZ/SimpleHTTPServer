#pragma once

#include "Common.hpp"
#include "IHandler.hpp"

namespace HTTP
{
	API_IMPORT extern const Map<String, String> _defaultMimeTypes;

	class DefaultPageHandler : virtual public IHandler
	{
	public:
		virtual bool operator()(const Request& request, Response& response, const Server& server) const override;
	};

	class FileHandler : virtual public IHandler
	{
	public:
		virtual bool operator()(const Request& request, Response& response, const Server& server) const override;
	};
}