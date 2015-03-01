#include <regex>

#include <HTTPServer/HTTPServer.hpp>

namespace HTTP
{
	const Map<String, String> _defaultMimeTypes = { // TODO, refactor this into a config file where the extensions arent a regex but are made into one
			{ ".+\\.html$", "text/html" }, //.+ match 1+ of any char, \. escape '.', html char sequence 
			{ ".+\\.css$", "text/css" }, //$ means end, so it ends on .css
			{ ".+\\.ico$", "image/x-icon" },
			{ ".+\\.js$", "application/javascript" },
			{ ".+\\.json$", "application/json" },
			{ ".+\\.txt$", "text/plain" },
			{ ".+\\.xml$", "text/xml" }
	};

	bool DefaultPageHandler::operator()( const Request& request, Response& response, const Server& server ) const
	{
		/*FileSystem::File file;

		if (server.fileSystem.exists("index.html")){
		file = server.fileSystem.load("index.html");
		}else if (server.fileSystem.exists("default.html")){
		file = server.fileSystem.load("default.html");
		}else if (server.fileSystem.exists("home.html")){
		file = server.fileSystem.load("home.html");
		}else
		{
		printf("ERROR: Could not find default page\n");
		return false;
		}

		Response::SetStatus(response, StatusCode::OK);
		response.SetHeader("Content-Type", "text/html");
		response.SetBody(file.GetContents());
		return true;*/

		FileResource file;
		if (file.Open("index.html") || //1st priority
			file.Open("default.html") || //2nd
			file.Open("home.html")) //nth
		{
			Response::SetStatus(response, StatusCode::OK);
			response.SetHeader("Content-Type", "text/html");
			response.SetBody(file.Get());
			return true;
		}
		else
		{
			printf("ERROR: Could not find default page\n");
			return false;
		}
	}

	bool FileHandler::operator()(const Request& request, Response& response, const Server& server) const
	{
		/*if (server.fileSystem.exists(request.GetPath()))
		{
		FileSystem::File file = server.fileSystem.Open(request.GetPath());

		Response::SetStatus(response, StatusCode::OK);
		response.SetHeader("Content-Type", FindMimeType(request.GetPath().filePath)); // todo do something with this anything mimetypes in server
		response.SetBody(file.Get());
		printf("200 OK. Sending file '%s'\n", request.GetPath().filePath.c_str());
		return true;
		}
		else
		{
		Response::SetStatus(response, StatusCode::NotFound);
		printf("ERROR: 404 Not Found. Could not load file '%s'\n", request.GetPath().filePath.c_str());
		return false;
		}*/

		if (std::regex_match(request.GetPath().filePath, std::regex(".*\\.\\..*"))) //updir?! regex: *..* (where * is 0 or more chars)
		{
			Response::SetStatus(response, StatusCode::Forbidden);
			printf("ERROR: 403 Forbidden. Request used updir\n");
		}
		else //path is fine
		{
			if (request.GetMethod() == Request::Method::GET)
			{
				FileResource file;
				if (file.Open(request.GetPath().filePath))
				{
					Response::SetStatus(response, StatusCode::OK);

					String mimeType = server.FindMimeType(request.GetPath().filePath);
					if (!mimeType.empty())
						response.SetHeader("Content-Type", mimeType);

					response.SetBody(file.Get());
					printf("200 OK. Sending file '%s'\n", request.GetPath().filePath.c_str());
					return true;
				}
				else
				{
					Response::SetStatus(response, StatusCode::NotFound);
					printf("ERROR: 404 Not Found. Could not load file '%s'\n", request.GetPath().filePath.c_str());
					return false;
				}
			}
			else
			{
				Response::SetStatus(response, StatusCode::NotImplemented);
				printf("ERROR: 501 Not Implemented. Method unsupported\n");
			}
		}
	}
}