#ifndef GET_GROUP_INFO_H
#define GET_GROUP_INFO_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 获取用户信息模块类
class Get_group_info : public Module
{
public:
	Get_group_info():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void get_group_info();
	chat::Message::Group_info* get_group_info(const std::string &uid);

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
