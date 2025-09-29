#include <iostream>
#include <shared_mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include "util.h"

class Linker {
public:
    bool init() {
        _init_socket(8990, 100);
        _m_thread = new std::thread(&Linker::_accept, this);
        _m_thread->detach();
        return true;
    }

    void exit() {
        std::cout << "begin exit thread" << std::endl;
        _m_running = false;
        std::lock_guard<std::mutex> lk(_m_running_mutex);
    }

private:
    bool _init_socket(int listen_port, int max_client)
    {
        _m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in server_addr = { 0 };
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(listen_port);
        int option = 1;
        _set_nonblock(_m_server_fd);
        if (setsockopt(_m_server_fd
                , SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)) < 0)
        {
            perror("setsockopt");
            return false;
        }
        if (bind(_m_server_fd, (sockaddr*)&server_addr
                                    , sizeof(sockaddr)) < 0)
        {
            perror("bind");
            return false;
        }
        if (listen(_m_server_fd, max_client) < 0)
        {
            perror("listen");
            return false;
        }
        return true;
    }

    void _accept()
    {
        int clientFd = 0;
        _m_running = true;
        std::lock_guard<std::mutex> lk(_m_running_mutex);
        while (_m_running) {
            clientFd = accept(_m_server_fd, (sockaddr*)&_M_addr, &_M_addr_len);
            if (clientFd < 0) {
                std::cout << "clientFd:" << clientFd << std::endl;
            } else {
                std::cout << "recv client fd:" << clientFd << std::endl;
                
                std::string ipv4 = Util::convertIpv4(ntohl(_M_addr.sin_addr.s_addr));
                int port = ntohs(_M_addr.sin_port);
                printf("addr: %s %d\n", ipv4.c_str(), port);
            }

            Util::sleep(5000);
        }
        std::cout << "exit accept thread" << std::endl;
    }

    bool _set_nonblock(int _fd)
    {
        int flags = fcntl(_fd, F_GETFL, 0);
        if (fcntl(_fd, F_SETFL, flags | O_NONBLOCK) < 0)
        {
            return false;
        }
        return true;
    }

    std::thread* _m_thread = nullptr;
    int _m_server_fd;
    sockaddr_in _M_addr = { 0 };
    socklen_t _M_addr_len = sizeof(sockaddr_in);
    bool _m_running = false;
    std::mutex _m_running_mutex;
};