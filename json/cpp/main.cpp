#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "WSA.h"

#define H_OS_WINDOWS
#include <evnsq/producer.h>
#include <evpp/event_loop.h>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>

json GetEmailJson() {
	json j;
	j["Id"] = "d69d7de3-1219-4fe6-9dbf-89e5866b9c26";
	j["TemplateID"] = "2760F1A1-0DC8-48B5-B9F2-29144E0C5CC9";
	j["KeyValues"] = {};
	j["SendTo"] = "abssdf@rhyta.com";
	j["Subject"] = "Verify your email address";

	json title;
	title["field"] = "{Title}";
	title["value"] = "Mrs";

	json lastname;
	lastname["field"] = "{LastName}";
	lastname["value"] = "Let";

	json url;
	url["field"] = "{VerifyUrl}";
	url["value"] = "http://baidu.com";

	j["KeyValues"] += title;
	j["KeyValues"] += lastname;
	j["KeyValues"] += url;
	return j;
}

int main(int argc, char* argv[]) {
	WSA wsa;
	std::string ip = "172.31.118.243:4150";
	std::string topic = "test";

	evpp::EventLoop loop;
	evnsq::Producer producer(&loop, evnsq::Option());
	producer.ConnectToNSQDs(ip);
	auto function = [&]() {
		while (true) {
			producer.Publish(topic, GetEmailJson().dump());
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	};
	std::thread publisher(function);
	loop.Run();

	std::cout << "Started" << '\n';
	std::cin.get();
	return 0;
}
