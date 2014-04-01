

#pragma once

#include "window/Window.h"
#include "window/Rect.h"

#include "packet_dispatcher.h"
#include "Archive.h"
#include "packet/packet.h"

#include "render/Render.h"
#include "render/Surface.h"

#include <list>
#include <string>
#include <map>

class GameWindow : public CWindow {
public:
    GameWindow();
    virtual ~GameWindow();

    static GameWindow* GetInstance(void);

    virtual void OnCreate();
    virtual void OnClose();
    virtual LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnEraseBackGround(WPARAM wParam, LPARAM lParam);
    virtual void OnPaint(HDC hdc);
    virtual void OnMouseMove(int x, int y, int keyState);
    virtual void OnClick(int nButton, int x, int y, int keyState);
    virtual void OnSize(int w, int h);

    void OnServerDown(void);
    void OnDataReceived(void);
    
    void Play(std::string cards);
    
    void OnPlayCards(bool ensure);
    void OnDeal(std::string set);
    void OnScene(std::string cards, std::string owner, std::string player, std::map<std::string, std::string>& m);

private:
    bool    on_turn_;
    void    AdjustButtons(void);
    std::string GetSelectedCards(void);

    struct Scene {
        std::string cards;
        std::string player;
        std::string owner;
    };

    Scene   scene_;

    CSurface* cards_area_surface_[6];

private:
    CRender*    render;
    class Cards {
    public:
        Cards() : surface(NULL) {}
        ~Cards() { if (surface) delete surface; }
        std::string card;
        CSurface*   surface;
        CPoint      pos;
        Cards& operator=(const Cards& o) {
            card = std::string(o.card.c_str());
            surface = o.surface;
            return *this;
        }
    };
    std::list<Cards*>   cards_;
    std::vector<std::list<Cards*> >   player_cards_;

    Cards*  CardFromPosition(int x, int y);
    CRect   ClientToRender(CRect rc);
    CPoint  ClientToRender(CPoint pt);
    CRect   RenderToClient(CRect rc);
    CPoint  RenderToClient(CPoint pt);

    CSurface* play_surface_;
    CSurface* pass_surface_;
    CSurface* shao_surface_;
    CSurface* clock_surface_;

private:
    class HelloWatcher : public PacketDispatcher::Watcher {
    public:
        HelloWatcher() : PacketDispatcher::Watcher(Packet::ID_HELLO) {}
        virtual void ParsePacket(CArchive& ar);
    };

    class ServerInfoWatcher: public PacketDispatcher::Watcher {
    public:
        ServerInfoWatcher() : PacketDispatcher::Watcher(Packet::ID_SERVERINFO) {}
        virtual void ParsePacket(CArchive& ar);
    };

    class MessageWatcher: public PacketDispatcher::Watcher {
    public:
        MessageWatcher() : PacketDispatcher::Watcher(Packet::ID_MESSAGE) {}
        virtual void ParsePacket(CArchive& ar);
    };

    class DealWatcher: public PacketDispatcher::Watcher {
    public:
        DealWatcher() : PacketDispatcher::Watcher(Packet::ID_DEAL) {}
        virtual void ParsePacket(CArchive& ar);
    };

    class SceneWatcher : public PacketDispatcher::Watcher {
    public:
        SceneWatcher() : PacketDispatcher::Watcher(Packet::ID_SCENE) {}
        virtual void ParsePacket(CArchive& ar);
    };

    class PlayWatcher : public PacketDispatcher::Watcher {
    public:
        PlayWatcher() : PacketDispatcher::Watcher(Packet::ID_PLAY) {}
        virtual void ParsePacket(CArchive& ar);
    };

    PacketDispatcher    packet_dispatcher_;
    HelloWatcher        hello_watcher_;
    ServerInfoWatcher   server_watcher_;
    MessageWatcher      message_watcher_;
    DealWatcher         deal_watcher_;
    SceneWatcher        scene_watcher_;
    PlayWatcher         play_watcher_;

    void SendPacket(Packet* packet);

};

