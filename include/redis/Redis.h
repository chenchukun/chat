#ifndef REDIS_H
#define REDIS_H

#include <hiredis/hiredis.h>
#include <string>
#include <assert.h>
#include <memory>
#include <iostream>

class Redis
{
public:
	typedef std::shared_ptr<redisReply> Reply;
	Redis(const std::string &ip, int port)
	{
		con = redisConnect(ip.c_str(), port);
		assert(con != NULL);
	}

	~Redis()
	{
		redisFree(con);
	}

	Reply command(const std::string &com)
	{
		redisReply *p = static_cast<redisReply*>(redisCommand(con, com.c_str()));
//		std::cout << "Redis debug: " <<  com.c_str() << std::endl;
//		return Reply(p, [](redisReply *p){freeReplyObject(p); std::cout << "freeReply" << std::endl;});
		return Reply(p, [](redisReply *p){freeReplyObject(p);});
//		return Reply(p);
	}

	

private:
	redisContext *con;	
};

#endif
