#include "nlohmann/json.hpp"
using json = nlohmann::json;

#define H_OS_WINDOWS
#include <evnsq/producer.h>
#include <evpp/event_loop.h>

#include <string>



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

struct WSA {
	WSA() {
		WSADATA wsaData = { 0 };
		if (WSAStartup(0x202, &wsaData) != 0) {
			throw std::runtime_error("WSAStartup() failed");
		}
	}
	~WSA() {
		try {
			WSACleanup();
		}
		catch (...) {}
	}
};

class NsqSender {
private:
	WSA wsa;
	std::string ip;
	std::string topic;
	std::string data;
public:
	NsqSender(const std::string& ip, const std::string& topic, const std::string& data)
		: ip(ip), topic(topic), data(data) {
		evpp::EventLoop loop;
		evnsq::Producer producer(&loop, evnsq::Option());
		producer.SetReadyCallback(std::bind(&NsqSender::OnReady, this, &loop, &producer));
		producer.ConnectToNSQDs(ip);
		loop.Run();
	}
	void OnReady(evpp::EventLoop* loop, evnsq::Producer* producer) {
		producer->Publish(topic, data);
		producer->Close();
		loop->Stop();
	}
};

int main(int argc, char* argv[]) {
	NsqSender sender("172.31.118.243:4150", "test", GetEmailJson().dump());
	std::cout << "Started" << '\n';
	std::cin.get();
	return 0;
}
