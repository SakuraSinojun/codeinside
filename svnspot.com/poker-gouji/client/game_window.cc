

#include "game_window.h"
#include "info.h"

#include "render/Render.h"
#include "render/Surface.h"

#include "tools/lock.h"
#include "tools/socket.h"

#include "packet/deal_packet.h"
#include "packet/scene_packet.h"
#include "packet/play_packet.h"
#include "packet/message_packet.h"

#include "rules/gouji.h"

#include "log.h"

#include <windows.h>
#include <string>
#include <list>

DECLARE_MODULE(gamewin);

#define WM_USER_SERVERDOWN      (WM_USER + 0x100)
#define WM_USER_ONDATARECEIVED  (WM_USER + 0x101)

namespace {

static bool g_thread_running = false;
static DWORD g_threadid;
static HANDLE g_handle;
static GameWindow * g_game_window = NULL;

static std::list<std::string>   g_data_list;
static tools::CLock             g_data_lock;

static DWORD WINAPI _socket_read_proc(LPVOID lpParam)
{
    tools::CSocket& sock = game_info::GetMainSocket();
    while (g_thread_running) {
        try {
            fd_set  rfds;
            struct timeval  tv;
            FD_ZERO(&rfds);
            FD_SET(sock.GetSocket(), &rfds);
            tv.tv_sec = 1;
            tv.tv_usec = 0;
            int ret = select(sock.GetSocket() + 1, &rfds, NULL, NULL, &tv);
            if (ret < 0) {
                throw ("select error");
            } else if (ret == 0) {
            } else {
                char    buffer[1025];
                int ret = sock.Recv(buffer, sizeof(buffer) - 1);
                if (ret <= 0) {
                    LOG() << "Server down";
                    if (g_game_window) {
                        g_game_window->OnServerDown();
                    }
                }
                buffer[ret] = '\0';
                {
                    tools::AutoLock locked(g_data_lock);
                    g_data_list.push_back(std::string(buffer));
                }
                g_game_window->SendMessage(WM_USER_ONDATARECEIVED, 0, 0);
                LOG() << "received:\n" << buffer;
            }
        } catch(const char * e) {
            LOG() << e;
            continue;
        }
    } // while.
    return 0;
}

static void StartSocketThread(void)
{
    g_thread_running = true;
    g_handle = CreateThread(NULL, 0, _socket_read_proc, NULL, 0, &g_threadid);
}

static void WaitSocketThread(void)
{
    g_thread_running = false;
    WaitForSingleObject(g_handle, INFINITE);
    CloseHandle(g_handle);
}

} // namespace 

void GameWindow::HelloWatcher::ParsePacket(CArchive& ar)
{
}

void GameWindow::ServerInfoWatcher::ParsePacket(CArchive& ar)
{
}

void GameWindow::MessageWatcher::ParsePacket(CArchive& ar)
{
    MessagePacket   packet;
    packet.Deserialize(ar);
    std::string data = ar.toStdString();
    GameWindow::GetInstance()->MsgBox(data.c_str());
}

void GameWindow::DealWatcher::ParsePacket(CArchive& ar)
{
    DealPacket  packet;
    packet.Deserialize(ar);

    GameWindow::GetInstance()->OnDeal(packet.Cards());
}

void GameWindow::SceneWatcher::ParsePacket(CArchive& ar)
{
    ScenePacket packet;
    packet.Deserialize(ar);
    GameWindow::GetInstance()->OnScene(packet.Cards(), packet.Owner(), packet.Player(), packet.Map());
}

void GameWindow::PlayWatcher::ParsePacket(CArchive& ar)
{
    PlayPacket  packet;
    packet.Deserialize(ar);
    GameWindow::GetInstance()->OnPlayCards(packet.Ensure());
}

GameWindow* GameWindow::GetInstance(void)
{
    return g_game_window;
}

GameWindow::GameWindow()
    : on_turn_(false)
    , render(NULL)
{
    SCHECK(g_game_window == NULL);
    g_game_window = this;
    
    CSize   size = game_info::GetRenderSize();
    render = new CRender(size.cx, size.cy);

    packet_dispatcher_.AddWatcher(&hello_watcher_);
    packet_dispatcher_.AddWatcher(&server_watcher_);
    packet_dispatcher_.AddWatcher(&message_watcher_);
    packet_dispatcher_.AddWatcher(&deal_watcher_);
    packet_dispatcher_.AddWatcher(&scene_watcher_);
    packet_dispatcher_.AddWatcher(&play_watcher_);
    
    int i;
    for (i=0; i<6; i++) {
        CRect rect = game_info::GetPlayerRect(i);
        CSurface* surface = render->CreateSurface();
        surface->MoveTo(rect.left, rect.top);
        surface->SetSize(rect.Width(), rect.Height());
        surface->Fill(255, 255, 192);
        surface->Show();
        cards_area_surface_[i] = surface;

        std::list<Cards*>   l;
        player_cards_.push_back(l);
    }

    play_surface_ = render->CreateSurface();
    pass_surface_ = render->CreateSurface();
    shao_surface_ = render->CreateSurface();

    play_surface_->Load("data/images/play.png");
    pass_surface_->Load("data/images/pass.png");
    shao_surface_->Load("data/images/shao.png");
    
    play_surface_->MoveTo(game_info::GetPlayButtonRect().left, game_info::GetPlayButtonRect().top);
    pass_surface_->MoveTo(game_info::GetPassButtonRect().left, game_info::GetPassButtonRect().top);
    shao_surface_->MoveTo(game_info::GetShaoButtonRect().left, game_info::GetShaoButtonRect().top);

    play_surface_->Show();
    pass_surface_->Show();
    //shao_surface_->Show();

    clock_surface_ = render->CreateSurface();
    clock_surface_->Load("data/images/clock.png");
    clock_surface_->MoveTo(game_info::GetClockRect(0).left, game_info::GetClockRect(0).top);
    // clock_surface_->Show();

}

GameWindow::~GameWindow()
{
    if (render) {
        delete render;
    }
}

void GameWindow::OnCreate()
{
    StartSocketThread();
}

void GameWindow::OnClose()
{
    WaitSocketThread();
    CWindow::OnClose();
}

void GameWindow::OnSize(int w, int h)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string GameWindow::GetSelectedCards(void)
{
    std::string cards;
    if (cards_.empty())
        return cards;

    std::list<Cards*>::iterator  iter;
    for (iter=cards_.begin(); iter != cards_.end(); iter++) {
        GameWindow::Cards * card = *iter;
        if (card->pos.y != game_info::GetHandCardsRect().top) {
            cards += card->card;
        }
    }
    return cards;
}

void GameWindow::AdjustButtons(void)
{
    std::string cards = GetSelectedCards();
    if (on_turn_) {
        pass_surface_->Load("data/images/pass2.png");
        if (cards.empty()) {
            play_surface_->Load("data/images/play.png");
        } else {
            play_surface_->Load("data/images/play2.png");
        }
        
        if (rules::IsGouJi(scene_.cards) && !game_info::IsOpposite(scene_.owner, game_info::GetPlayerName())) {
            shao_surface_->Load("data/images/shao2.png");
        } else {
            shao_surface_->Load("data/images/shao.png");
        }
        
    } else {
        play_surface_->Load("data/images/play.png");
        pass_surface_->Load("data/images/pass.png");
        shao_surface_->Load("data/images/shao.png");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameWindow::OnPaint(HDC hdc)
{
    BMPHEADER       bm;
    BMPINFOHEADER   bi;
    HBITMAP         hBitmap;
    if (render == NULL)
        return;

    AdjustButtons();


    const char * buffer = render->RenderScene(bm, bi);
    struct {
        BITMAPINFOHEADER    Info;
        DWORD               BitField[3];
    } Header;
    memset(&Header, 0, sizeof(Header));
    memcpy(&Header.Info, &bi, sizeof(bi));

    Header.Info.biPlanes        = 1;
    Header.Info.biCompression   = 0; //BI_BITFIELDS;
    Header.BitField[0]          = 0xff0000;
    Header.BitField[1]          = 0x00ff00;
    Header.BitField[2]          = 0x0000ff;

    hBitmap = CreateDIBitmap(hdc, &Header.Info, CBM_INIT, buffer, (BITMAPINFO*)&Header, DIB_RGB_COLORS);
    if (hBitmap == NULL) {
        return;
    }

    HDC memDC = CreateCompatibleDC(hdc);
    SelectObject(memDC, hBitmap);

    int i;
    for (i=0; i<6; i++) {
        CRect   rect = game_info::GetPlayerRect(i);
        std::string name = game_info::SeatToPlayerName(i);
        if (!name.empty())
            TextOutA(memDC, rect.left, rect.top - 10, name.c_str(), name.length()); 
    }

    CRect   rect;
    GetClientRect(rect);
    SetStretchBltMode(hdc, HALFTONE);
    StretchBlt(hdc, 0, 0, rect.Width(), rect.Height(), memDC, 0, 0, bi.biWidth, bi.biHeight, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(hBitmap);
}

void GameWindow::OnServerDown(void)
{
    this->MsgBox("Server Down!");
    exit(0);
}

void GameWindow::OnDataReceived(void)
{
    std::string data;
    {
        tools::AutoLock locked(g_data_lock);
        data = g_data_list.front();
        g_data_list.pop_front();
    }
    packet_dispatcher_.Dispatch(data.c_str(), data.length());
}

LRESULT GameWindow::OnEraseBackGround(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void GameWindow::SendPacket(Packet* packet)
{
    SCHECK(packet != NULL);
    if (!packet)
        return;

    CArchive    ar;
    std::string data;
    packet->Serialize(ar);
    data = ar.toStdString();

    game_info::GetMainSocket().Send(data.c_str(), data.length());

}

LRESULT GameWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (hWnd == this->GetSafehWnd()) {
        if (uMsg == WM_USER_SERVERDOWN) {
            OnServerDown();
        } else if (uMsg == WM_USER_ONDATARECEIVED) {
            OnDataReceived();
        }
    }
    return CWindow::WindowProc(hWnd, uMsg, wParam, lParam);
}
    
GameWindow::Cards* GameWindow::CardFromPosition(int x, int y)
{
    CRect   rc;
    GetClientRect(rc);

    float step = (game_info::GetHandCardsRect().Width() - game_info::GetCardSize().cx) / (cards_.size()+ 1.0f);

    std::list<Cards*>::iterator  iter;
    for (iter=cards_.begin(); iter != cards_.end(); iter++) {
        GameWindow::Cards * card = *iter;
        CRect   rect;

        rect.left = (LONG)((float)rc.Width() * card->pos.x / game_info::GetRenderSize().cx);
        rect.top  = (LONG)((float)rc.Height() * card->pos.y / game_info::GetRenderSize().cy);
        rect.right = (LONG)((float)rc.Width() * (card->pos.x + step) / game_info::GetRenderSize().cx);
        rect.bottom = (LONG)((float)rc.Height() * (card->pos.y + game_info::GetCardSize().cy) / game_info::GetRenderSize().cy);
        std::list<Cards*>::iterator  it2 = iter;

        it2++;
        if (it2 == cards_.end())
            rect.right = (LONG)((float)rc.Width() * (card->pos.x + game_info::GetCardSize().cx) / game_info::GetRenderSize().cx);

        if (rect.PtInRect(CPoint(x, y))) {
            break;
        }
    }
    if (iter != cards_.end()) {
        return *iter;
    }
    return NULL;
}

CRect GameWindow::ClientToRender(CRect rc)
{
    CPoint  pt1(rc.left, rc.top);
    CPoint  pt2(rc.right, rc.bottom);
    pt1 = ClientToRender(pt1);
    pt2 = ClientToRender(pt2);
    CRect   rect(pt1, pt2);
    return rect;
}

CPoint GameWindow::ClientToRender(CPoint pt)
{
    CRect   rect;
    GetClientRect(rect);
    CRect   rc = CRect(CPoint(0, 0), game_info::GetRenderSize());

    int x = (pt.x - rect.left) * rc.Width() / rect.Width();
    int y = (pt.y - rect.top) * rc.Height() / rect.Height();
    return CPoint(x, y);
}

CRect GameWindow::RenderToClient(CRect rc)
{
    CPoint  pt1(rc.left, rc.top);
    CPoint  pt2(rc.right, rc.bottom);
    pt1 = RenderToClient(pt1);
    pt2 = RenderToClient(pt2);
    CRect   rect(pt1, pt2);
    return rect;    
}

CPoint GameWindow::RenderToClient(CPoint pt)
{
    CRect   rect;
    GetClientRect(rect);
    CRect   rc = CRect(CPoint(0, 0), game_info::GetRenderSize());

    int x = pt.x * rect.Width() / rc.Width();
    int y = pt.y * rect.Height() / rc.Height();
    return CPoint(x, y);
}

void GameWindow::OnClick(int nButton, int x, int y, int keyState)
{
    if (this->on_turn_) {
        CRect   rect =  RenderToClient(game_info::GetPlayButtonRect());
        if (rect.PtInRect(CPoint(x, y))) {
            std::string cards = this->GetSelectedCards();
            if (!cards.empty())
                this->Play(cards);
            return;
        }
        rect = RenderToClient(game_info::GetPassButtonRect());
        if (rect.PtInRect(CPoint(x, y))) {
            this->Play(std::string(""));
            return;
        }
    }

    GameWindow::Cards* card = CardFromPosition(x, y);
    if (card) {
        if (card->pos.y == game_info::GetHandCardsRect().top) {   
            card->pos.y -= 30;
        } else {
            card->pos.y = game_info::GetHandCardsRect().top;
        }
        card->surface->MoveTo(card->pos.x, card->pos.y);
        this->Invalidate();
    }
}

void GameWindow::OnMouseMove(int x, int y, int keyState)
{
}

void GameWindow::OnDeal(std::string set)
{
    std::list<Cards*>::iterator  iter;
    for (iter=cards_.begin(); iter != cards_.end(); iter++) {
        GameWindow::Cards * card = *iter;
        if (render)
            render->Detach(card->surface);
        delete card;
    }
    cards_.clear();

    int len = set.length() / 2;
    int i;

    CRect rect = game_info::GetHandCardsRect();
    float step = (rect.Width() - game_info::GetCardSize().cx) / (len + 1.0f);

    for (i=0; i<len; i++) {
        char    temp[1024];
        std::string card = set.substr(i*2, 2);
        sprintf(temp, "data/images/%s.jpg", card.c_str());

        CSurface* surface = render->CreateSurface();
        surface->Load(temp);

        float left = rect.left + (i + 1) * step;
        surface->MoveTo((int)left, rect.top);
        // surface->SetColorKey(true);
        // surface->SetAlpha(224);

        Cards* cards = new Cards();
        cards->card = card;
        cards->surface = surface;
        cards->pos = CPoint((int)left, rect.top);
        cards_.push_back(cards);

        surface->Show();
    }
    this->Invalidate();
}

void GameWindow::OnScene(std::string set, std::string owner, std::string player, std::map<std::string, std::string>& m)
{
    scene_.cards = set;
    scene_.owner = owner;
    scene_.player = player;

    int oi = game_info::PlayerNameToSeat(owner);
    int pi = game_info::PlayerNameToSeat(player);

    CRect pr = game_info::GetClockRect(pi);
    
    clock_surface_->MoveTo(pr.left, pr.top);
    clock_surface_->Show();
    if (player == game_info::GetPlayerName()) {
        on_turn_ = true;
        this->SetWindowText("Your turn.");
    } else {
        this->SetWindowText("Game");
    }

    {
        std::list<Cards*>::iterator  iter;
        for (iter = player_cards_[pi].begin(); iter != player_cards_[pi].end(); iter++) {
            GameWindow::Cards * card = *iter;
            if (render)
                render->Detach(card->surface);
            delete card;
        }
    }

    if (oi >= 0 && oi < 6) {
        std::list<Cards*>::iterator  iter;
        for (iter = player_cards_[oi].begin(); iter != player_cards_[oi].end(); iter++) {
            GameWindow::Cards * card = *iter;
            if (render)
                render->Detach(card->surface);
            delete card;
        }
        player_cards_[oi].clear();

        int len = set.length() / 2;
        int i;

        CRect rect = game_info::GetPlayerRect(oi);
        float step = (rect.Width() - game_info::GetCardSize().cx) / (len + 1.0f);

        for (i=0; i<len; i++) {
            char    temp[1024];
            std::string card = set.substr(i*2, 2);
            sprintf(temp, "data/images/%s.jpg", card.c_str());

            CSurface* surface = render->CreateSurface();
            surface->Load(temp);

            float left = rect.left + (i + 1) * step;
            surface->MoveTo((int)left, rect.top);
            // surface->SetColorKey(true);
            // surface->SetAlpha(224);

            Cards* cards = new Cards();
            cards->card = card;
            cards->surface = surface;
            cards->pos = CPoint((int)left, rect.top);
            player_cards_[oi].push_back(cards);

            surface->Show();
        }
    }

    std::map<std::string, std::string>::iterator    iter;
    for (iter = m.begin(); iter != m.end(); iter++) {
        int st = game_info::PlayerNameToSeat(iter->first);
        std::string stat = iter->second;
        if (stat == "SUCCESS")
            cards_area_surface_[st]->Fill(0, 255, 0);
        else if (stat == "FAILURE")
            cards_area_surface_[st]->Fill(255, 0, 0);
    }
    this->Invalidate();
}

void GameWindow::Play(std::string cards)
{
    PlayPacket  packet;
    packet.Cards() = cards;
    SendPacket(&packet);
}
    
void GameWindow::OnPlayCards(bool ensure)
{
    if (!ensure) {
        this->MsgBox("出牌不合法！");
    }
}




