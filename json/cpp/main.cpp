#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "WSA.h"
#include "LockingQueue.h"

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

class NsqSender {
private:	
	evpp::EventLoop loop;
	evnsq::Producer producer;
	LockingQueue<std::string> items;
	std::thread thread;
	std::thread event;
	std::atomic<bool> terminate = false;
public:
	NsqSender(): producer(&loop, evnsq::Option()) {
	}
	~NsqSender() {
		producer.Close();
		terminate = true;
		if (thread.joinable()) {
			thread.join();
		}
		loop.Stop();
		if (event.joinable()) {
			event.join();
		}
	}
	void Connect(const std::string& ip, const std::string& topic) {
		producer.ConnectToNSQDs(ip);
		thread = std::thread([&, topic = topic]() {
			std::string item;
			while (!terminate) {
				bool ok = items.try_pop_for_ms(item, 1000);
				if (ok) {
					bool published = false;
					do {
						published = producer.Publish(topic, item);
					} while (!terminate && !published);
				}
			}
		});
		event = std::thread([&]() {loop.Run(); });
	}
	void Push(const std::string& item) {
		items.push(item);
	}
};

int main(int argc, char* argv[]) {
	WSA wsa;
	std::string ip = "172.31.118.243:4150";
	std::string topic = "test";
	{
		NsqSender sender;
		sender.Connect(ip, topic);
		for (int i = 0; i<10; ++i) {
			sender.Push(GetEmailJson().dump(i));
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
	std::cout << "Done" << '\n';
	std::cin.get();
	return 0;
}
