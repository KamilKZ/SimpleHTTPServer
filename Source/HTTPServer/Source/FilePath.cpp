#include <HTTPServer/FilePath.hpp>

#include <regex>

namespace HTTP
{

	FilePath FilePath::Parse(String& data)
	{
		FilePath fp;
		fp.filePath = data;
	
		std::smatch match;


		//-----------------------------------
		// Directory/Path RegEx
		//-----------------------------------
		// Holy hell
		// \\/.*\\/ means: /characters.../
		// \\/?.*\\/ means: optional/characters.../
		// 					which will allow: path/to/file.file, /path/to/file.file and also /file.file
		//
		// \\/? means optional(single) '/' (escaped)
		// .* means optional(multiple) any-character
		// \\/ means required '/'
		// The RegEx only works for directories delimited by '/'
		// I.E.: '/folder' will act as a file
		
		std::regex_search(data, match, std::regex("\\/?.*\\/"));
		
		if(match.size() != 1)
		{
			if(match.size() == 0)
				fp.path = "";
			else
				fp.path = match[0];
				
			printf("Warning, unexpected request path handled:\n");
			printf("\trequest: %s\n", data.c_str());
			printf("\tunderstood base path: %s\n", match[0]);
			printf("\tregex matches: %i, expected: 1\n", match.size());
		}
		else
		{
			fp.path = match[0];
		}
		
		//-----------------------------------
		// File RegEx
		//-----------------------------------
		// [^\\/]+ means any apart from '/'
		// \\. means period '.' (escaped)
		// $ means EOL
		// The RegEx is also valid for filenames WITHOUT an extension: 
		// both /file.file and /file work.
		
		std::regex_search(data, match, std::regex("[^\\/]+$"));
		
		if(match.size() > 1)
		{
			fp.file =  match[0];
				
			printf("Warning, unexpected request path handled:\n");
			printf("\trequest: %s\n", data.c_str());
			printf("\tunderstood file extension: %s\n", match[0]);
			printf("\tregex matches: %i, expected: 1\n", match.size());
		}
		else
		{
			fp.file = (match.size() == 1) ? match[0] : String("");
		}
		
		//-----------------------------------
		// File Extension RegEx
		//-----------------------------------
		// \\. means period (escaped)
		// [^\\/\\.]+ means many-any characters excluding '/' and '.'
		// $ means EOL
		
		std::regex_search(data, match, std::regex("\\.[^\\/\\.]+$"));
		
		if(match.size() > 1)
		{
			fp.fileExt =  match[0];
				
			printf("Warning, unexpected request path handled:\n");
			printf("\trequest: %s\n", data.c_str());
			printf("\tunderstood file extension: %s\n", match[0]);
			printf("\tregex matches: %i, expected: 1\n", match.size());
		}
		else
		{
			fp.fileExt = (match.size() == 1) ? match[0] : String("");
		}
		
		//-----------------------------------
		// File Name
		//-----------------------------------
		// Just subtract fileExt from file

		int length = fp.file.size() - fp.fileExt.size();
		fp.fileName = fp.file.substr(0, length);
		
		return fp;
	}
}