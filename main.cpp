#include <iostream>
#include "server.hpp"
#include "http_request.hpp"
#include "http_response.hpp"

int main()
{

	Server *server = new Server();

	server->cache_file("public/html/index.html");

	server->use([](HTTP_Request *request, HTTP_Response *response) {

		std::cout << request->get_path() << "\n";

		response->init_response(200);

		response->send_file("public/html/index.html");
	});

	server->bind(8080);

	delete server;
	return 0;
}