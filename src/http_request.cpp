#include "http_request.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <vector>
#include <string>
#include <string.h>
#include <map>

std::string HTTP_Request::get_path() {
	return path;
}

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos)
			pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty())
			tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

Header::Header(std::string header_line)
{
	int seperation_index = 0;
	for (int i = 0; i < header_line.size(); i++)
	{
		if (header_line[i] == ':')
		{
			seperation_index = i;
			break;
		}
	}

	std::string name = header_line.substr(0, seperation_index);

	std::string value = header_line.substr(seperation_index + 2, header_line.length() - seperation_index - 2);

	this->name = name;
	this->value = value;
}

HTTP_Request::HTTP_Request(char *body)
{
	std::string raw = std::string(body);

	raw_body = raw;

	std::vector<std::string> lines = split(raw, "\n");

	std::vector<std::string> first_line = split(lines[0], " ");

	if (first_line[0] == "GET")
	{
		method = GET;
	}
	else if (first_line[0] == "POST")
	{
		method = POST;
	}
	else
	{
		method = GET;
	}

	path = first_line[1];

	protocol = first_line[2];

	for (int i = 1; i < lines.size()-1; i++)
	{
		Header *current = new Header(lines[i]);
		raw_headers.push_back(current);
		headers[current->name] = current->value;
	}
}


HTTP_Request::~HTTP_Request() {
	for (int i = 0; i < raw_headers.size(); i++) {
		delete raw_headers[i];
	}
}