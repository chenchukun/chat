#ifndef CODER_H
#define CODER_H

#include "protobuf/chat.pb.h"
#include "reactor/Tcp_connection.h"
#include <string>
#include <memory>
/*
 * 消息编码器
 * 实现对接受到的消息进行解码，然后返回Message类型指针
 * 接受Message类型，对其进行编码成string然后发送出去
 */ 

class Coder
{
public:
	Coder() = default;
	~Coder(){}
	std::shared_ptr<chat::Message> recv(tiny::Tcp_connection_ptr conn);
	void send(tiny::Tcp_connection_ptr conn, chat::Message &msg);

private:
	// 从缓冲区读取一个报文长度，读取失败返回-1，成功返回报文长度
	int message(tiny::Buffer &buffer, string &msg);
	std::shared_ptr<chat::Message> loads(const std::string &str_msg);
	std::string dumps(chat::Message &msg_msg);
	const uint32_t HEAD_SIZE = 4;
};

#endif
