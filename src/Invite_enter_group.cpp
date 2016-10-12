#include "chat/Invite_enter_group.h"
#include <iostream>

void Invite_enter_group::run(tiny::Tcp_connection_ptr conn, std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Invite_enter_group debug" << std::endl;
#endif

	if(msg_ptr->msg_type() == chat::Message::INVITE_ENTER_GROUP)
	{
		request();
	}
	else
	{
		respond();
	}
}



// 暂存响应
void Invite_enter_group::temporary_respond()
{
	std::string src_id = _msg_ptr->src_id();
	std::string dst_id = _msg_ptr->dst_id();
	std::string user_id = _msg_ptr->user_id();
	std::string stime = std::to_string(time(NULL));

	// 谁响应 , key名保存了响应谁
	Redis::Reply reply = _redis.command("HSET invite_respond:" + dst_id + ":" + stime + " src_id " + src_id);

	// 响应加入什么群
	reply = _redis.command("HSET invite_respond:" + dst_id + ":" + stime + " user_id " + user_id);
}



// 暂存请求
void Invite_enter_group::temporary_request()
{
	std::string src_id = _msg_ptr->src_id();
	std::string dst_id = _msg_ptr->dst_id();
	std::string user_id = _msg_ptr->user_id();
	std::string stime = std::to_string(time(NULL));

	// 谁邀请 , key名保存了邀请谁
	Redis::Reply reply = _redis.command("HSET invite:" + dst_id + ":" + stime + " src_id " + src_id);

	// 邀请加入什么群
	reply = _redis.command("HSET invite:" + dst_id + ":" + stime + " user_id " + user_id);
}



// 请求
void Invite_enter_group::request()
{
	if(online_id_to_con.count(_msg_ptr->dst_id()) == 0)
	{
		temporary_request();
	}
	else
	{
		_coder.send(online_id_to_con[_msg_ptr->dst_id()], *_msg_ptr);
	}
}



// 响应
void Invite_enter_group::respond()
{
	std::string src_id = _msg_ptr->src_id();
	std::string user_id = _msg_ptr->user_id(); // 群ID

	// 将用户添加到群成员列表中
	Redis::Reply reply = _redis.command("SADD group:user:" + user_id + " " + src_id);
	
	// 将群添加到用户群列表
	reply = _redis.command("SADD group:list:" + src_id + " " + user_id);

	// 更新群信息
	reply = _redis.command("HINCRBY group:info:" + user_id + " nums 1");

	// 转发报文
	if(online_id_to_con.count(_msg_ptr->dst_id()) == 0)
	{
		temporary_respond();
	}
	else
	{
		_coder.send(online_id_to_con[_msg_ptr->dst_id()], *_msg_ptr);
	}
}


