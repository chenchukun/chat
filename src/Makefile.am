bin_PROGRAMS=chat
chat_LDADD=$(top_srcdir)/lib/libreactor.a
chat_LDFLAGS=-std=c++11 -L$(top_srcdir)/lib -lpthread -L/usr/local/lib -lprotobuf -lhiredis 
INCLUDES=-I$(top_srcdir)/include -I/usr/local/include
AM_CXXFLAGS=-std=c++11
chat_SOURCES=Get_chat_record.cpp Get_user_info.cpp Create_group.cpp Group_msg.cpp Chat_server.cpp chat.pb.cpp Invite_enter_group.cpp Add_user.cpp Register.cpp Coder.cpp Get_group_user_list.cpp Single_msg.cpp Handler.cpp Login.cpp Get_group_chat_record.cpp Get_group_list.cpp main.cpp Get_group_info.cpp Get_friend_list.cpp
