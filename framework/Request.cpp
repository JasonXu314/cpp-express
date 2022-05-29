#include "Request.h"

using namespace std;

Request::Request(const string& raw) {
	vector<string> lines = split(raw, raw.find("\r") != string::npos ? "\r\n" : "\n");
	vector<string> firstLine = split(lines[0], " ");

	_method = parseMethod(firstLine[0]);

	_path = firstLine[1];

	if (_path.find("?") != string::npos) {
		vector<string> fullPath = split(_path, "?");
		_path = fullPath[0];

		vector<string> queryPairs = split(fullPath[1], "&");
		for (string queryPair : queryPairs) {
			vector<string> keyValue = split(queryPair, "=");
			_query[keyValue[0]] = keyValue[1];
		}
	}

	bool inBody = false;
	for (size_t i = 1; i < lines.size(); i++) {
		if (!inBody) {
			if (lines[i] == "") {
				inBody = true;
				continue;
			}

			vector<string> header = split(lines[i], ": ");
			_headers[header[0]] = header[1];
		} else {
			_body += lines[i];

			if (i != lines.size() - 1) {
				_body += "\n";
			}
		}
	}
}
