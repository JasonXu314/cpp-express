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
#include "Response.h"

const int BUFFER_SIZE = 4096;

class Server {
public:
	Server(int port);
	~Server();

	void start();
	void wait();

	Server& get(const std::string& path, const std::function<void(const Request&, const Response&)>& handler);
	Server& post(const std::string& path, const std::function<void(const Request&, const Response&)>& handler);
	Server& put(const std::string& path, const std::function<void(const Request&, const Response&)>& handler);
	Server& del(const std::string& path, const std::function<void(const Request&, const Response&)>& handler);
	Server& patch(const std::string& path, const std::function<void(const Request&, const Response&)>& handler);

private:
	int _port;
	int _serverSocket;
	std::thread _thread;
	std::map<std::string, std::map<Method, std::function<void(const Request&, const Response&)>>> _handlers;
	char* _buffer;

	void _clearBuffer();
};

#endif