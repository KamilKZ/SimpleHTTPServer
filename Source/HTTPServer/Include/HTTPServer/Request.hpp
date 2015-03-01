#pragma once

#include <sstream>

#include "Common.hpp"
#include "FilePath.hpp"
#include "URI.hpp"

//http://sfml-dev.org/documentation/2.1/classsf_1_1Http_1_1Request.php

// Parameters :
// MIME-Type: application/x-www-form-urlencoded
// Params: ?parameter=value&also=another
// Parse those.

namespace HTTP
{
	class Request;

	class API_EXPORT Request
	{
	public:
		enum class Method { GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE };

		static Request Parse(const String& data);
		static String Prepare(const Request& r);

		Request() {};

		void SetBody(String body) { mBody = body; };
		//void SetPath(FilePath path) { mPath = path; };
		void SetMethod(Method method) { mMethod = method; };
		void SetURI(URI uri) { mURI = uri; };

		String GetBody(void) const { return mBody; };
		FilePath GetPath(void) const { return mURI.path; };
		Method GetMethod(void) const { return mMethod; };
		URI GetURI(void) const { return mURI; };

		void SetHeader(String header, String value) { mHeaders[header] = value; };
		void SetParam(String param, String value) { mParams[param] = value; };

		String GetHeader(String header) const;
		String GetParam(String param) const;

	protected:
		friend class Server;
		friend class Connection;

		void ParseHeaders(std::istream& data);

		String mBody;
		URI mURI;
		Method mMethod;
		//FilePath mPath; // TODO REMOVE THIS, USE mURI.path instead
		//Protocol mProtocol; //HTTP/HTTPS + VERSION: 1.0/1.1
		Map<String, String> mParams;
		Map<String, String> mHeaders;
	};
};