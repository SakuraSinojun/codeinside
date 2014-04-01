
#pragma once

#include "packet_parser.h"
#include "Archive.h"

#include <vector>

class PacketDispatcher : public PacketParser {
public:
    PacketDispatcher();
    virtual ~PacketDispatcher();    

    void Dispatch(const void * data, int len = 0);
    bool ShouldQuit(void) {return should_quit_;}
    void ShouldQuit(bool bQuit) {should_quit_ = bQuit;}

public:
    class Watcher {
    public:
        Watcher(int id) : packet_id_(id) {}
        virtual ~Watcher(){}
        int PacketId() { return packet_id_; }
        virtual void ParsePacket(CArchive& ar) = 0;
    private:
        int packet_id_;
    };

    void AddWatcher(Watcher* watcher);
    void RemoveWatcher(Watcher* watcher);

private:
    std::vector<Watcher*>     watcher_vector_;
    bool should_quit_;
};


