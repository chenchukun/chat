#include "chat/Group_msg.h"
#include <iostream>

void Group_msg::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Group_msg debug" << std::endl;
#endif
	
	group_msg();
}


void Group_msg::temporary(std::string &uid)
{
	std::string src_id = _msg_ptr->src_id();
	std::string dst_id = _msg_ptr->dst_id();
	std::string time = std::to_string(_msg_ptr->msg_text().time());

	std::string key = "group_msg:" + uid + ":" +time;

	std::string record_key = "record_group:" + src_id + ":" + dst_id + ":" + time;

	_redis.command("SET " + key + " " + record_key);
}


void Group_msg::group_msg()
{
	// 目前群ID
	std::string dst_id = _msg_ptr->dst_id();

	// 保存聊天信息
	record();

	// 获取群成员列表
	Redis::Reply reply = _redis.command("SMEMBERS group:user:" + dst_id);

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;

		for(int i=0; i<size; ++i)
		{
			std::string uid = reply->element[i]->str;

			if(online_id_to_con.count(uid) == 0)
			{
				// 用户不在线, 利用聊天记录来保存
				temporary(uid);
			}
			else
			{
				_coder.send(online_id_to_con[uid], *_msg_ptr);
			}
		}
	}
}

void Group_msg::record()
{
	std::string src_id = _msg_ptr->src_id();
	std::string dst_id = _msg_ptr->dst_id();
	std::string time = std::to_string(_msg_ptr->msg_text().time());
	std::string msg = _msg_ptr->msg_text().msg();
//	std::cout << msg << std::endl;
	
	std::string record_key = "record_group:" + src_id + ":" + dst_id + ":" + time;
	Redis::Reply reply = _redis.command("hset " + record_key + " src_id " + src_id);
	reply = _redis.command("hset " + record_key + " dst_id " + dst_id);
	reply = _redis.command("hset " + record_key + " time " + time);

	std::string cmd = "hset " + record_key + " msg " + msg;
//	std::string cmd = "hset " + record_key + " msg \"hello,kikuchanj.\"";
	reply = _redis.command(cmd);
//	std::cout << cmd << std::endl;

	if(reply->type != REDIS_REPLY_INTEGER || reply->integer!=1)
	{
#ifdef DEBUG
		std::cout << "hste msg error" << std::endl;
		std::cout << reply->type << std::endl;
		std::cout << reply->integer << std::endl;
		std::cout << reply->str << std::endl;
#endif
	}

}






