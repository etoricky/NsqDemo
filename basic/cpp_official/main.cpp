#define H_OS_WINDOWS

//#include <evnsq/exp.h>
#include <evnsq/consumer.h>
#include <evnsq/producer.h>
#include <evpp/event_loop.h>

#include <chrono>
#include <thread>

#include "getopt.h"

void Publish(evnsq::Producer* producer) {
	static const std::string topic1 = "test";
	static int i = 0;
	std::stringstream ss;
	ss << "a NSQ message, index=" << i++ << " ";
	producer->Publish(topic1, ss.str());
}

void OnReady(evpp::EventLoop* loop, evnsq::Producer* p) {
	loop->RunEvery(evpp::Duration(0.001), std::bind(&Publish, p));
	for (int i = 0; i < 20; i++) {
		Publish(p);
	}
}

int main(int argc, char* argv[]) {
	google::InitGoogleLogging(argv[0]);

	FLAGS_stderrthreshold = 0;

	int opt = 0;
	int option_index = 0;
	const char* optstring = "t:h:";
	static struct option long_options[] = {
		{ "nsqd_tcp_addr", required_argument, NULL, 't' },
	{ "lookupd_http_addr", required_argument, NULL, 'h' },
	{ 0, 0, 0, 0 }
	};

	std::string nsqd_tcp_addr;
	std::string lookupd_http_url;

	nsqd_tcp_addr = "172.31.118.243:4150";
	//lookupd_http_url = "http://172.31.118.243:14561/nodes";

	while ((opt = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1) {
		switch (opt) {
		case 't':
			nsqd_tcp_addr = optarg;
			break;

		case 'h':
			lookupd_http_url = optarg;
			break;

		default:
			printf("error argument [%s]\n", argv[optind]);
			return -1;
		}
	}

	WSADATA wsaData = { 0 };
	if (WSAStartup(0x202, &wsaData) == 0)
	{

		evpp::EventLoop loop;
		evnsq::Producer client(&loop, evnsq::Option());
		client.SetReadyCallback(std::bind(&OnReady, &loop, &client));

		if (!lookupd_http_url.empty()) {
			client.ConnectToLookupds(lookupd_http_url);
		}
		else {
			client.ConnectToNSQDs(nsqd_tcp_addr);
		}

		auto f = [](evpp::EventLoop* l, evnsq::Producer* c) {
			std::this_thread::sleep_for(std::chrono::seconds(2));
			for (;;) {
				if (l->pending_functor_count() > 10000) {
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
				}
				else {
					Publish(c);
				}
			}
		};
		std::thread publish_thread(std::bind(f, &loop, &client));
		loop.Run();

		WSACleanup();
	}
	else
		printf("ERROR: Initialization failure.\n");
	return 0;
}