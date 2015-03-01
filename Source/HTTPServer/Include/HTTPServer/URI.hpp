#pragma once

#include "Common.hpp"
#include "FilePath.hpp"

namespace HTTP
{
	typedef Map<String, String> ParamMap;
	struct API_EXPORT URI
	{
	public:
		static URI Parse(const String& data);
		String static ToString(const URI& uri);

		String GetParameter(String& key);
		
		String scheme; 		// "http"
		String userInfo; 	// "hi"@
		String host; 		// "www.example.com"
		UInt port; 			// :"8080"
		FilePath path; 		// "/path/to.file" also can end with a forward slash: "/path/"
		ParamMap params; 	// "?key=value"
		String fragment; 	// #"fragment"
	};
}