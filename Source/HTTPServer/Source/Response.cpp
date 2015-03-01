#include <HTTPServer/Response.hpp>
#include <HTTPServer/StatusCode.hpp>
#include <sstream>
#include <cctype>
#include <limits>

String toString(UInt i)
{
	std::ostringstream convert;
	convert << i;
	return convert.str();
}

namespace HTTP
{
	/*
	
	Response Message = Status-Line CRLF
						Headers CRLF
						CRLF
						Message Body

	Status-Line = HTTP-Version SP Status Code SP Status Phrase CRLF

	*/


	String Response::Prepare(const Response& r)
	{
		std::ostringstream out;

		out << "HTTP/1.1 " << r.mStatusCode << " " << r.mStatusPhrase << "\r\n";

		// Write headers
		for (Map<String, String>::const_iterator i = r.mHeaders.begin(); i != r.mHeaders.end(); ++i)
		{
			out << i->first << ": " << i->second << "\r\n";
		}

		// Use an extra \r\n to separate the header from the body
		out << "\r\n";

		// Add the body
		out << r.mBody;

		return out.str();
	}

	Response Response::Parse(const String& data)
	{
		Response r;

		std::istringstream in;
		in.str(data);

		// Extract the HTTP version
		String version;
		UInt versionMajor;
		UInt versionMinor;
		if (in >> version) //ex: HTTP/1.1
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
				return Response();
			}
		}

		// Extract Status Code
		in >> r.mStatusCode; //ex: 404
		
		// Extract Status Phrase
		//fetch string up to '\r', exclude preceding ' ' if it exists
		std::ostringstream out;

		char c = in.get();
		if (c != '\r' && c != ' ')
			out << c;

		while (in.good())
		{
			char c = in.get();
			if (c == '\r')
				break;
			out << c;
		}

		c = in.get(); // get rid of \n

		r.mStatusPhrase = out.str();

		// Extract headers
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
				r.mHeaders[toLower(field)] = value;
			}
		}

		// Cut off CRLF
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Body
		std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(r.mBody));

		return r;
	}


	String Response::GetHeader(String header) const {
		Map<String, String>::const_iterator it = mHeaders.find(header);
		if (it == mHeaders.end())
		{
			return "";
		}
		else
		{
			return it->second;
		}
	};
	
	void Response::SetStatus(Response& response, StatusCode::StatusCode status)
	{
		std::ostringstream page;

		page <<
			"<html>\n"\
			"<head>\n"\
				"\t<title>" << status.GetCode() << " " << status.GetPhrase() << "</title>\n"\
			"</head>\n"\
			"<body>\n"\
				"\t<h1>" << status.GetCode() << " " << status.GetPhrase() << "</h1>\n"\
				"\t" << status.GetMessage() << "\n"\
			"</body>\n"\
			"</html>";

		response.SetStatusCode(status.GetCode());
		response.SetStatusPhrase(status.GetPhrase());
		response.SetBody(page.str());
		response.SetHeader("Content-Type", "text/html");
	}
};