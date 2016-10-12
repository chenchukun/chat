#include "chat/Handler.h"
#include <functional>
#include <iostream>
#include <mutex>

Online_id_to_con online_id_to_con;    // 用户ID与Tcp_connection_ptr的映射


void Handler::handler(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr)
{
#ifdef DEBUG
//	std::cout << "Handler debug handler msg_type: " << msg_ptr->msg_type() << std::endl;
#endif
	auto module = _module_factory.create(msg_ptr->msg_type());	
	if(module != NULL)
	{
		_thread_pool.run(std::bind(&Module::run, module, conn, msg_ptr));
		//	module->run(conn, msg_ptr);
#ifdef DEBUG
//	std::cout << "Handler debug handler handler success" << std::endl;
#endif

	}
}

void Handler::online(tiny::Tcp_connection_ptr conn)
{
}


// 用户下线, 这种方法效率太低
void Handler::offline(tiny::Tcp_connection_ptr conn)
{
#ifdef DEBUG
//	std::cout << "Handler debug offline" << std::endl;
#endif
	auto begin = online_id_to_con.begin();
	auto end = online_id_to_con.end();
	while(begin != end)
	{
		if(begin->second == conn)
		{
			online_id_to_con.erase(begin);
			break;
		}
		++begin;
	}
}

void Handler::error(tiny::Tcp_connection_ptr conn)
{
	auto begin = online_id_to_con.begin();
	auto end = online_id_to_con.end();
	while(begin != end)
	{
		if(begin->second == conn)
		{
			online_id_to_con.erase(begin);
			break;
		}
		++begin;
	}
}
