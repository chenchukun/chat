#ifndef ADD_USER_H
#define ADD_USER_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 添加好友模块类
class Add_user : public Module
{
public:
	Add_user():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void add_user_request();
	void add_user_respond();
	void temporary_request();
	void temporary_respond();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
