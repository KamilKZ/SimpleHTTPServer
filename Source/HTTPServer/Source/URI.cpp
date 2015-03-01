#include <sstream>

#include <HTTPServer/URI.hpp>

namespace HTTP
{

URI URI::Parse(const String& data)
{
	URI uri;

	std::istringstream in(data);
	in.str(data);

	//query = substr(path, "?")
	//fragment = substr(path, "#")
	//regex(query, "& or ;")

	String sPath;
	in >> sPath;
	uri.path = FilePath::Parse(sPath);

	//uri.path = FilePath::Parse(path);
	
	return uri;
}

String URI::GetParameter(String& key)
{
	ParamMap::const_iterator it = params.begin();
	for(it; it != params.end(); ++it)
	{
		if (it->first == key)
		{
			return it->second;
		}
	}
	return "";
}

};