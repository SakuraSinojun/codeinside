

#include "getopt.h"
#include "connect.h"
#include "info.h"

#include "log.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <windows.h>

static std::string  g_exename = "poker";
static std::string  g_server_address = "127.0.0.1";
static int          g_server_port = 10234;

static void usage(void)
{
    printf("usage: %s --server SERVERIP --port PORT\n", g_exename.c_str());
}

static int parse_arguments(int argc, char* argv[])
{
    struct option long_options[] = {
        {"help",    no_argument,        NULL,   'h'},
        {"server",  required_argument,  NULL,   's'},
        {"port",    required_argument,  NULL,   'p'},
        {"name",    required_argument,  NULL,   'n'},
        {NULL,      no_argument,        NULL,   0}
    };
    int     option_index = 0;
    int     c;
    while (true) {
        c = getopt_long(argc, argv, "hs:p:n:", long_options, &option_index);
        switch (c) {
        case -1:
            return 0;
            break;
        case 'h':
            usage();
            return -1;
            break;
        case 's':
            g_server_address = optarg;
            break;
        case 'p':
            g_server_port = atoi(optarg);
            break;
        case 'n':
            game_info::SetPlayerName(std::string(optarg));
            break;
        case '?':
            return -1;
            break;
        default:
            break;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    g_exename = argv[0];
    InitLog();
    if (parse_arguments(argc, argv) != 0)
        return -1;
    connect_start(g_server_address.c_str(), g_server_port);
    return 0;
}

