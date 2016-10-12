#ifndef GET_FRIEND_LIST_H
#define GET_FRIEND_LIST_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 获取好友列表模块类
class Get_friend_list : public Module
{
public:
	Get_friend_list():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void get_friend_list();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
