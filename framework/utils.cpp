#include "utils.h"

using namespace std;

vector<string> split(const string& str, const string& delimiter) {
	string copy = str;
	vector<string> result;

	size_t pos = 0;
	while ((pos = copy.find(delimiter)) != string::npos) {
		result.push_back(copy.substr(0, pos));
		copy.erase(0, pos + delimiter.length());
	}

	if (!copy.empty()) {
		result.push_back(copy);
	}

	return result;
}

Method parseMethod(const string& method) {
	if (method == "GET") {
		return GET;
	} else if (method == "POST") {
		return POST;
	} else if (method == "PUT") {
		return PUT;
	} else if (method == "DELETE") {
		return DELETE;
	} else if (method == "PATCH") {
		return PATCH;
	}

	throw invalid_argument("Invalid method: " + method);
}