#ifndef MODULE_FACTORY
#define MODULE_FACTORY

#include "chat/Module.h"
#include "chat/Login.h"
#include "chat/Register.h"
#include "chat/Add_user.h"
#include "chat/Get_friend_list.h"
#include "chat/Get_user_info.h"
#include "chat/Single_msg.h"
#include "chat/Get_chat_record.h"
#include "chat/Create_group.h"
#include "chat/Invite_enter_group.h"
#include "chat/Get_group_list.h"
#include "chat/Get_group_info.h"
#include "chat/Get_group_user_list.h"
#include "chat/Group_msg.h"
#include "chat/Get_group_chat_record.h"
#include <memory>

// 功能模板工厂
class Module_factory
{
public:
	Module_factory() = default;
	~Module_factory(){}
	std::shared_ptr<Module> create(chat::Message_TYPE type)
	{
		if(type == chat::Message::LOGIN)
		{
			return std::shared_ptr<Module>(new Login());
		}
		else if(type == chat::Message::REGISTER)
		{
			return std::shared_ptr<Module>(new Register());
		}
		else if(type== chat::Message::ADD_REQUEST 
				|| type== chat::Message::ADD_RESPOND)
		{
			return std::shared_ptr<Module>(new Add_user());
		}
		else if(type == chat::Message::GET_FRIEND_LIST)
		{
			return std::shared_ptr<Module>(new Get_friend_list());
		}
		else if(type == chat::Message::GET_USER_INFO)
		{
			return std::shared_ptr<Module>(new Get_user_info());
		}
		else if(type == chat::Message::SINGLE_MSG)
		{
			return std::shared_ptr<Module>(new Single_msg());
		}
		else if(type == chat::Message::GET_CHAT_RECORD)
		{
			return std::shared_ptr<Module>(new Get_chat_record());
		}
		else if(type == chat::Message::CREATE_GROUP)
		{
			return std::shared_ptr<Module>(new Create_group());
		}
		else if(type == chat::Message::INVITE_ENTER_GROUP
				|| type == chat::Message::INVITE_ENTER_GROUP_RESPOND)
		{
			return std::shared_ptr<Module>(new Invite_enter_group());
		}
		else if(type == chat::Message::GET_GROUP_LIST)
		{
			return std::shared_ptr<Module>(new Get_group_list());
		}
		else if(type == chat::Message::GET_GROUP_INFO)
		{
			return std::shared_ptr<Module>(new Get_group_info());
		}
		else if(type == chat::Message::GET_GROUP_USER_LIST)
		{
			return std::shared_ptr<Module>(new Get_group_user_list());
		}
		else if(type == chat::Message::GROUP_MSG)
		{
			return std::shared_ptr<Module>(new Group_msg());
		}
		else if(type == chat::Message::GET_GROUP_CHAT_RECORD)
		{
			return std::shared_ptr<Module>(new Get_group_chat_record());
		}
		else
		{
			return NULL;
		}
	}
};

#endif
