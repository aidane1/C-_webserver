#ifndef SERVER
#define SERVER

#include <netinet/in.h>
#include <vector>
#include <map>
#include "http_request.hpp"
#include "http_response.hpp"

class HTTP_Response;

class Server
{

private:
	// a vector of pointers to functions that take two paramaters (a pointer to an HTTP_Request
	// and a pointer to an HTTP_Response) and returns void
	std::vector<void (*)(HTTP_Request *, HTTP_Response *)> middleware;

	std::map<std::string, std::string> file_cache;

public:
	void bind(int port);

	void use(void (*middleware_func)(HTTP_Request *, HTTP_Response *));

	void cache_file(std::string path);

	// hit: true or false, then value
	std::pair<bool, std::string> read_cache(std::string path);
	
	std::string read_file(std::string path, bool allow_cache = true);

	std::string read_file_and_cache(std::string path, bool allow_cache = true);

	// void handle_request(http_request)
};

#endif