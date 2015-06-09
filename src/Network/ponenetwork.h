#ifndef PONENETWORK_H
#define PONENETWORK_H

#include <ponesettings.h>

#if PONY_PLATFORM == PLAT_WIN32 && defined(_MSC_VER)
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#endif

#include <enet/enet.h>

class PonyNetwork
{
public:
    PonyNetwork();
    ~PonyNetwork();

// You can override these functions to use a different network library if you choose.
protected:
    virtual void init();
    virtual void deInit();
    virtual void createClient();
    virtual void destroyClient();
    virtual void createServer();
    virtual void destroyServer();
    virtual void event(ENetEvent event);

private:
    ENetHost * client;
    ENetHost * server;
    ENetAddress address;
    ENetEvent pEvent;
};

#endif // PONENETWORK_H
