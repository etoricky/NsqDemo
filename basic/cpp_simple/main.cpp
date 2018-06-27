#define H_OS_WINDOWS

#include "WSA.h"
#include <evnsq/producer.h>
#include <evpp/event_loop.h>
#include <string>

void Publish(evnsq::Producer* producer) {
	time_t now = time(0);
	char timestamp[10] = "";
	strftime(timestamp, 10, "%H:%M:%S", localtime(&now));
	producer->Publish("test", std::string(timestamp) + " Hello");
}

void OnReady(evpp::EventLoop* loop, evnsq::Producer* producer) {
	//loop->RunEvery(evpp::Duration(0.5), std::bind(&Publish, producer));
	Publish(producer);
	producer->Close();
	loop->Stop();
}

int main(int argc, char* argv[]) {
	WSA wsa;
	evpp::EventLoop loop;
	evnsq::Producer producer(&loop, evnsq::Option());
	producer.SetReadyCallback(std::bind(&OnReady, &loop, &producer));
	producer.ConnectToNSQDs("172.31.118.243:4150");
	loop.Run();
	std::cin.get();
	return 0;
}
