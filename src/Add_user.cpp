#include "chat/Add_user.h"
#include <iostream>

void Add_user::run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Add_user debug" << std::endl;
#endif

	if(msg_ptr->msg_type() == chat::Message::ADD_REQUEST)
	{
		add_user_request();
	}
	else
	{
		add_user_respond();
	}

}


// 暂存request报文
void Add_user::temporary_request()
{
	std::string key = "add_request:";
	key += _msg_ptr->dst_id() + ":" + std::to_string(time(NULL));

	// 保存是谁请求
	Redis::Reply reply = _redis.command("SET " + key + " " + _msg_ptr->src_id());
}

// 暂存respond报文
void Add_user::temporary_respond()
{
	std::string key = "add_respond:";
	key += _msg_ptr->dst_id() + ":" + std::to_string(time(NULL));

	// 保存是谁回复
	Redis::Reply reply = _redis.command("SET " + key + " " + _msg_ptr->src_id());
}



// 添加好友请求
void Add_user::add_user_request()
{
#ifdef DEBUG
//	std::cout << "Add_user debug add_user_request" << std::endl;
#endif

	if(online_id_to_con.count(_msg_ptr->dst_id()) == 0)
	{
		// 用户不在线
		temporary_request();
#ifdef DEBUG
//		std::cout << "Add_user debug add_user_request user"
//			<< _msg_ptr->dst_id() << " not online" << std::endl;
#endif
	}
	else
	{
		// 转发报文
		_coder.send(online_id_to_con[_msg_ptr->dst_id()], *_msg_ptr);
	}
}


// 添加好友回复
void Add_user::add_user_respond()
{
#ifdef DEBUG
//	std::cout << "Add_user debug add_user_respond" << std::endl;
#endif

	std::string src_id = _msg_ptr->src_id();
	std::string dst_id = _msg_ptr->dst_id();

	// 修改好友列表
	Redis::Reply reply = _redis.command("SADD friend:list:" + src_id + " " + dst_id);
	if(reply->type != REDIS_REPLY_INTEGER || reply->integer != 1)
	{
#ifdef DEBUG
		std::cout << "Add_user debug add_user_respond sadd friend error" << std::endl;
#endif
	}

	// 修改好友列表
	reply = _redis.command("SADD friend:list:" + dst_id + " " + src_id);
	if(reply->type != REDIS_REPLY_INTEGER || reply->integer != 1)
	{
#ifdef DEBUG
		std::cout << "Add_user debug add_user_respond sadd friend error" << std::endl;
#endif
	}

	// 修改好友数
	reply = _redis.command("HINCRBY user:info:" + src_id + " friends 1");
	if(reply->type != REDIS_REPLY_INTEGER)
	{
#ifdef DEBUG
		std::cout << "Add_user debug add_user_respond hincrby user:info friends" << std::endl;
#endif
	}

	// 修改好友数
	reply = _redis.command("HINCRBY user:info:" + dst_id + " friends 1");
	if(reply->type != REDIS_REPLY_INTEGER)
	{
#ifdef DEBUG
		std::cout << "Add_user debug add_user_respond hincrby user:info friends" << std::endl;
#endif
	}

	// 转发报文
	if(online_id_to_con.count(_msg_ptr->dst_id()) == 0)
	{
		// 用户不在线， 暂时存储起来
		temporary_respond();

#ifdef DEBUG
//		std::cout << "Add_user debug add_user_respond user"
//			<< _msg_ptr->dst_id() << "not online" << std::endl;
#endif
	}
	else
	{
		_coder.send(online_id_to_con[_msg_ptr->dst_id()], *_msg_ptr);
	}
}


