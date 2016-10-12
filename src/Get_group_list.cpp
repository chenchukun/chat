#include "chat/Get_group_list.h"
#include <iostream>

void Get_group_list::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Get_group_list debug" << std::endl;
#endif

	get_group_list();
}

void Get_group_list::get_group_list()
{
	std::string src_id = _msg_ptr->src_id();

	chat::Message msg;
	msg.set_msg_type(chat::Message::GET_GROUP_LIST);
	msg.set_dst_id(src_id);

	Redis::Reply reply = _redis.command("SMEMBERS group:list:" + src_id);

	if(reply->type == REDIS_REPLY_ARRAY)
	{

		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			msg.add_group_list(reply->element[i]->str);
		}
	}

	_coder.send(_conn, msg);
}




