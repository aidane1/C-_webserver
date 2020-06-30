#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <sstream>
#include <string>
#include <map>
#include <ctime>

#include "server.hpp"

class Server;

class HTTP_Response
{
private:
	int sd;
	int *client_socket;
	Server *server;

	std::stringstream response_headers;
	std::stringstream response_body;

	int status;

	static std::string get_response_text(int status);
	static std::string format_time(std::time_t time);

	bool sent = false;

public:
	HTTP_Response(int sd, int *client_socket, Server *server);

	void init_response(int status);

	void add_body(std::string body);

	void send_file(std::string path);

	void send();

	bool isSent();

	// HTTP_Response();

	// HTTP_Response(int status_code);
};

#endif