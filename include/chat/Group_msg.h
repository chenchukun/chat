#ifndef GROUP_MSG_H
#define GROUP_MSG_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 一对一聊天模块类
class Group_msg : public Module
{
public:
	Group_msg():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void group_msg();
	void record();
	void temporary(std::string &uid);

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
