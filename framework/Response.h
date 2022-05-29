#ifndef RESPONSE_H
#define RESPONSE_H

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

class Server;

class Response {
public:
	const Response& status(int status) const;
	const Response& header(const std::string& header, const std::string& value) const;

	void send(const std::string& body) const;

	friend class Server;

private:
	Response(std::function<void()> onSend) : _status(200), _sent(false), _onSend(onSend) {}

	mutable std::map<std::string, std::string> _headers;
	mutable std::string _body;
	mutable unsigned int _status;
	mutable bool _sent;
	std::function<void()> _onSend;
};

#endif