#include "chat/Get_group_user_list.h"
#include <iostream>

void Get_group_user_list::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
	std::cout << "Get_group_user_list debug" << std::endl;
#endif

	get_group_user_list();
}

void Get_group_user_list::get_group_user_list()
{
	std::string src_id = _msg_ptr->src_id();
	std::string user_id = _msg_ptr->user_id();

	chat::Message msg;
	msg.set_msg_type(chat::Message::GET_GROUP_USER_LIST);
	msg.set_dst_id(src_id);
	msg.set_user_id(user_id);

	Redis::Reply reply = _redis.command("SMEMBERS group:user:" + user_id);
#ifdef DEBUG
//	std::cout << "Get_group_user_list debug get_group_user_list()" << std::endl;
#endif

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
#ifdef DEBUG
//	std::cout << "Get_group_user_list debug nums: " << size << std::endl;
#endif
		for(int i=0; i<size; ++i)
		{
			msg.add_group_user_list(reply->element[i]->str);
		}
	}

	_coder.send(_conn, msg);
}




