
#include "gate_config.h"

#include "player.h"
#include "game.h"


#include "log.h"
#include "tools/socket.h"
#include "tools/select.h"

#include <cstdlib>
#include <cstring>

#include "packet/server_info_packet.h"
#include "packet/hello_packet.h"
#include "packet/message_packet.h"

DECLARE_MODULE(main);

using namespace tools;
static CSocket  sockL(CSocket::eSocketType_TCP);
static CSelect* selectA = NULL;


static int socket_read_proc(CSocket* sock)
{
    char    buf[1025];
    int     bytes_read;

    if (sock == &sockL) {
        if (Game::GetInstance()->CanAcceptPlayers()) {
            Player* sockP = new Player();
            sock->Accept(*sockP);
            selectA->AddReadSocket(sockP);
            LOG() << sockP->GetRemoteIP() << '(' << sockP->GetRemotePort() << ") connected.";
        } else {
            MessagePacket   packet;
            packet.Message() = "server full!";
            CArchive        ar;
            packet.Serialize(ar);
            std::string     xml = ar.toStdString();

            CSocket* sockP = new CSocket(CSocket::eSocketType_TCP);
            sock->Accept(*sockP);
            sockP->Send(xml.c_str(), xml.length());
            LOG() << sockP->GetRemoteIP() << '(' << sockP->GetRemotePort() << ") refused.";
            sockP->Close();
            delete sockP;
        }
        return 0;
    }
    memset(buf, 0, sizeof(buf));
    bytes_read = sock->Recv(buf, sizeof(buf) - 1);
    if (bytes_read <= 0) {
        selectA->DelReadSocket(sock);
        LOG() << sock->GetRemoteIP() << '(' << sock->GetRemotePort() << ") disconnected.";
        Game::GetInstance()->LeavePlayer(dynamic_cast<Player*>(sock));
        sock->Close();
        delete sock;
    } else {
        buf[bytes_read] = '\0';
        LOG() << "message from " << sock->GetRemoteIP() << '(' << sock->GetRemotePort()<< "):\n" << buf;
    }
    return 0;
}

int main()
{
    InitLog();
    CSocket::InitNetwork();

    selectA = new CSelect();

    int     port;
    try {
        sockL.Create();
        port = kGatePortBegin;
        while (true) {
            try {
                LOG() << "trying port: " << port << "...";
                sockL.Bind(port);
                break;
            } catch(...) {
                port ++;
                if (port >= kGatePortEnd) {
                    LOG() << "bind failed. all ports are used between " << kGatePortBegin << " and " << kGatePortEnd;
                    return -1;
                }
            }
        }
        sockL.Listen();
        LOG() << "listening on port " << port;
        selectA->AddReadSocket(&sockL);
        selectA->SetTimeout(10);
        selectA->RegisterReadCallback(socket_read_proc);
        selectA->Run();
    } catch(const char * e) {
        LOG() << e;
    }
    delete selectA;
    return 0;
}

