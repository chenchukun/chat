#include "chat/Get_group_info.h"
#include <iostream>

void Get_group_info::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Get_group_info debug" << std::endl;
#endif

	get_group_info();
}

void Get_group_info::get_group_info()
{
	std::string src_id = _msg_ptr->src_id();
	// 要获取信息的gid
	std::string user_id = _msg_ptr->user_id();

	chat::Message::Group_info *group_info = get_group_info(user_id);

	// 获取群信息失败
	if(group_info == NULL)
	{
#ifdef DEBUG
//	std::cout << "Get_group_info debug get_user_info error" << std::endl;
#endif
	}
	else
	{
		chat::Message msg;
		msg.set_msg_type(chat::Message::GET_GROUP_INFO);
		msg.set_dst_id(src_id);
		msg.set_user_id(user_id);
		msg.set_allocated_group_info(group_info); 
		_coder.send(_conn, msg);
	}
}



// 根据uid获取其信息
chat::Message::Group_info* Get_group_info::get_group_info(const std::string &gid)
{
	// 群名称
	Redis::Reply reply = _redis.command("HGET group:info:" + gid + " name");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Get_group_info debug hget get_group_info() group:info uname error" << std::endl;
#endif
		return NULL;
	}
	std::string name(reply->str);

	// 创建者
	reply = _redis.command("HGET group:info:" + gid + " own");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Get_group_info debug hget get_group_info() group:inof own error" << std::endl;
#endif
		return NULL;
	}
	std::string own(reply->str);

	// 群成员数
	reply = _redis.command("HGET group:info:" + gid + " nums");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Get_group_info debug hget get_group_info() group:info nums error" << std::endl;
#endif
		return NULL;
	}
	std::string nums(reply->str);

	chat::Message::Group_info *group_info = new chat::Message::Group_info();
	group_info->set_gid(gid);
	group_info->set_name(name);
	group_info->set_own(own);
	group_info->set_nums(stoi(nums));

	return group_info;
}


