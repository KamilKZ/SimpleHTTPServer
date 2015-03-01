#include <sstream>
#include <cctype>
#include <limits>

#include <HTTPServer/Request.hpp>

// Convert a string to lower case
String API_EXPORT toLower(String str)
{
	for (std::string::iterator i = str.begin(); i != str.end(); ++i)
		*i = static_cast<char>( std::tolower(*i) );
	return str;
}

namespace HTTP
{
	
	//http://tools.ietf.org/html/rfc2616#section-5
	/*

	Request Message = Request-Line CRLF
						Headers CRLF
						CRLF
						Message Body

	Request-Line = Method SP Request-URI SP HTTP-Version CRLF
	
	*/

	String Request::Prepare(const Request& r)
	{
		std::ostringstream out;

		switch (r.mMethod)
		{
			case Method::GET:
				out << "GET";
				break;
			case Method::POST:
				out << "POST";
				break;
			case Method::HEAD:
				out << "HEAD";
				break;
			default:
				out << "ERROR";
				break;
		}

		out << " " << r.mURI.path.filePath << " HTTP/1.1" << "\r\n"; //ex: "GET /hi.html HTTP/1.1 CRLF"

		Map<String, String>::const_iterator it = r.mHeaders.begin();
		for (it; it != r.mHeaders.end(); it++)
		{
			out << it->first << ": " << it->second << "\r\n"; //ex: Content-Type: text/html CRLF
		}

		out << "\r\n"; // CRLF

		if (!r.mBody.empty())
		{
			out << r.mBody; // Message Body
		}

		return out.str();
	}

	Request Request::Parse(const String& data)
	{
		Request r;

		std::istringstream in(data);
		in.str(data);

		// Extract the method
		String method;
		if (in >> method)
		{
			if ( method.size() >= 3 && method.size() <= 10 )
			{
				if (toLower(method.substr(0, 3)) == "get")
					r.mMethod = Method::GET;
				else if (toLower(method.substr(0, 4)) == "post")
					r.mMethod = Method::POST;
				else if (toLower(method.substr(0, 4)) == "head")
					r.mMethod = Method::HEAD;
			}
			else
			{
				// Invalid HTTP version
				return Request();
			}
		}

		// Extract the path
		String path;
		in >> path;
		//parsed after headers for 'host' uri

		// Extract the HTTP version
		String version;
		UInt versionMajor;
		UInt versionMinor;
		if (in >> version)
		{
			if (( version.size() >= 8 ) && ( version[6] == '.' ) &&
				( toLower(version.substr(0, 5)) == "http/" ) &&
				isdigit(version[5]) && isdigit(version[7]))
			{
				versionMajor = version[5] - '0';
				versionMinor = version[7] - '0';
			}
			else
			{
				return Request();
			}
		}

		// Ignore the end of the first line
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Parse the other lines, which contain fields, one by one
		r.ParseHeaders(in);

		r.mBody.clear();

		// Determine whether the transfer is chunked
		if (toLower(r.GetHeader("transfer-encoding")) != "chunked")
		{
			// Not chunked - just read everything at once
			std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(r.mBody));
		}
		else
		{
			// Chunked - have to read chunk by chunk
			std::size_t length;

			// Read all chunks, identified by a chunk-size not being 0
			while (in >> std::hex >> length)
			{
				// Drop the rest of the line (chunk-extension)
				in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				// Copy the actual content data
				std::istreambuf_iterator<char> it(in);
				for (std::size_t i = 0; i < length; i++)
					r.mBody.push_back(*it++);
			}

			// Drop the rest of the line (chunk-extension)
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			// Read all trailers (if present)
			r.ParseHeaders(in);
		}

		//parsed after headers for 'host'
		r.mURI = URI::Parse(/*r.GetHeader("Host") +*/ path);

		return r;
	}

	void Request::ParseHeaders(std::istream &in)
	{
		std::string line;
		while (std::getline(in, line) && ( line.size() > 2 ))
		{
			std::string::size_type pos = line.find(": ");
			if (pos != std::string::npos)
			{
				// Extract the field name and its value
				std::string field = line.substr(0, pos);
				std::string value = line.substr(pos + 2);

				// Remove any trailing \r
				if (!value.empty() && ( *value.rbegin() == '\r' ))
					value.erase(value.size() - 1);

				// Add the field
				mHeaders[toLower(field)] = value;
			}
		}
	}

	String Request::GetHeader(String header) const
	{
		Map<String, String>::const_iterator it = mHeaders.find(header);
		if (it == mHeaders.end())
		{
			return "";
		}
		else
		{
			return it->second;
		}
	}

	String Request::GetParam(String param) const
	{
		Map<String, String>::const_iterator it = mParams.find(param);
		if (it == mParams.end())
		{
			return "";
		}
		else
		{
			return it->second;
		}
	}
};