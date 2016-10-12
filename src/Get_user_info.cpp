#include "chat/Get_user_info.h"
#include <iostream>

void Get_user_info::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Get_user_info debug" << std::endl;
#endif

	get_user_info();
}

void Get_user_info::get_user_info()
{
	std::string src_id = _msg_ptr->src_id();
	// 要获取信息的uid
	std::string user_id = _msg_ptr->user_id();

	chat::Message::User_info *user_info = get_user_info(user_id);

	// 获取好友信息失败
	if(user_info == NULL)
	{
#ifdef DEBUG
//	std::cout << "Get_user_info debug get_user_info error" << std::endl;
#endif
	}
	else
	{
		chat::Message msg;
		msg.set_msg_type(chat::Message::GET_USER_INFO);
		msg.set_dst_id(src_id);
		msg.set_user_id(user_id);
		msg.set_allocated_user_info(user_info); 
		_coder.send(_conn, msg);
	}
}



// 根据uid获取其信息
chat::Message::User_info* Get_user_info::get_user_info(const std::string &uid)
{
	// 用户名
	Redis::Reply reply = _redis.command("HGET user:info:" + uid + " uname");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Get_user_info debug hget get_user_info() user:info uname error" << std::endl;
#endif
		return NULL;
	}
	std::string uname(reply->str);

	// 地址
	reply = _redis.command("HGET user:info:" + uid + " area");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Get_user_info debug hget get_user_info(user:inof area error" << std::endl;
#endif
		return NULL;
	}
	std::string area(reply->str);

	// 好友数
	reply = _redis.command("HGET user:info:" + uid + " friends");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Get_user_info debug hget get_user_info(user:info friends error" << std::endl;
#endif
		return NULL;
	}
	std::string friends(reply->str);

	chat::Message::User_info *user_info = new chat::Message::User_info();
	user_info->set_uid(uid);
	user_info->set_uname(uname);
	user_info->set_area(area);
	user_info->set_friends(stoi(friends));

	return user_info;
}
