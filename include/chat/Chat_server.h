#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include "reactor/Reactor.h"
#include "reactor/Internet_addr.h"
#include "reactor/Tcp_server.h"
#include "protobuf/Coder.h"
#include "chat/Handler.h"

class Chat_server
{
public:
	Chat_server(tiny::Reactor *r, const tiny::Internet_addr &addr);

	void start()
	{
		_server.start();
	}

private:
	// 用户上线回调
	void online(tiny::Tcp_connection_ptr conn);
	// 用户下线回调
	void offline(tiny::Tcp_connection_ptr conn); 
	// 消息到达回调
	void message(tiny::Tcp_connection_ptr conn);
	// 发送错误
	void error(tiny::Tcp_connection_ptr conn);

	tiny::Reactor *_reactor;
	tiny::Tcp_server _server;
	
	Coder _coder;
	Handler _handler;
};

#endif
