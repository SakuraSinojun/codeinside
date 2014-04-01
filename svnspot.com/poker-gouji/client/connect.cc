

#include "connect.h"
#include "game.h"
#include "packet_dispatcher.h"
#include "info.h"

#include "log.h"
#include "tools/socket.h"
#include "packet/packet.h"
#include "packet/hello_packet.h"
#include "packet/server_info_packet.h"
#include "packet/game_start_packet.h"
#include "Archive.h"

#include <cstring>
#include <string>
using namespace tools;

DECLARE_MODULE(connect);


namespace {
static CSocket sock(CSocket::eSocketType_TCP);
static void send_packet(Packet* packet)
{
    SCHECK(packet != NULL);
    if (packet == NULL)
        return;
    
    CArchive    ar;
    packet->Serialize(ar);

    std::string data = ar.toStdString();
    sock.Send(data.c_str(), data.length());
}

static PacketDispatcher g_packet_dispatcher;

class HelloPacketWatcher : public PacketDispatcher::Watcher {
public:
    HelloPacketWatcher() : PacketDispatcher::Watcher(Packet::ID_HELLO) {}
    virtual ~HelloPacketWatcher() {}
    virtual void ParsePacket(CArchive& ar) {
        HelloPacket hello;
        hello.Deserialize(ar);
        game_info::SetPlayerName(hello.PlayerID());
    }
};

class ServerInfoPacketWatcher : public PacketDispatcher::Watcher {
public:
    ServerInfoPacketWatcher() : PacketDispatcher::Watcher(Packet::ID_SERVERINFO) {}
    virtual ~ServerInfoPacketWatcher() {}
    virtual void ParsePacket(CArchive& ar) {
        ServerInfoPacket    packet;
        packet.Deserialize(ar);

        LOG() << "Server: " << packet.ServerInfo();
        LOG() << "Players: " << packet.Players() << '/' << packet.MaxPlayers();

    }
};

class GameStartPacketWatcher : public PacketDispatcher::Watcher {
public:
    GameStartPacketWatcher() : PacketDispatcher::Watcher(Packet::ID_GAMESTART) {}
    virtual ~GameStartPacketWatcher() {}
    virtual void ParsePacket(CArchive& ar) {
        g_packet_dispatcher.ShouldQuit(true);
        GameStartPacket packet;
        packet.Deserialize(ar);
        game_start(sock, packet);
    }
};
}

void connect_start(const char * server, int port)
{

    HelloPacketWatcher      hiwatcher;
    ServerInfoPacketWatcher siwatcher;
    GameStartPacketWatcher  gswatcher;

    g_packet_dispatcher.AddWatcher(&hiwatcher);
    g_packet_dispatcher.AddWatcher(&siwatcher);
    g_packet_dispatcher.AddWatcher(&gswatcher);

    try {
        tools::CSocket::InitNetwork();
        sock.Create();
        sock.SetRemoteIP(server);
        sock.SetRemotePort(port);
        sock.Connect();

        HelloPacket hello;
        hello.PlayerID() = game_info::GetPlayerName();
        send_packet(&hello);


        int     to = 0;
        while (true) {
            fd_set  rfds;
            struct timeval  tv;
            FD_ZERO(&rfds);
            FD_SET(sock.GetSocket(), &rfds);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            int ret = select(sock.GetSocket() + 1, &rfds, NULL, NULL, &tv);
            if (ret < 0) {
                throw("select error.");
            } else if (ret == 0) {
                // RUN_HERE();
                // send_packet(&hello);
                to++;
                if (to > 60)
                    throw("server hasn't send any message for a long time.");
                continue;
            } else {
                to = 0;
                char    buffer[1025];
                int ret = sock.Recv(buffer, sizeof(buffer) - 1);
                if (ret <= 0)
                    throw ("server down.");
                buffer[ret] = '\0';
                LOG() << "\n" << buffer;
                g_packet_dispatcher.Dispatch(buffer, ret);
                if (g_packet_dispatcher.ShouldQuit()) {
                    break;
                }
            }
        }
    } catch (const char * e) {
        LOG() << e;
    }
    sock.Close();
    g_packet_dispatcher.RemoveWatcher(&hiwatcher);
    g_packet_dispatcher.RemoveWatcher(&siwatcher);
    g_packet_dispatcher.RemoveWatcher(&gswatcher);
}

