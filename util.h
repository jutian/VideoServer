#include <thread>

namespace Util {
    static void sleep(int milliseSeconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    }

    static std::string convertIpv4(int ipv4) {
        int first = (ipv4 & 0xFF000000) >> 24;
        int second = (ipv4 & 0xFF0000) >> 16;
        int third = (ipv4 & 0xFF00) >> 8;
        int fourth = ipv4 & 0xFF;
        std::string str = std::to_string(first).append(".");
        str += std::to_string(second).append(".");
        str += std::to_string(third).append(".");
        str += std::to_string(fourth);
        std::cout<< "convertIpv4: "<< str << std::endl;
        return str;
    }
}