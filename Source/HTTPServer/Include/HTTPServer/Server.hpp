#pragma once

//#include <Elson.hpp> // Json
//#include <functional>
#include <SFML/Network.hpp> // Third-party/Common includes at the top because they might be re-included in headers which is pointless
#include <memory>

#include "Common.hpp"
#include "Thread.hpp"
#include "IHandler.hpp"
#include "Defaults.hpp"

namespace HTTP
{
	class Request;
	class Response;

	typedef sf::TcpListener TcpListener;
	typedef Map<String, String> MimeTypesMap;
	typedef Map<String, std::shared_ptr<IHandler>> HandlerMap;
	typedef ArrayList<std::shared_ptr<Connection>> ConnectionsList;

	class API_EXPORT Server : public Thread
	{
	public:
		/////////////////////////////////////////////////
		/// \brief Default Constructor
		/// \param mimeTypesMap(optional) MIME-Types to set
		///
		/////////////////////////////////////////////////
		Server(MimeTypesMap mimeTypesMap = _defaultMimeTypes);

		/////////////////////////////////////////////////
		/// \brief Default Destructor
		///
		/////////////////////////////////////////////////
		virtual ~Server();
		
		/////////////////////////////////////////////////
		/// \brief Start the server
		///
		/// This function starts the server on the specified
		/// port, or port 80 if the port paramater is not
		/// passed.
		/// The server will run fine and handle requests after
		/// this function has been called, until stopped
		///
		/// \param port Port to run server on (default: 80)
		///
		/// \see Thread::Stop
		///
		/////////////////////////////////////////////////
		bool Start(UInt port = 80);

		/////////////////////////////////////////////////
		/// \brief Set new or replace existing MIME-Type
		///
		/// The MIME-Type is used when defaulting to the
		/// FileHandler. They're bound to file regexes, for
		/// example, all '.html' files are set to text/html
		///
		/// \param regex Regex to check filename (including path) against
		/// \param mimeType MIME-Type to use for this kind of file
		///
		/////////////////////////////////////////////////
		void SetMimeType(const String& regex, const String& mimeType);

		/////////////////////////////////////////////////
		/// \brief Retrieves a MIME-Type by the regex
		///
		/// \param regex Regex by which the MIME-Type is identified
		///
		/// \return MIME-Type, or empty string
		///
		/////////////////////////////////////////////////
		String GetMimeType(const String& regex) const;

		/////////////////////////////////////////////////
		/// \brief Look up what MIME-Type this file is
		///
		/// Performs a check of the regex for every MIME-Type,
		/// and if a match is found, the MIME-Type is returned.
		/// If no match has been found, an empty string is returned.
		/// This function is used by the FileHandler to look up a
		/// file's MIME-Type, or whether there is none.
		///
		/// \param filename Filename (including path) of file to look up
		///
		/// \return MIME-Type, or empty string
		///
		/////////////////////////////////////////////////
		String FindMimeType(const String& filename) const;

		/////////////////////////////////////////////////
		/// \brief Set the exit(status) code for shutdown
		///
		/// This is not used by the server itself, yet
		///
		/// \param exitCode Exit code to set
		///
		/////////////////////////////////////////////////
		void SetExitCode(const UInt exitCode) { mExitCode = exitCode; }

		/////////////////////////////////////////////////
		/// \brief Retrieve the exit(status) code
		///
		/// \return Exit code
		///
		/////////////////////////////////////////////////
		UInt GetExitCode(void) const { return mExitCode; }
		
		
		/////////////////////////////////////////////////
		/// \brief Register a new handler
		///
		/// This function will create and add a new handler
		/// to the internal list of handlers
		///
		/// Handlers are very flexible, mostly due to the fact
		/// that the regex used here is for testing the filename
		/// and path, and the Request and Response objects are
		/// passed through to the handler functions.
		///
		/// \param template The handler class(type) to add
		/// \param regex The filename and/or path this handler applies to
		///
		/// \see HTTP::IHandler
		///
		/////////////////////////////////////////////////
		template <typename T>
		API_EXPORT void AddHandler(const String& regex)
		{
			std::shared_ptr<T> handler = std::make_shared<T>();
			mHandlerMap[regex] = handler;
		}
		/* Definition is in headers because templates are a compile-time type and don't work
			for a compiled library, whereas headers are not compiled with the library */

	protected:
		friend class Connection; //handle http request???
			Response HandleRequest(Request& request); // Access by connection
			void DestroyConnection(Connection* connection); // ditto

		friend class Thread; //used for calling ::Run
			void* Run(void) override; // Used only by Thread and should not be used elsewhere
	
		std::atomic<UInt> mExitCode;
		std::atomic<UInt> mPort;

		TcpListener mListener;
		MimeTypesMap mMimeTypes;
		HandlerMap mHandlerMap;
		std::shared_ptr<IHandler> fileHandler;
		ConnectionsList mConnections;
	};

	/////////////////////////////////////////////////
	/// \class HTTP::Server
	///
	/// This class is what is used to create a HTTP server.
	/// The server handles multiple asynchronous connections,
	/// handlers, and retrieval or files and mimetypes as a
	/// fallback in case there has been no handler taking care
	/// of the request. This means that this isn't intended to
	/// be used as a standard HTTP (web) server, where the files
	/// are just requested and retreived, but for customized
	/// requests and handlers. The server is just there for the
	/// protocol.
	///
	/// Using the server is very simple, and everything is
	/// taken care of accordingly at destruction.
	/// The server has a nice object-oriented approach and
	/// creating a new server is as easy as doing:
	/// \code
	/// HTTPServer::Server server;
	/// server.Start(); // defaults to port 80
	///
	/// waitUntil(!running);
	/// 
	///	server.Stop();
	/// \endcode
	/////////////////////////////////////////////////
};