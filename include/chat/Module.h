#ifndef MODULE_H
#define MODULE_H
#include "protobuf/chat.pb.h"
#include "reactor/Tcp_connection.h"
#include <memory>

typedef std::map<std::string, tiny::Tcp_connection_ptr> Online_id_to_con;

extern Online_id_to_con online_id_to_con;    // 用户ID与Tcp_connection_ptr的映射

// 功能模板抽象类
class Module
{
public:
	Module() = default;
	virtual ~Module()
	{
#ifdef DEBUG
//		std::cout << "Module debug ~Module()" << std::endl;
#endif
	}
	virtual void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr) = 0;
};

#endif
