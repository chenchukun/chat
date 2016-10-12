#ifndef LOGIN_H
#define LOGIN_H
#include "chat/Module.h"
#include "redis/Redis.h"
#include "protobuf/Coder.h"

// 登录模块类
class Login : public Module
{
public:
	Login():_redis("127.0.0.1", 6379)
	{
	}
	void run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr);

private:
	chat::Message_STATUS login();
	void error(chat::Message_STATUS status);
	void success();
	chat::Message::User_info* get_user_info(const std::string &uid);

	void offline_info();
	void offline_add_request();
	void offline_add_respond();
	void offline_single_msg();
	void offline_invite_request();
	void offline_invite_respond();
	void offline_group_msg();

	tiny::Tcp_connection_ptr _conn;
	std::shared_ptr<chat::Message> _msg_ptr;
	Coder _coder;
	Redis _redis;
};


#endif
