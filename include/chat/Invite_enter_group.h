#ifndef INVITE_ENTER_GROUP_H
#define INVITE_ENTER_GROUP_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 邀请加入群模块类
class Invite_enter_group : public Module
{
public:
	Invite_enter_group():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void request();
	void respond();
	void temporary_request();
	void temporary_respond();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
