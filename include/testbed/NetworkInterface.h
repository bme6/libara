/*
 * $FU-Copyright$
 */

#ifndef _NETWORK_INTERFACE_H_
#define _NETWORK_INTERFACE_H_

#include "Testbed.h"
#include "ReliableNetworkInterface.h"

TESTBED_NAMESPACE_BEGIN

class NetworkInterface : public ReliableNetworkInterface {
    public:
        NetworkInterface(dessert_meshif_t* dessertPointer, AbstractARAClient* client, PacketFactory* packetFactory, int ackTimeoutInMicroSeconds);
        void registerInterface();
        bool isRegistered();
        void receive(Packet* packet);
        bool equals(ARA::NetworkInterface* otherInterface);
        dessert_meshif_t* getDessertPointer()  const;

    protected:
        void doSend(const Packet* packet, std::shared_ptr<Address> recipient);
        void deliverToARAClient(Packet* packet);
        static AddressPtr localAddress;
        static AddressPtr broadcastAddress;
        dessert_meshif_t* dessertPointer;
};

TESTBED_NAMESPACE_END

#endif // _TESTBED_INTERFACE_H_
