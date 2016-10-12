#ifndef GET_GROUP_USER_LIST_H
#define GET_GROUP_USER_LIST_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 获取群成员列表模块类
class Get_group_user_list : public Module
{
public:
	Get_group_user_list():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void get_group_user_list();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
