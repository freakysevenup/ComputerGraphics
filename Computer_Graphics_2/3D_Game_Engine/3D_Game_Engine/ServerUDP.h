#pragma once
#include "Package.h"
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <GLM/glm.hpp>

class ServerUDP
{
public:
	ServerUDP(Uint16 port)
		: m_port(port)
	{}
			
	~ServerUDP()
	{
		SDL_GL_DeleteContext(m_context);
	}

	void run();

private:
	Uint16 m_port;

	UDPsocket m_socketDescriptor;       /* Socket descriptor */
	UDPpacket *m_packet;       /* Pointer to packet memory */
	int m_quit;
	Package *m_serverPackage = new Package();

	SDL_Window * m_window;
	SDL_GLContext m_context;

	glm::vec4 bgC = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	bool init();
	bool allocatePacket(int size);
	bool open();
	void freePacket(UDPpacket & packetToFree);
};

