#include "chat/Get_chat_record.h"
#include <iostream>

void Get_chat_record::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Get_chat_record debug" << std::endl;
#endif
	
	get_chat_record();
}


void Get_chat_record::get_chat_record()
{
	// 建立相关聊天记录有序集合
	std::string key = get_key();
	// 设置存活时间
	_redis.command("EXPIRE " + key + " 120");

	std::string src_id = _msg_ptr->src_id();
	std::string user_id = _msg_ptr->user_id();
	int page = _msg_ptr->page();

	Redis::Reply reply = _redis.command("ZCARD " + key);
	int size = reply->integer;

	chat::Message msg;
	msg.set_msg_type(chat::Message::GET_CHAT_RECORD);
	msg.set_dst_id(src_id);
	msg.set_user_id(user_id);
	msg.set_page(page);

	if((page-1)*10 < size)
	{
		int start = (page-1)*10;
		int end = start + 9;
		reply = _redis.command("ZRANGE " + key + " " + std::to_string(start) 
				+ " " + std::to_string(end));

		// 聊天记录条数
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			std::string rkey(reply->element[i]->str);
	//		std::cout << rkey << std::endl;

			chat::Message_Text *chat_record = msg.add_chat_record();
			Redis::Reply ret = _redis.command("HGET " + rkey + " src_id");
			chat_record->set_src_id(ret->str);

			ret = _redis.command("HGET " + rkey + " dst_id");
			chat_record->set_dst_id(ret->str);

			ret = _redis.command("HGET " + rkey + " time");
			chat_record->set_time(std::stoi(ret->str));

			ret = _redis.command("HGET " + rkey + " msg");
			chat_record->set_msg(ret->str);
		}
	}

	_coder.send(_conn, msg);

}


std::string Get_chat_record::get_key()
{
	std::string src_id = _msg_ptr->src_id();
	std::string user_id = _msg_ptr->user_id();

	std::string key1 = "record:sset:" + src_id + ":" + user_id;
	std::string key2 = "record:sset:" + user_id + ":" + src_id;
	
	Redis::Reply reply = _redis.command("EXISTS " + key1);
	if(reply->type==REDIS_REPLY_INTEGER && reply->integer==1)
	{
		return key1;
	}
	reply = _redis.command("EXISTS " + key2);
	if(reply->type==REDIS_REPLY_INTEGER && reply->integer==1)
	{
		return key2;
	}
	
	create_record_list(key1);
	return key1;
}


void Get_chat_record::create_record_list(std::string key)
{
	std::string src_id = _msg_ptr->src_id();
	std::string user_id = _msg_ptr->user_id();

	Redis::Reply reply = _redis.command("KEYS record:" + src_id + ":" + user_id +":*");
	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			const char *hkey = reply->element[i]->str;
			const char *time = "0";
			Redis::Reply ret = _redis.command(std::string() + "HGET " + hkey + " time");
			if(ret->type == REDIS_REPLY_STRING)
			{
				time = ret->str;
			}
			else
			{
				// 获取失败
			}
			ret = _redis.command("ZADD " + key + " " + time + " " + hkey);
			if(ret->type != REDIS_REPLY_INTEGER || ret->integer!=1)
			{
				// 添加失败
			}
		}
	}
	reply = _redis.command("KEYS record:" + user_id + ":" + src_id +":*");
	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			const char *hkey = reply->element[i]->str;
			const char *time = "0";
			Redis::Reply ret = _redis.command(std::string() + "HGET " + hkey + " time");
			if(ret->type == REDIS_REPLY_STRING)
			{
				time = ret->str;
			}
			else
			{
				// 获取失败
			}
			ret = _redis.command("ZADD " + key + " " + time + " " + hkey);
			if(ret->type != REDIS_REPLY_INTEGER || ret->integer!=1)
			{
				// 添加失败
			}
		}
	}
}




