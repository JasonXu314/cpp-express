#include "Response.h"

using namespace std;

const Response& Response::status(int status) const {
	if (_sent) {
		throw runtime_error("Status cannot be set after response has been sent");
	}

	_status = status;
	return *this;
}

const Response& Response::header(const string& header, const string& value) const {
	if (_sent) {
		throw runtime_error("Headers cannot be set after the response has been sent");
	}

	_headers[header] = value;
	return *this;
}

void Response::send(const string& body) const {
	if (_sent) {
		throw runtime_error("Response already sent");
	}

	_sent = true;
	_body = body;

	_onSend();
}