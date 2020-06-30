#include "http_response.hpp"
#include "server.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <string>
#include <string.h>
#include <map>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// HTTP_Response::HTTP_Response()
// {
// 	this->status = 200;
// 	init_response(status);
// }

// HTTP_Response::HTTP_Response(int status)
// {
// 	this->status = status;
// 	init_response(status);
// }

bool HTTP_Response::isSent()
{
	return sent;
}

HTTP_Response::HTTP_Response(int sd, int *client_socket, Server *server) : sd(sd), client_socket(client_socket), server(server)
{
}

void HTTP_Response::send_file(std::string path)
{
	std::string body = server->read_file_and_cache(path);
	response_headers << "Content-Length: " << body.length() << "\n";
	response_body << body;
	send();
}

void HTTP_Response::send()
{
	std::string response = response_headers.str() + "\n" + response_body.str();
	::send(sd, response.c_str(), response.length(), 0);
	::close(sd);
	*client_socket = 0;
	sent = true;
}

void HTTP_Response::init_response(int status)
{
	this->status = status;

	std::string response_text = HTTP_Response::get_response_text(status);
	std::string time_string = HTTP_Response::format_time(time(0));

	response_headers << "HTTP/1.1 " << status << " " << response_text << "\n";
	response_headers << "Date: " << time_string << "\n";
	response_headers << "Server: Eggy/0.0.1 (Unix)\n";

	response_headers << "Connection: Closed\n";
	response_headers << "Content-Type: text/html; charset=iso-8859-1\n";
}

void HTTP_Response::add_body(std::string body)
{
	response_headers << "Content-Length: " << body.length() << "\n";
	response_body << body;
}

std::string HTTP_Response::get_response_text(int status)
{
	std::map<int, std::string> response_codes;

	response_codes[100] = "Continue";
	response_codes[101] = "Switching Protocols";
	response_codes[200] = "OK";
	response_codes[202] = "Accepted";
	response_codes[203] = "Non-Authoritative Information";
	response_codes[204] = "No Content";
	response_codes[205] = "Reset Content";
	response_codes[206] = "Partial Content";
	response_codes[300] = "Multiple Choices";
	response_codes[301] = "Moved Permanently";
	response_codes[302] = "Found";
	response_codes[303] = "See Other";
	response_codes[304] = "Not Modified";
	response_codes[305] = "Use Proxy";
	response_codes[306] = "(Unused)";
	response_codes[307] = "Temporary Redirect";
	response_codes[400] = "Bad Request";
	response_codes[401] = "Unauthorized";
	response_codes[402] = "Payment Required";
	response_codes[403] = "Forbidden";
	response_codes[404] = "Not Found";
	response_codes[405] = "Method Not Allowed";
	response_codes[406] = "Not Acceptable";
	response_codes[407] = "Proxy Authentication Required";
	response_codes[408] = "Request Timeout";
	response_codes[409] = "Conflict";
	response_codes[410] = "Gone";
	response_codes[411] = "Length Required";
	response_codes[412] = "Precondition Failed";
	response_codes[413] = "Request Entity Too Large";
	response_codes[414] = "Request-URI Too Long";
	response_codes[415] = "Unsupported Media Type";
	response_codes[416] = "Requested Range Not Satisfiable";
	response_codes[417] = "Expectation Failed";
	response_codes[500] = "Internal Server Error";
	response_codes[501] = "Not Implemented";
	response_codes[502] = "Bad Gateway";
	response_codes[503] = "Service Unavailable";
	response_codes[504] = "Gateway Timeout";
	response_codes[505] = "HTTP Version Not Supported";

	std::map<int, std::string>::iterator it = response_codes.find(status);

	if (it != response_codes.end())
	{
		return response_codes[status];
	}
	else
	{
		return "Not Found";
	}
}

std::string HTTP_Response::format_time(std::time_t time)
{
	std::vector<std::string> days_of_week = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	std::vector<std::string> months_of_year = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev"};

	tm *ltm = std::localtime(&time);

	std::string min = std::to_string(1 + ltm->tm_min);
	std::string sec = std::to_string(ltm->tm_sec);

	if (min.length() == 1)
	{
		min = "0" + min;
	}

	if (sec.length() == 1)
	{
		sec = "0" + sec;
	}

	std::stringstream dateString;
	dateString << days_of_week[ltm->tm_wday];
	dateString << ", ";
	dateString << ltm->tm_mon;
	dateString << " ";
	dateString << months_of_year[ltm->tm_mon];
	dateString << " ";
	dateString << (ltm->tm_year + 1900);
	dateString << " ";
	dateString << ltm->tm_hour << ":";
	dateString << min << ":";
	dateString << sec;
	dateString << " ";
	dateString << ltm->tm_zone;

	return dateString.str();
}