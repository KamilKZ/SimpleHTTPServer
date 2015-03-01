#pragma once

#include "Common.hpp"

namespace HTTP
{
	struct API_EXPORT FilePath
	{
	public:
		static FilePath Parse(String& data);
		
		String filePath; 	// "/path/to.file"
		String path; 		// "/path/"
		String file;		// "to.file"
		String fileName; 	// "to"
		String fileExt; 	// ".file"
	};
};