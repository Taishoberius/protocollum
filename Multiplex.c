
#include "utils.h"
#include "server.h"

int main(int argc, char const *argv[]) {
    handle_server_events(init_server(AF_INET, INADDR_ANY, 7777, SOCK_STREAM, 0));
}
