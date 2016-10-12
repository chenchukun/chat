#include "chat/Login.h"
#include <iostream>

void Login::run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Login debug" << std::endl;
#endif

	chat::Message_STATUS status = login();

	if(status != chat::Message::SUCCESS)
	{
		error(status);
	}
	else
	{
		success();
	}
}


void Login::error(chat::Message_STATUS status)
{
	chat::Message msg;
	msg.set_msg_type(chat::Message::LOGIN);
	msg.set_status(status);

	_coder.send(_conn, msg);
}


chat::Message::User_info* Login::get_user_info(const std::string &uid)
{
	// 用户名
	Redis::Reply reply = _redis.command("HGET user:info:" + uid + " uname");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Login debug hget get_user_info() user:info uname error" << std::endl;
#endif
		return NULL;
	}
	std::string uname(reply->str);

	// 地址
	reply = _redis.command("HGET user:info:" + uid + " area");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Login debug hget get_user_info(user:inof area error" << std::endl;
#endif
		return NULL;
	}
	std::string area(reply->str);

	// 好友数
	reply = _redis.command("HGET user:info:" + uid + " friends");
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Login debug hget get_user_info(user:info friends error" << std::endl;
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


void Login::success()
{
	chat::Message msg;
	msg.set_msg_type(chat::Message::LOGIN);
	msg.set_status(chat::Message::SUCCESS);
	msg.set_dst_id(_msg_ptr->src_id());

	chat::Message::User_info *user_info = get_user_info(_msg_ptr->src_id());
	if(user_info == NULL)
	{
#ifdef DEBUG
	std::cout << "Login debug success() get_user_info error" << std::endl;
#endif
		error(chat::Message::ERROR);
	}

	msg.set_allocated_user_info(user_info); 

	// 添加到在线列表
	online_id_to_con[_msg_ptr->src_id()] = _conn;

	_coder.send(_conn, msg);

	// 处理离线信息
	offline_info();
}



chat::Message_STATUS Login::login()
{
	std::string uid = _msg_ptr->src_id();
	std::string password = _msg_ptr->password();

	Redis::Reply reply = _redis.command("SISMEMBER user:list " + uid);
	if(reply->integer == 0)
	{
		return chat::Message::UID_NOEXISTS;
	}
	
	reply = _redis.command("HGET user:passwd " + uid);
	// 数据错误
	if(reply->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Login debug login() hgt user:passwd error" << std::endl;
#endif
		return chat::Message::ERROR;
	}

	// 密码错误
	if(password != std::string(reply->str))
	{
		return chat::Message::PASSWORD_ERROR;
	}

	return chat::Message::SUCCESS;
}





// 离线好友请求信息
void Login::offline_add_request()
{
	std::string src_id = _msg_ptr->src_id();
	Redis::Reply reply = _redis.command("KEYS add_request:" + src_id + ":*");

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			std::string akey = reply->element[i]->str;
			Redis::Reply ret = _redis.command("GET " + akey);			

			std::string uid = ret->str;
			chat::Message msg;
			msg.set_msg_type(chat::Message::ADD_REQUEST);
			msg.set_src_id(uid);
			msg.set_dst_id(src_id);

			_coder.send(_conn, msg);

			_redis.command("DEL " + akey);
		}
	}
}




// 离线好友回复信息
void Login::offline_add_respond()
{
	std::string src_id = _msg_ptr->src_id();
	Redis::Reply reply = _redis.command("KEYS add_respond:" + src_id + ":*");

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			std::string akey = reply->element[i]->str;
			Redis::Reply ret = _redis.command("GET " + akey);			

			std::string uid = ret->str;
			chat::Message msg;
			msg.set_msg_type(chat::Message::ADD_RESPOND);
			msg.set_src_id(uid);
			msg.set_dst_id(src_id);

			_coder.send(_conn, msg);

			_redis.command("DEL " + akey);
		}
	}
}


// 离线群消息
void Login::offline_group_msg()
{
	std::string src_id = _msg_ptr->src_id();
	// 获取所有离线群消息的key
	Redis::Reply reply = _redis.command("KEYS group_msg:" + src_id + ":*");

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		// key的数目
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			// 离线群消息key
			std::string rkey = reply->element[i]->str;
			Redis::Reply ret = _redis.command("GET " + rkey);			

			chat::Message msg;
			msg.set_msg_type(chat::Message::GROUP_MSG);
			chat::Message::Text *msg_text = new chat::Message::Text();

			// 聊天记录key
			std::string record_key = ret->str;
			ret = _redis.command("HGET " + record_key + " src_id");
			msg.set_src_id(ret->str);
			msg_text->set_src_id(ret->str);

			ret = _redis.command("HGET " + record_key + " dst_id");
			msg.set_dst_id(ret->str);
			msg_text->set_dst_id(ret->str);

			ret = _redis.command("HGET " + record_key + " time");
			msg_text->set_time(std::stoi(ret->str));

			ret = _redis.command("HGET " + record_key + " msg");
			msg_text->set_msg(ret->str);

			msg.set_allocated_msg_text(msg_text);

			_coder.send(_conn, msg);

			_redis.command("DEL " + rkey);
		}
	}
}


// 离线聊天信息
void Login::offline_single_msg()
{
	std::string src_id = _msg_ptr->src_id();
	// 获取所有离线消息的key
	Redis::Reply reply = _redis.command("KEYS single_msg:" + src_id + ":*");

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		// key的数目
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			// 离线消息key
			std::string rkey = reply->element[i]->str;
			Redis::Reply ret = _redis.command("GET " + rkey);			

			chat::Message msg;
			msg.set_msg_type(chat::Message::SINGLE_MSG);
			chat::Message::Text *msg_text = new chat::Message::Text();

			// 聊天记录key
			std::string record_key = ret->str;
			ret = _redis.command("HGET " + record_key + " src_id");
			msg.set_src_id(ret->str);
			msg_text->set_src_id(ret->str);

			ret = _redis.command("HGET " + record_key + " dst_id");
			msg.set_dst_id(ret->str);
			msg_text->set_dst_id(ret->str);

			ret = _redis.command("HGET " + record_key + " time");
			msg_text->set_time(std::stoi(ret->str));

			ret = _redis.command("HGET " + record_key + " msg");
			msg_text->set_msg(ret->str);

			msg.set_allocated_msg_text(msg_text);

			_coder.send(_conn, msg);

			_redis.command("DEL " + rkey);
		}
	}
}

// 离线邀请请求
void Login::offline_invite_request()
{
	std::string src_id = _msg_ptr->src_id();
	Redis::Reply reply = _redis.command("KEYS invite:" + src_id + ":*");

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			std::string akey = reply->element[i]->str;
			Redis::Reply ret = _redis.command("HGET " + akey + " src_id");
			std::string uid = ret->str;

			ret = _redis.command("HGET " + akey + " user_id");
			std::string gid = ret->str;

			chat::Message msg;
			msg.set_msg_type(chat::Message::INVITE_ENTER_GROUP);
			msg.set_src_id(uid);
			msg.set_dst_id(src_id);
			msg.set_user_id(gid);

			_coder.send(_conn, msg);

			_redis.command("DEL " + akey);
		}
	}
}

// 离线邀请回复
void Login::offline_invite_respond()
{
	std::string src_id = _msg_ptr->src_id();
	Redis::Reply reply = _redis.command("KEYS invite_respond:" + src_id + ":*");

	if(reply->type == REDIS_REPLY_ARRAY)
	{
		int size = reply->elements;
		for(int i=0; i<size; ++i)
		{
			std::string akey = reply->element[i]->str;
			Redis::Reply ret = _redis.command("HGET " + akey + " src_id");
			std::string uid = ret->str;

			ret = _redis.command("HGET " + akey + " user_id");
			std::string gid = ret->str;

			chat::Message msg;
			msg.set_msg_type(chat::Message::INVITE_ENTER_GROUP);
			msg.set_src_id(uid);
			msg.set_dst_id(src_id);
			msg.set_user_id(gid);

			_coder.send(_conn, msg);

			_redis.command("DEL " + akey);
		}
	}
}


// 处理离线信息
void Login::offline_info()
{
	offline_add_request();
	offline_add_respond();
	offline_single_msg();
	offline_invite_request();
	offline_invite_respond();
	offline_group_msg();
}




