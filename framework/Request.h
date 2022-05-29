#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>
#include <map>
#include <string>

#include "utils.h"

class Request {
public:
	Request(const std::string& raw);
	Request(const Request& other) : _headers(other._headers), _query(other._query), _method(other._method), _path(other._path), _body(other._body) {}

	Method method() const { return _method; }
	std::string path() const { return _path; }
	std::string body() const { return _body; }
	std::map<std::string, std::string> headers() const { return _headers; }
	std::map<std::string, std::string> query() const { return _query; }

	std::string header(const std::string& header) const { return _headers.at(header); }
	std::string query(const std::string& key) const { return _query.at(key); }

private:
	std::map<std::string, std::string> _headers;
	std::map<std::string, std::string> _query;

	Method _method;
	std::string _path;
	std::string _body;
};

#endif