#pragma once

#include "Common.hpp"

API_EXPORT extern String fileDirectory;
API_EXPORT String GetFileDirectory();
API_EXPORT void SetFileDirectory(String fileDir);

class API_EXPORT FileResource
{
public:
	FileResource() {};

	bool Open(String filename);

	String Get(void) { return mContent; };

	bool IsOpen(void) { return !mContent.empty(); };

private:
	String mContent;
	String mFilename;
};