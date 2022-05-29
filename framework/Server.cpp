#include "Server.h"

using namespace std;

Server::Server(int port) : _port(port) {
	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	_buffer = new char[BUFFER_SIZE];

	bind(_serverSocket, (sockaddr*)&addr, sizeof(addr));
}

void Server::start() {
	listen(_serverSocket, 5);

	_thread = thread([this]() {
		ofstream log("server.log");

		while (true) {
			int clientSocket = accept(_serverSocket, nullptr, nullptr);

			recv(clientSocket, _buffer, BUFFER_SIZE, MSG_WAITFORONE);

			log << "Raw Request:\n" << _buffer << endl;

			Request req(_buffer);

			if (!_handlers.count(req.path())) {
				stringstream notFoundSS;

				notFoundSS << "<html>\n"
						   << "<head>\n"
						   << "<title>404 Not Found</title>\n"
						   << "</head>\n"
						   << "<body>\n"
						   << "<h1>404 Not Found</h1>\n"
						   << "</body>\n"
						   << "</html>\n";

				stringstream ss;
				ss << "HTTP/1.1 404 Not Found\n"
				   << "Content-Type: text/html\n"
				   << "Content-Length: " << notFoundSS.str().length() << "\n"
				   << "\n"
				   << notFoundSS.str();

				send(clientSocket, ss.str().c_str(), ss.str().length(), 0);
			} else if (!_handlers[req.path()].count(req.method())) {
				stringstream methodNotAllowedSS;

				methodNotAllowedSS << "<html>\n"
								   << "<head>\n"
								   << "<title>405 Method Not Allowed</title>\n"
								   << "</head>\n"
								   << "<body>\n"
								   << "<h1>405 Method Not Allowed</h1>\n"
								   << "</body>\n"
								   << "</html>\n";

				stringstream ss;
				ss << "HTTP/1.1 405 Method Not Allowed\n"
				   << "Content-Type: text/html\n"
				   << "Content-Length: " << methodNotAllowedSS.str().length() << "\n"
				   << "\n"
				   << methodNotAllowedSS.str();

				send(clientSocket, ss.str().c_str(), ss.str().length(), 0);
			} else {
				Response res([&clientSocket, &res]() {
					stringstream ss;
					ss << "HTTP/1.1 " << res._status << " OK\n";

					for (auto& header : res._headers) {
						ss << header.first << ": " << header.second << "\n";
					}

					if (!res._headers.count("Content-Type")) {
						ss << "Content-Type: text/plain\n";
					}

					ss << "Content-Length: " << res._body.length() << "\n"
					   << "\n"
					   << res._body;

					send(clientSocket, ss.str().c_str(), ss.str().length(), 0);
				});

				_handlers[req.path()][req.method()](req, res);
			}

			close(clientSocket);
			_clearBuffer();
		}
	});
}

void Server::wait() { _thread.join(); }

Server& Server::get(const string& path, const function<void(const Request&, const Response&)>& handler) {
	_handlers[path][Method::GET] = handler;
	return *this;
}

Server& Server::post(const string& path, const function<void(const Request&, const Response&)>& handler) {
	_handlers[path][Method::POST] = handler;
	return *this;
}

Server& Server::put(const string& path, const function<void(const Request&, const Response&)>& handler) {
	_handlers[path][Method::PUT] = handler;
	return *this;
}

Server& Server::del(const string& path, const function<void(const Request&, const Response&)>& handler) {
	_handlers[path][Method::DELETE] = handler;
	return *this;
}

Server& Server::patch(const string& path, const function<void(const Request&, const Response&)>& handler) {
	_handlers[path][Method::PATCH] = handler;
	return *this;
}

Server::~Server() {
	pthread_kill(_thread.native_handle(), SIGINT);
	close(_serverSocket);
	delete[] _buffer;
}

void Server::_clearBuffer() {
	for (int i = 0; i < BUFFER_SIZE; i++) {
		_buffer[i] = '\0';
	}
}