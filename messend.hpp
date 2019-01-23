#include "messend.h"
#include <iostream>

using namespace std;

namespace messend {

    void startup() {
        messend_startup();
    }

    void shutdown() {
        messend_shutdown();
    }

    class Message {
        public:
            Message(uint8_t* data, uint64_t size) : data(data), size(size) {
            }

            uint8_t* data;
            uint64_t size;
    };

    class Peer {
        public:

            Peer(MessendPeer peer) : peer(peer) {
            }

            ~Peer() {
                if (this->peer) {
                    cout << "Free dis biz" << endl;
                    messend_peer_free(this->peer);
                    this->peer = 0;
                }
            }

            void sendMessage(Message mess) {
                MessendMessage message;

                message.data = mess.data;
                message.size = mess.size;

                messend_peer_send_message(this->peer, message);
            }

            MessendPeer getPeer() {
                return this->peer;
            }

        private:
            MessendPeer peer;
    };

    class PeerResult {
        public:
            PeerResult() : success(false), peer(NULL) {
            }

            PeerResult(bool success, Peer* peer) : success(success), peer(peer) {
            }

            bool success;
            Peer* peer;
    };


    class Acceptor {
        public:
            Acceptor(uint16_t port) : port(port) {
                this->acceptor = messend_acceptor_create(port);
            }

            ~Acceptor() {
                messend_acceptor_free(this->acceptor);
                this->acceptor = 0;
            }

            PeerResult accept() {
            //MessendPeer accept() {

                MessendPeer mpeer = messend_acceptor_accept(this->acceptor);
                //return peer;

                if (mpeer) {
                    Peer* peer = new Peer(mpeer);
                    return PeerResult(true, peer);
                }
                else {
                    return PeerResult(false, NULL);
                }
            }

            MessendAcceptor getAcceptor() {
                return this->acceptor;
            }

        private:
            uint16_t port;
            MessendAcceptor acceptor;
    };
}
