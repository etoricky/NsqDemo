#include <string>
#include "include/curl/curl.h"
int main(void) {
	CURLcode res = CURLE_OK;
	CURL *curl = curl_easy_init();
	if (curl) {
		std::string data("Data from libcurl");
		curl_easy_setopt(curl, CURLOPT_URL, "http://172.31.118.243:4151/pub?topic=test");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return res;
}