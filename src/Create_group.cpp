#include "chat/Create_group.h"
#include <iostream>

void Create_group::run(tiny::Tcp_connection_ptr conn, 
		std::shared_ptr<chat::Message> msg_ptr)
{
	_conn = conn;
	_msg_ptr = msg_ptr;

#ifdef DEBUG
//	std::cout << "Create_group debug" << std::endl;
#endif
	
	create_group();
}


void Create_group::create_group()
{
	// 获取GID
	std::string gid = get_gid();

	set_group_list(gid);

	set_group_info(gid);

	success(gid);

}

void Create_group::success(std::string &gid)
{
	chat::Message msg;
	msg.set_msg_type(chat::Message::CREATE_GROUP);
	msg.set_dst_id(_msg_ptr->src_id());

	chat::Message::Group_info *group_info = new chat::Message::Group_info();

	group_info->set_gid(gid);
	group_info->set_name(_msg_ptr->user_name());
	group_info->set_own(_msg_ptr->src_id());
	group_info->set_nums(1);
	
	msg.set_allocated_group_info(group_info);

	_coder.send(_conn, msg);
}



void Create_group::set_group_info(std::string &gid)
{
	Redis::Reply reply = _redis.command("HSET group:info:" + gid + " gid " + gid);

	reply = _redis.command("HSET group:info:" + gid + " name " + _msg_ptr->user_name());

	reply = _redis.command("HSET group:info:" + gid + " own " + _msg_ptr->src_id());

	reply = _redis.command("HSET group:info:" + gid + " nums 1");
}




void Create_group::set_group_list(std::string &gid)
{
	// 将gid添加到系统群列表
	Redis::Reply reply = _redis.command("SADD group:list " + gid);
	// 判断，先忽略
	

	// 将gid添加到用户的群列表
	reply = _redis.command(std::string("SADD group:list:") 
			+ _msg_ptr->src_id() + " " + gid);
	// 判断，先忽略


	// 往群中添加成员
	reply = _redis.command("SADD group:user:" + gid + " " + _msg_ptr->src_id());
	// 判断，先忽略
}

std::string Create_group::get_gid()
{
	Redis::Reply reply = _redis.command("GET gid:max");
	// 判断，先忽略
	
	std::string gid = reply->str;

	reply = _redis.command("INCR gid:max");
	// 判断，先忽略

	return gid;
}





