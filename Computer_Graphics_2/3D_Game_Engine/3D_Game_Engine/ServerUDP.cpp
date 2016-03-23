#include "ServerUDP.h"
#include <iostream>
#include <string>
#include <assert.h>

void ServerUDP::run()
{
	//init();
	assert(init());
	
	//open();
	assert(open());

	//allocatePacket(BUFFER_SIZE);
	assert(allocatePacket(BUFFER_SIZE));

	m_window = SDL_CreateWindow("Server Window", 600, 0, 100, 100, SDL_WINDOW_OPENGL);
	m_context = SDL_GL_CreateContext(m_window);
	glewInit();

	m_quit = 0;
	while (!m_quit)
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(m_socketDescriptor, m_packet))
		{

			memcpy(m_serverPackage->data, m_packet->data, sizeof(Package));

			switch (m_serverPackage->e.type)
			{
			case SDL_KEYDOWN:
				printf("Key press detected\n");
				bgC = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				break;

			case SDL_KEYUP:
				printf("Key release detected\n");
				bgC = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
				break;

			case SDL_MOUSEMOTION:
				printf("Mouse Moved\n");
				bgC = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
				break;

			case SDL_MOUSEBUTTONDOWN:
				printf("Mouse Button Down\n");
				bgC = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
				break;

			case SDL_MOUSEBUTTONUP:
				printf("Mouse Button Up\n");
				bgC = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
				break;

			default:
				break;
			}

			SDL_GL_SwapWindow(m_window);
			glClearColor(bgC.x, bgC.y, bgC.z, bgC.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	/* Clean and exit */
	freePacket(*m_packet);
	SDLNet_Quit();
}

bool ServerUDP::init()
{
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
		return false;
	}

	printf("SDLNet initialized...\n");
	return true;
}

bool ServerUDP::open()
{
	/* Open a socket */
	if (!(m_socketDescriptor = SDLNet_UDP_Open(m_port)))
	{
		std::cerr << "SDLNet_UDP_Open: " << SDLNet_GetError() << std::endl;
		return false;
	}

	printf("Socket Opened...\n");
	return true;
}

bool ServerUDP::allocatePacket(int size)
{
	/* Make space for the packet */
	if (!(m_packet = SDLNet_AllocPacket(BUFFER_SIZE)))
	{
		std::cerr << "SDLNet_AllocPacket failed..\n" << std::endl;
		return false;
	}
	printf("Packet Allocated...\n");
	return true;
}

void ServerUDP::freePacket(UDPpacket & packetToClose)
{
	SDLNet_FreePacket(&packetToClose);
}
