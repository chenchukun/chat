#include "protobuf/Coder.h"
#include <assert.h>
#include <arpa/inet.h>

int Coder::message(tiny::Buffer &buffer, string &msg)
{
	if(buffer.size() >= HEAD_SIZE)
	{
		// 获取头部报文长度
		uint32_t *p_len = reinterpret_cast<uint32_t*>(buffer.head());
		uint32_t msg_len = ntohl(*p_len);

		// 获取报文
		if(buffer.size() >= msg_len + HEAD_SIZE)
		{
			buffer.discard_nbyte(HEAD_SIZE);
			buffer.read(msg, msg_len);

			return msg_len;
		}
	}
	return -1;
}

std::shared_ptr<chat::Message> Coder::loads(const std::string &str_msg)
{
	std::shared_ptr<chat::Message> msg_ptr(new chat::Message());
	bool ret = msg_ptr->ParseFromString(str_msg);
	assert(ret);

	return msg_ptr;

}

std::string Coder::dumps(chat::Message &msg_msg)
{
	std::string str_msg;
	bool ret = msg_msg.SerializeToString(&str_msg);
	assert(ret);

	return std::move(str_msg);
}

std::shared_ptr<chat::Message> Coder::recv(tiny::Tcp_connection_ptr conn)
{
	std::string str_msg;
	int ret = message(conn->read_buffer(), str_msg);
	if(ret != -1)
	{
		return loads(str_msg);
	}
	return std::shared_ptr<chat::Message>(NULL);
}

void Coder::send(tiny::Tcp_connection_ptr conn, chat::Message &msg)
{
	std::string str_msg = dumps(msg);
	uint32_t msg_len = str_msg.size();
#ifdef DEBUG
//	std::cout << "msg_len: " << msg_len << std::endl;
#endif
	msg_len = htonl(msg_len);
	conn->send(&msg_len, sizeof(msg_len));
	conn->send(str_msg);
}


