#ifndef GET_CHAT_RECORD_H
#define GET_CHAT_RECORD_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 获取聊天记录模块类
class Get_chat_record : public Module
{
public:
	Get_chat_record():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	void create_record_list(std::string key);
	std::string get_key();
	void get_chat_record();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};

#endif
