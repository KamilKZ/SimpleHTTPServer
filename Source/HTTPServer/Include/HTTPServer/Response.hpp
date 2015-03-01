#pragma once

#include "Common.hpp"

String API_EXPORT toString(UInt i);
String API_EXPORT toLower(String str);

namespace HTTP
{
	namespace StatusCode
	{
		struct StatusCode;
	};

	class API_EXPORT Response
	{
	public:
		static Response Parse(const String& data);
		static String Prepare(const Response& r);

		Response() {};
		
		static void SetStatus(Response& response, const StatusCode::StatusCode status);

		void SetBody(String body) { mBody = body; SetHeader("Content-Length", toString(body.size())); };
		void SetStatusCode(UInt statusCode) { mStatusCode = statusCode; };
		void SetStatusPhrase(String statusPhrase) { mStatusPhrase = statusPhrase; };
		void SetHeader(String header, String value) { mHeaders[toLower(header)] = value; };

		String GetBody(void) const { return mBody; };
		UInt GetStatusCode(void) const { return mStatusCode; };
		String GetStatusPhrase(void) const { return mStatusPhrase; };
		String GetHeader(String header) const;

	protected:
		friend class Server;
		friend class Connection;

		String mBody;
		UInt mStatusCode;
		String mStatusPhrase;
		//Protocol mProtocol; // see Request.hpp
		Map<String, String> mHeaders;
	};
};