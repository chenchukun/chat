#ifndef HANDLER_H
#define HANDLER_H

#include "protobuf/chat.pb.h"
#include "reactor/Tcp_connection.h"
#include "reactor/Thread_pool.h"
#include "chat/Module_factory.h"
#include <memory>
#include <map>
#include <string>
typedef std::map<std::string, tiny::Tcp_connection_ptr> Online_id_to_con;

extern Online_id_to_con online_id_to_con;    // 用户ID与Tcp_connection_ptr的映射

//  业务处理类，Chat_server接受到报文后解析为Message对象，传递给该类进行处理
class Handler
{
public:
	Handler(): _thread_num(4), _thread_pool(_thread_num)
	{
	}
	~Handler(){}
	void handler(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);
	void online(tiny::Tcp_connection_ptr conn);
	void offline(tiny::Tcp_connection_ptr conn);
	void error(tiny::Tcp_connection_ptr conn);

private:
	const int _thread_num = 4;
	tiny::Thread_pool _thread_pool; // 线程池
	Module_factory _module_factory; // 功能模板工厂
};

#endif
