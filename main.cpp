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
			 [](const Request& req, const Response& res) {
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
				 res.header("Content-Type", "text/html").send(ss.str());

				 res.send(ss.str());
			 })
		.post("/",
			  [](const Request& req, const Response& res) {
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

				  res.status(201);
				  res.header("Content-Type", "text/html");

				  res.send(ss.str());
			  })
		.get("/404", [](const Request& req, const Response& res) {
			res.status(404);
			res.header("Content-Type", "text/html");
			res.send(
				"<html>\n"
				"<head>\n"
				"<title>404 Not Found</title>\n"
				"</head>\n"
				"<body>\n"
				"<h1>404 Not Found</h1>\n"
				"</body>\n"
				"</html>\n");
		});

	server.start();
	server.wait();

	return 0;
}