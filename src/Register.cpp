#include "chat/Register.h"
#include <iostream>

void Register::run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Register debug" << std::endl;
#endif

	std::string uid = registering();
	if(uid.empty())
	{
		error();
	}
	else
	{
		success(uid);
	}

#ifdef DEBUG
//	std::cout << "Register debug register success" << std::endl;
#endif
}



void Register::error()
{
#ifdef DEBUG
	std::cout << "Register debug: error" << std::endl;
#endif
	chat::Message msg;
	msg.set_msg_type(chat::Message::REGISTER);
	msg.set_status(chat::Message::ERROR);
	
	_coder.send(_conn, msg);
}



void Register::success(std::string &uid)
{
	chat::Message msg;
	msg.set_msg_type(chat::Message::REGISTER);
	msg.set_status(chat::Message::SUCCESS);
	msg.set_dst_id(uid);

	chat::Message::User_info *user_info = new chat::Message::User_info();
	user_info->set_uid(uid);
	user_info->set_uname(_msg_ptr->user_name());
	user_info->set_area("未知");
	user_info->set_friends(0);
	msg.set_allocated_user_info(user_info);  // 会不会造成内存泄漏？
	
	_coder.send(_conn, msg);
}


// 操作数据库，成功返回UID
const std::string Register::registering()
{
	std::string uname = _msg_ptr->user_name();
	std::string password = _msg_ptr->password();

	// 获取要分配给用户的id
	Redis::Reply respond = _redis.command("GET id:max");
	if(respond->type != REDIS_REPLY_STRING)
	{
#ifdef DEBUG
	std::cout << "Register debug get id:max error" << std::endl;
#endif
		return std::string();
	}

	std::string uid = respond->str;
#ifdef DEBUG
//	std::cout << "Register debug uid: " << uid << std::endl;
#endif

	// 更新系统可用id的值
	respond = _redis.command("INCR id:max");
	if(respond->type != REDIS_REPLY_INTEGER)
	{
#ifdef DEBUG
	std::cout << "Register debug incr id:max error" << std::endl;
#endif
//		error();		
		return std::string();
	}

	// 将分配给用户的id添加到用户列表中
	respond = _redis.command("SADD user:list " + uid);
	if(respond->type != REDIS_REPLY_INTEGER || respond->integer == 0)
	{
#ifdef DEBUG
	std::cout << "Register debug sadd user:list error" << std::endl;
#endif
//		error();		
		return std::string();
	}

	respond = _redis.command("hset user:info:"+uid + " uname " + uname);
	if(respond->type != REDIS_REPLY_INTEGER || respond->integer == 0)
	{
		return std::string();
	}

	respond = _redis.command("hset user:info:"+uid + " area " + "未知");
	if(respond->type != REDIS_REPLY_INTEGER || respond->integer == 0)
	{
		return std::string();
	}

	respond = _redis.command("hset user:info:"+uid + " friends " + "0");
	if(respond->type != REDIS_REPLY_INTEGER || respond->integer == 0)
	{
		return std::string();
	}

	respond = _redis.command("hset user:passwd " + uid + " " + password);
	if(respond->type != REDIS_REPLY_INTEGER || respond->integer == 0)
	{
		return std::string();
	}

#ifdef DEBUG
//	std::cout << "Register debug update redis success" << std::endl;
#endif
	return uid;
}
