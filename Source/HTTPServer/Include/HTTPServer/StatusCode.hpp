#pragma once

#include <sstream>

#include "Common.hpp"

namespace HTTP
{
namespace StatusCode
{
	struct API_EXPORT StatusCode
	{
	public:
		StatusCode(UInt errorCode, String errorPhrase, String errorMessage) : 
				  mCode(errorCode), mPhrase(errorPhrase), mMessage(errorMessage) {};
				  
		void SetCode(UInt code) { mCode = code; }
		void SetPhrase(String phrase) { mPhrase = phrase; }
		void SetMessage(String message) { mMessage = message; }
		
		UInt GetCode(void) { return mCode; }
		String GetPhrase(void) { return mPhrase; }
		String GetMessage(void) { return mMessage; }
		
	private:
		UInt mCode;
		String mPhrase;
		String mMessage;
	};

	// 1xx: informational
	API_IMPORT extern const StatusCode Continue;
	API_IMPORT extern const StatusCode SwitchingProtocols;
													
	// 2xx: success									
	API_IMPORT extern const StatusCode OK;
	API_IMPORT extern const StatusCode Created;
	API_IMPORT extern const StatusCode Accepted;
	API_IMPORT extern const StatusCode NoContent;
	API_IMPORT extern const StatusCode ResetContent;
	API_IMPORT extern const StatusCode PartialContent;
													
	// 3xx: redirection								
	API_IMPORT extern const StatusCode MultipleChoices;
	API_IMPORT extern const StatusCode MovedPermanently;
	API_IMPORT extern const StatusCode MovedTemporarily;
	API_IMPORT extern const StatusCode NotModified;
													
	// 4xx: client error							
	API_IMPORT extern const StatusCode BadRequest;
	API_IMPORT extern const StatusCode Unauthorized;
	API_IMPORT extern const StatusCode Forbidden;
	API_IMPORT extern const StatusCode NotFound;
	API_IMPORT extern const StatusCode RangeNotSatisfiable;
													
	// 5xx: server error							
	API_IMPORT extern const StatusCode InternalServerError;
	API_IMPORT extern const StatusCode NotImplemented;
	API_IMPORT extern const StatusCode BadGateway;
	API_IMPORT extern const StatusCode ServiceNotAvailable;
	API_IMPORT extern const StatusCode GatewayTimeout;
	API_IMPORT extern const StatusCode VersionNotSupported;
	
};
};