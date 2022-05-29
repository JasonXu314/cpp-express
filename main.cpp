#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>

#include "framework/Server.h"

using namespace std;

int main() {
	Server server(3000);

	server
		.get("/",
			 [](const Request& req) {
				 stringstream ss;

				 ss << "<html>\n"
					<< "<head>\n"
					<< "<title>Hello, World!</title>\n"
					<< "</head>\n"
					<< "<body>\n"
					<< "<h1>Hello, World!</h1>\n";

				 if (req.query().count("name")) {
					 ss << "<p>Hello, " << req.query("name") << "!</p>\n";
				 }

				 ss << "</body>\n"
					<< "</html>\n";

				 return ss.str();
			 })
		.post("/", [](const Request& req) {
			stringstream ss;

			ss << "<html>\n"
			   << "<head>\n"
			   << "<title>Hello, World!</title>\n"
			   << "</head>\n"
			   << "<body>\n"
			   << "<h1>Hello, World!</h1>\n"
			   << "<p>Hello, " << req.body() << "!</p>\n"
			   << "</body>\n"
			   << "</html>\n";

			return ss.str();
		});

	server.start();
	server.wait();

	return 0;
}