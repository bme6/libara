/*
 * $FU-Copyright$
 */

#ifndef ABSTRACT_ARACLIENT_MOCK_BASE_H_
#define ABSTRACT_ARACLIENT_MOCK_BASE_H_

#include "ARAMacros.h"
#include "AbstractNetworkClient.h"
#include "NetworkInterfaceMock.h"
#include "Configuration.h"
#include "RoutingTable.h"
#include "PacketFactory.h"

#include <string>
#include <deque>

ARA_NAMESPACE_BEGIN

class NetworkInterfaceMock;

/**
 * The abstract AbstractClientMockBase supplies several mocking methods that can be
 * used by its implementing mocks (ARACLientMock and EARAClientMock)
 */
class AbstractClientMockBase : public virtual AbstractNetworkClient {
public:
    struct PacketInfo {
        const Packet* packet;
        std::shared_ptr<Address> nextHop;
        const NetworkInterface* interface;
    };

    AbstractClientMockBase() {};
    virtual ~AbstractClientMockBase();

    void storeReceivedPacket(Packet* packet, NetworkInterface* interface);
    void storeRouteFailurePacket(Packet* packet, AddressPtr nextHop, NetworkInterface* interface);
    void storeDeliveredPacket(const Packet* packet);
    void storeUndeliverablePacket(const Packet* packet);

    // Mocking methods

    std::deque<const Packet*>* getDeliveredPackets();
    virtual NetworkInterfaceMock* createNewNetworkInterfaceMock(const std::string localAddressName = "DEFAULT");

    int getNumberOfReceivedPackets();
    std::deque<Pair<const Packet*, const NetworkInterface*>*> getReceivedPackets();

    int getNumberOfRouteFailures();
    std::deque<PacketInfo> getRouteFailurePackets();

    int getNumberOfUndeliverablePackets();
    std::deque<const Packet*> getUndeliverablePackets();

protected:
    std::deque<NetworkInterfaceMock*> interfaceMocks;
    std::deque<const Packet*> deliveredPackets;
    std::deque<Pair<const Packet*, const NetworkInterface*>*> receivedPackets;
    std::deque<PacketInfo> routeFailurePackets;
    std::deque<const Packet*> undeliverablePackets;
};

ARA_NAMESPACE_END

#endif /* ABSTRACT_ARACLIENT_MOCK_BASE_H_ */
