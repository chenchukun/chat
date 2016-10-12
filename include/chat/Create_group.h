#ifndef CREATE_GROUP_H
#define CREATE_GROUP_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 创建群模块类
class Create_group : public Module
{
public:
	Create_group():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void create_group();
	std::string get_gid();
	void set_group_list(std::string &gid);
	void set_group_info(std::string &gid);
	void success(std::string &gid);

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
