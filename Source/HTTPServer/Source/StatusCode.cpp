#include <HTTPServer/StatusCode.hpp>

namespace HTTP
{
	namespace StatusCode
	{
		API_EXPORT const StatusCode Continue(100, "Continue", "You should've be seeing this");
		API_EXPORT const StatusCode SwitchingProtocols(101, "Switching Protocols", "You should've be seeing this");

		API_EXPORT const StatusCode OK(200, "OK", "You should've be seeing this");
		API_EXPORT const StatusCode Created(201, "Created", "You should've be seeing this");
		API_EXPORT const StatusCode Accepted(202, "Accepted", "You should've be seeing this");
		API_EXPORT const StatusCode NoContent(203, "No Content", "You should've be seeing this");
		API_EXPORT const StatusCode ResetContent(204, "Reset Content", "You should've be seeing this");
		API_EXPORT const StatusCode PartialContent(205, "PartialContent", "You should've be seeing this");

		API_EXPORT const StatusCode MultipleChoices(300, "Multiple Choice", "Multiple choices exist"); //No shit sherlock
		API_EXPORT const StatusCode MovedPermanently(301, "Moved Permanantly", "The website has been moved to a new location");
		API_EXPORT const StatusCode MovedTemporarily(302, "Moved Temporarily", "Moved Temporarily");
		API_EXPORT const StatusCode NotModified(304, "Not Modified", "What?");

		API_EXPORT const StatusCode BadRequest(400, "Bad Request", "The client request could not be understood by the server, it may be malformed");
		API_EXPORT const StatusCode Unauthorized(401, "Unauthorized", "You do not have access to the requested resource");
		API_EXPORT const StatusCode Forbidden(402, "Forbidden", "Access to this resource has been disallowed");
		API_EXPORT const StatusCode NotFound(404, "Not Found", "The requested resource could not be located on the server");
		API_EXPORT const StatusCode RangeNotSatisfiable(407, "Range Not Satisfiable", "TODO: Document this");

		API_EXPORT const StatusCode InternalServerError(500, "Internal Server Error", "The server could not complete the request");
		API_EXPORT const StatusCode NotImplemented(501, "Method Not Implemented", "The requested HTTP method is not available on this server");
		API_EXPORT const StatusCode BadGateway(502, "Bad Gateway", "Bad Gateway");
		API_EXPORT const StatusCode ServiceNotAvailable(503, "Service Not Available", "Service Not Available");
		API_EXPORT const StatusCode GatewayTimeout(504, "Gateway Timeout", "The gateway has timed out");
		API_EXPORT const StatusCode VersionNotSupported(505, "Version Not Supported", "The client version is not supported");
	}
}