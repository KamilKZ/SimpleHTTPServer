#include <HTTPServer/FileResource.hpp>

#include <sstream>

String fileDirectory = "../Data/";

String GetFileDirectory()
{
	return fileDirectory;
}

void SetFileDirectory(String fileDir)
{
	fileDirectory = fileDir;
}

bool FileResource::Open(String filename)
{
	std::FILE* file(std::fopen((fileDirectory+filename).c_str(), "rb"));

	if (file == nullptr)
		return false;

	std::ostringstream stream;

	std::vector<char> buffer(1024);
	UInt chunk_size = 0;

	do
	{
		chunk_size = std::fread(&buffer[0], sizeof buffer[0], buffer.size(), file);
		//chunk_size = number of bytes read
		//&buffer[0] = pointer to start of buffer
		//sizeof buffer[0] = element size (1 byte)
		//buffer.size() = size of buffer (1024 * 1 byte = 1024 bytes)
		//file = pointer to open file handle

		stream << String((char*)(&buffer[0]), chunk_size);
		//push to stream, because string.
	}
	while (chunk_size == buffer.size()); //chunk size is full; not EOF

	mContent = stream.str();

	fclose(file);

	return !mContent.empty();
}