#pragma once
#include "Package.h"

class ServerTCP
{
public:
	ServerTCP() {}
	ServerTCP(Uint16 port)
		:	m_port(port)
	{}
	~ServerTCP() 
	{
		/* Close the server socket */
		close(m_socketDescriptor);
		//assert(close(m_socketDescriptor));
	}

	void run();
	void receiveMessages();
	inline Package * getPackage() { return m_serverPackage; }

private:

	Package * m_serverPackage = new Package();
	Uint16 m_port;

	TCPsocket m_socketDescriptor = nullptr, m_clientSocketDescriptor = nullptr;
	IPaddress m_ipAddress, *m_remoteIPAddress;
	int m_quit, m_quit2;
	char m_buffer[BUFFER_SIZE];

	bool init();
	bool resolve();
	bool open();
	bool close(TCPsocket socketToClose);
};

