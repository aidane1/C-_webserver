#ifndef HTTP_REQUEST
#define HTTP_REQUEST


#include <string>
#include <vector>
#include <map>


enum Method {
	GET,
	POST,
	PUT,
	DELETE,
};


struct Header {
	std::string name;
	std::string value;

	Header();

	Header(std::string header_line);
};


class HTTP_Request {
	private:
	std::string raw_body;


	Method method; // ex. GET

	std::string path; // ex. /home/users

	std::string protocol; // ex. HTTP/1.1

	std::vector<Header *> raw_headers;

	std::map<std::string, std::string> headers;
	

	public:

	HTTP_Request(char *request);

	~HTTP_Request();

	std::string get_path();
};


#endif