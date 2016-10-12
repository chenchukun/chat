#ifndef REGISTER_H
#define REGISTER_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 注册模块类
class Register : public Module
{
public:
	Register():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void error();
	void success(std::string &uid);
	const std::string registering();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
