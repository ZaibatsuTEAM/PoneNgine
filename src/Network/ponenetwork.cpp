#include "ponenetwork.h"
#include "qogrewindow.h"
#include "ponecore.h"

// A lot of the code here is from ENet's tutorial
// It will be changed to something more fitting for a game in the future
// Since I am new at this it might take a while.

PonyNetwork::PonyNetwork()
{

}

PonyNetwork::~PonyNetwork()
{
    destroyClient();
    destroyServer();
    deInit();
}

void PonyNetwork::init()
{
    if (enet_initialize() != 0)
    {
        pne.logMsg(QString(PREF_LOG_SEVERE)+"An Error occured while initializing the PoneNgine Network");
        exit(EXIT_FAILURE);
    }
}

void PonyNetwork::deInit()
{
    atexit(enet_deinitialize);
}

void PonyNetwork::createClient()
{
    client = enet_host_create(NULL,         // Create a client host
                              1,            // Only allow 1 outgoing connection
                              2,            // Allow up to 2 channels to be used, 0 and 1
                              57600 / 8,    // 56K modem with 56 Kbps downstream bandwidth
                              14400 / 8);   // 56K modem with 14 Kbps upstream bandwidth

    if (client == NULL)
    {
        pne.logMsg(QString(PREF_LOG_SEVERE)+"An Error occured while trying to create client host.");
        exit(EXIT_FAILURE);
    }
}

void PonyNetwork::destroyClient()
{
    enet_host_destroy(client);
}

void PonyNetwork::createServer()
{
    address.host = ENET_HOST_ANY;
    address.port = 1041;

    server = enet_host_create(&address, // Bind the address the server will host on
                              32,       // Allow up to 32 Players (clients and/our outgoing connections
                              2,        // Allow up to 2 channels to be used, 0 and 1
                              0,        // Assume any amount of incoming bandwidth
                              0);       // Assume any amound of outgoing bandwidth

    if (server == NULL)
    {
        pne.logMsg(QString(PREF_LOG_SEVERE)+"An Error occured while trying to create local server host.");
        exit(EXIT_FAILURE);
    }
}

void PonyNetwork::destroyServer()
{
    enet_host_destroy(server);
}

void PonyNetwork::event(ENetEvent event)
{
    while (enet_host_service(client,&event, 100) > 0)
    {
        // Do some cazy l33t conversion of void* data from event.peer to a QString
        void *peerPacketdataV = event.peer->data;

        QString &peerPacketdataS = *static_cast<QString*>(peerPacketdataV); // Yo dawg, I herd you like pointers
        QString peerPacketdataF = peerPacketdataS;

        switch(event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            pne.logMsg(QObject::tr("%1A new client has connected from %2:%3").
                       arg(PREF_LOG_NETINFO).
                       arg((unsigned int)event.peer->address.host). // 32 uint
                       arg((unsigned short)event.peer->address.port)); // 16 ushort, 8 uchar

            event.peer->data = "Client Information";

            break;
        case ENET_EVENT_TYPE_RECEIVE:
            pne.logMsg(QObject::tr("%1A packet of lenght %2 containing %3 was received from %4 on channel %5").
                       arg(PREF_LOG_NETINFO).
                       arg(event.packet->dataLength).
                       arg((unsigned char)event.packet->data).
                       arg(static_cast<QString>(peerPacketdataF)).
                       arg(QString().setNum(event.channelID)));

            enet_packet_destroy(event.packet);
        case ENET_EVENT_TYPE_DISCONNECT:
            pne.logMsg(QObject::tr("%1%2 has disconnected").arg(PREF_LOG_NETINFO).arg(static_cast<QString>(peerPacketdataF)));

            event.peer->data = NULL;
        }

        // Dear Princess Celestia
        // Today I learned what the fuck static_cast is and how to use it.
        // Sinceresly, Wapa Da Viddter
    }
}
