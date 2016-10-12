#include "protobuf/chat.pb.h"
#include "reactor/Reactor.h"
#include "chat/Chat_server.h"
#include "protobuf/Coder.h"
#include "redis/Redis.h"
#include <iostream>
#include <string>
using namespace tiny;
using namespace std;

/*
void work(Reactor *reactor)
{
	cout << "press any key to exit." << endl;
	getchar();
	reactor->stop();
}
*/

int main()
{
//	Redis redis("127.0.0.1", 6379);
//	Redis::Reply reply = redis.command("SET name kikuchanj");
	Reactor reactor;

	Chat_server chat(&reactor, Internet_addr("0.0.0.0", 2345));

	chat.start();

//	thread exit_thread = thread(work, &reactor);
	reactor.run();

	return 0;
}
