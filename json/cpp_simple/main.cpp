#define H_OS_WINDOWS

#include "nlohmann/json.hpp"
#include <evnsq/producer.h>
#include <evpp/event_loop.h>
#include <string>

using json = nlohmann::json;

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

class NsqProducer {
public:
	NsqProducer() {
		WSA wsa;
		evpp::EventLoop loop;
		evnsq::Producer producer(&loop, evnsq::Option());
		producer.SetReadyCallback(std::bind(&NsqProducer::OnReady, this, &loop, &producer));
		producer.ConnectToNSQDs("172.31.118.243:4150");
		loop.Run();
	}
	void Publish(evnsq::Producer* producer) {
		json m1 = GetEmailJson();
		json m2 = m1;
		m2["Id"] = "second";
		std::vector<std::string> messages;
		messages.push_back(m1.dump(2));
		messages.push_back(m2.dump(2));
		producer->MultiPublish("test", messages);
	}

	void OnReady(evpp::EventLoop* loop, evnsq::Producer* producer) {
		//loop->RunEvery(evpp::Duration(0.5), std::bind(&Publish, producer));
		Publish(producer);
	}
	void Send(const std::string& s) {

	}
};

int main(int argc, char* argv[]) {
	NsqProducer producer;
	std::cin.get();
	return 0;
}
