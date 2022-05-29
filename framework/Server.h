#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <functional>
#include <sstream>
#include <thread>
#include <vector>

#include "Request.h"

const int BUFFER_SIZE = 4096;

class Server {
public:
	Server(int port);
	~Server();

	void start();
	void wait();

	Server& get(const std::string& path, const std::function<std::string(const Request&)>& handler) {
		_handlers[path][Method::GET] = handler;
		return *this;
	}
	Server& post(const std::string& path, const std::function<std::string(const Request&)>& handler) {
		_handlers[path][Method::POST] = handler;
		return *this;
	}
	Server& put(const std::string& path, const std::function<std::string(const Request&)>& handler) {
		_handlers[path][Method::PUT] = handler;
		return *this;
	}
	Server& del(const std::string& path, const std::function<std::string(const Request&)>& handler) {
		_handlers[path][Method::DELETE] = handler;
		return *this;
	}
	Server& patch(const std::string& path, const std::function<std::string(const Request&)>& handler) {
		_handlers[path][Method::PATCH] = handler;
		return *this;
	}

private:
	int _port;
	int _serverSocket;
	std::thread _thread;
	std::map<std::string, std::map<Method, std::function<std::string(const Request&)>>> _handlers;
	char* _buffer;

	void _clearBuffer();
};

#endif