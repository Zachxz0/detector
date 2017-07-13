/*
* Wrote by :Zhuang zhuoxin
*/
#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include <string>
#include <error.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

namespace zoson
{

class ServerCallback
{
public:
	virtual void getClientMessage(int fd,const char *buf,int size) = 0;
	virtual void hasClientConn(int fd)=0;
	virtual void hasClientClose(int fd)=0;
};

class Server
{
public:
	Server();
	~Server();
	void startServer();
	void setAddress(const std::string&);
	void setPost(int post);
	void setClient(int count);
	void sendMessage(const char* msg,int size);
	void sendMessage(int fd,const char* msg,int size);
	void setCallback(ServerCallback* callback);
protected:
	void createSocket();
	void initFdSet();
private:
	std::string m_addr;
	int m_sfd; 
	int *m_cfd;
	int m_client_num;
	int m_post;
	char m_buff[1024];
	fd_set m_global_rdfs;
	fd_set m_current_rdfs;
	fd_set m_global_wdfs;
	fd_set m_current_wdfs;
	int m_max_fd;
	int m_listenfd;
	struct sockaddr_in m_server;
	struct sockaddr_in m_client;

	char *m_send;
	int m_send_size;
	bool able_send;

	ServerCallback* m_callback;
};

}

#endif