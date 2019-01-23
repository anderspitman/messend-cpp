#include "messend.hpp"
#include <thread>
#include <chrono>


using namespace messend;

int main(int argc, char** argv) {

    messend::startup();

    Acceptor acceptor(9001);

    PeerResult result = acceptor.accept();

    while (!result.success) {
        result = acceptor.accept();
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    Peer peer = result.peer;

    Message message((uint8_t*)"Hi from server", 14);

    peer.sendMessage(message);

    std::this_thread::sleep_for (std::chrono::milliseconds(1000));

    //result.success = false;
    //while (!result.success) {
    //    result = acceptor.accept();
    //}

    messend::shutdown();

    return 0;
}