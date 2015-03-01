#include <HTTPServer/HTTPServer.hpp>
#include <lua.hpp>
#include <chrono>

namespace Lua
{

	lua_State* L;

	void Start()
	{
		L = lua_open();
		luaL_openlibs(L);
	}

	void Close()
	{
		lua_close(L);
	}

	//traceback print function
	int Traceback(lua_State* L)
	{
		if (!lua_isstring(L, 1)) { /* Non-string error object? Try metamethod. */
			if (lua_isnoneornil(L, 1) ||
				!luaL_callmeta(L, 1, "__tostring") ||
				!lua_isstring(L, -1))
				return 1;  /* Return non-string error object. */
			lua_remove(L, 1);  /* Replace object by result of __tostring metamethod. */
		}
		luaL_traceback(L, L, lua_tostring(L, 1), 1);
		return 1;
	}

	//wraps up pcall with traceback
	int DoCall(const int nargs)
	{
		int base = lua_gettop(L) - nargs; // top-nargs /* function index */
		lua_pushcfunction(L, Traceback);  /* push traceback function */
		lua_insert(L, base);  /* put it under chunk and args */

		int status = lua_pcall(L, nargs, LUA_MULTRET, base);

		lua_remove(L, base);  /* remove traceback function */
		/* force a complete garbage collection in case of errors */
		if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
		return status;
	}
	
	//returns number of values
	int CallFunction(const int nargs)
	{
		int top = lua_gettop(L) - 1;

		if (DoCall(nargs) != -1)
			return lua_gettop(L) - top;
		else
			return 0;
	}

	int DoString(const String& source, const String& chunkName = "chunk")
	{
		int status = luaL_loadbuffer(L, source.c_str(), source.size(), chunkName.c_str());
		if (status != -1)
			return CallFunction(0);

		return status;
	}

}

class LuaHandler : virtual public HTTP::IHandler
{
public:
	virtual bool operator()(const HTTP::Request& request, HTTP::Response& response, const HTTP::Server& server) const override
	{
		if (request.GetPath().fileExt == ".lua"){
			FileResource file;
			if (file.Open(request.GetPath().filePath))
			{
				// Send server and response into
				// the doString as UserData

				int vars = Lua::DoString(file.Get(), request.GetPath().file);

				if (vars > 0)
				{
					HTTP::Response::SetStatus(response, HTTP::StatusCode::OK);
					response.SetBody(String(lua_tostring(Lua::L, -1), lua_strlen(Lua::L, -1)));
					response.SetHeader("Content-Type", "text/html");
					return true;
				}
			}
		}
		return false;
	}
};

#include <condition_variable>
#include <Windows.h>
std::mutex m;
std::condition_variable cv;


HTTP::Server server;
//Create server with default mime types

BOOL exit_handler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		server.Stop(false);
		Lua::Close();
		//while (server.IsRunning())
		//	sleep(10ms);
		return TRUE;
	}
	return FALSE;
}

int main(int* argc, char** argv)
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) ( exit_handler ), TRUE);

	Lua::Start();


	//server.mHandlerMap[String(/*"\\.lua$"*/".+")] = std::make_shared<LuaHandler>();
	server.AddHandler<LuaHandler>(/*\\.lua$"*/".+");

	printf("Starting\n");
	server.Start(9398); //9398 is externally connectable

	//lua closes server

	/*while (server.IsRunning())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1)); //this thread doesnt have to be running at all
	}*/

	{
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, []{return !server.IsRunning(); });
	}

	printf("Stopping\n");
	server.Stop(false);

	Lua::Close();

	return 0;
}