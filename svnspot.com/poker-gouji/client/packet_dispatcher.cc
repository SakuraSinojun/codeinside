

#include "packet_dispatcher.h"
#include "log.h"
#include <algorithm>

DECLARE_MODULE(common);

PacketDispatcher::PacketDispatcher()
    : should_quit_(false)
{
}

PacketDispatcher::~PacketDispatcher()
{
}

void PacketDispatcher::AddWatcher(Watcher* watcher)
{
    SCHECK(watcher != NULL);
    if (watcher == NULL)
        return;
    watcher_vector_.push_back(watcher);
}

void PacketDispatcher::RemoveWatcher(Watcher* watcher)
{
    SCHECK(watcher != NULL);
    if (watcher == NULL)
        return;
    std::vector<Watcher*>::iterator iter;
    iter = std::find(watcher_vector_.begin(),
        watcher_vector_.end(), watcher);

    if (iter != watcher_vector_.end())
        watcher_vector_.erase(iter);
}

void PacketDispatcher::Dispatch(const void * data, int len)
{
    std::string xml;
    for (xml = ParsePacket(data, len); !xml.empty(); xml = ParsePacket(NULL)) {

        CArchive ar;
        int     id;
        try {
            ar.fromStdString(xml);
            ar >> id;
            ar.fromStdString(xml);
        } catch(const char*) {
            continue;
        }

        std::vector<Watcher*>::iterator iter;
        for (iter = watcher_vector_.begin(); iter != watcher_vector_.end(); iter++) {
            Watcher* watcher = *iter;
            if (watcher->PacketId() == id) {
                watcher->ParsePacket(ar);
            }
        }
    }
}




