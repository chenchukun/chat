#include "chat/Chat_server.h"
#include "protobuf/chat.pb.h"
#include <iostream>
#include <functional>
#include <string>
using namespace tiny;
using namespace std;

Chat_server::Chat_server(Reactor *r, const Internet_addr &addr)
	: _reactor(r), _server(r, addr, "Chat")
{
	_server.set_connection_callback(bind(&Chat_server::online, this, placeholders::_1));

	_server.set_shutdown_callback(bind(&Chat_server::offline, this, placeholders::_1));

	_server.set_message_callback(std::bind(&Chat_server::message, this, placeholders::_1));

	_server.set_error_callback(std::bind(&Chat_server::error, this, placeholders::_1));

//	_reactor->stop();
}

void Chat_server::online(tiny::Tcp_connection_ptr conn)
{
	std::cout << conn->get_peeraddr()->to_string() << " online" << std::endl;
	_handler.online(conn);
}

void Chat_server::offline(tiny::Tcp_connection_ptr conn)
{
	std::cout << conn->get_peeraddr()->to_string() << " offline" << std::endl;
	_handler.offline(conn);
}

// 消息到达回调
void Chat_server::message(tiny::Tcp_connection_ptr conn)
{
#ifdef DEBUG
//	cout << "Chat_server debug: message" << endl;
#endif
	shared_ptr<chat::Message> msg_ptr = _coder.recv(conn);

	if(msg_ptr != NULL)
	{
		// 处理
		_handler.handler(conn, msg_ptr);
	}
}
// 发送错误
void Chat_server::error(tiny::Tcp_connection_ptr conn)
{
//	std::cout << conn->get_peeraddr()->to_string() << "error" << std::endl;
	_handler.error(conn);
}

