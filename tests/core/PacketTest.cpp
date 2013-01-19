/******************************************************************************
 Copyright 2012, The DES-SERT Team, Freie Universität Berlin (FUB).
 All rights reserved.

 These sources were originally developed by Friedrich Große
 at Freie Universität Berlin (http://www.fu-berlin.de/),
 Computer Systems and Telematics / Distributed, Embedded Systems (DES) group
 (http://cst.mi.fu-berlin.de/, http://www.des-testbed.net/)
 ------------------------------------------------------------------------------
 This program is free software: you can redistribute it and/or modify it under
 the terms of the GNU General Public License as published by the Free Software
 Foundation, either version 3 of the License, or (at your option) any later
 version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along with
 this program. If not, see http://www.gnu.org/licenses/ .
 ------------------------------------------------------------------------------
 For further information and questions please use the web site
 http://www.des-testbed.net/
 *******************************************************************************/

#include "CppUTest/TestHarness.h"
#include <cstring>
#include "testAPI/mocks/AddressMock.h"
#include "Packet.h"
#include "PacketType.h"

using namespace ARA;

typedef std::shared_ptr<Address> AddressPtr;

TEST_GROUP(PacketTest) {};

TEST(PacketTest, testCreate) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender (new AddressMock("sender"));    // This is the node from which the packet has actually been received
    char type = PacketType::FANT;
    unsigned int seqNr = 1;

    Packet packet = Packet(source, destination, sender, type, seqNr);

    CHECK(packet.getSource()->equals(source));
    CHECK(packet.getDestination()->equals(destination));
    CHECK(packet.getSender()->equals(sender));
    CHECK_EQUAL(type, packet.getType());
    CHECK_EQUAL(seqNr, packet.getSequenceNumber());
    CHECK_EQUAL(1, packet.getHopCount());

    CHECK_EQUAL(0, packet.getPayloadLength());
    CHECK(packet.getPayload() == false);
}

TEST(PacketTest, testCreateWithPayload) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender (new AddressMock("sender"));
    char type = PacketType::DATA;
    unsigned int seqNr = 2;
    const char* payload = "Hello World!";

    Packet packet = Packet(source, destination, sender, type, seqNr, payload, strlen(payload));

    CHECK(packet.getSource()->equals(source));
    CHECK(packet.getDestination()->equals(destination));
    CHECK(packet.getSender()->equals(sender));
    CHECK_EQUAL(type, packet.getType());
    CHECK_EQUAL(seqNr, packet.getSequenceNumber());
    CHECK_EQUAL(1, packet.getHopCount());

    LONGS_EQUAL(strlen(payload), packet.getPayloadLength());
    CHECK_EQUAL(payload, packet.getPayload());
}

TEST(PacketTest, testCreateWithPayloadAndHopCount) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender (new AddressMock("sender"));
    char type = PacketType::DATA;
    unsigned int seqNr = 3;
    const char* payload = "Hello World";
    unsigned int hopCount = 123;

    Packet packet = Packet(source, destination, sender, type, seqNr, payload, strlen(payload), hopCount);

    CHECK(packet.getSource()->equals(source));
    CHECK(packet.getDestination()->equals(destination));
    CHECK(packet.getSender()->equals(sender));
    CHECK_EQUAL(type, packet.getType());
    CHECK_EQUAL(seqNr, packet.getSequenceNumber());
    LONGS_EQUAL(strlen(payload), packet.getPayloadLength());
    CHECK_EQUAL(payload, packet.getPayload());
    CHECK_EQUAL(hopCount, packet.getHopCount());
}

TEST(PacketTest, testCreateWithoutPayloadButWithHopCount) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender (new AddressMock("sender"));
    char type = PacketType::FANT;
    unsigned int seqNr = 1;
    unsigned int hopCount = 7;

    Packet packet = Packet(source, destination, sender, type, seqNr, hopCount);

    CHECK(packet.getSource()->equals(source));
    CHECK(packet.getDestination()->equals(destination));
    CHECK(packet.getSender()->equals(sender));
    CHECK_EQUAL(type, packet.getType());
    CHECK_EQUAL(seqNr, packet.getSequenceNumber());
    CHECK_EQUAL(hopCount, packet.getHopCount());

    CHECK_EQUAL(0, packet.getPayloadLength());
    CHECK(packet.getPayload() == false);
}

TEST(PacketTest, testCreateFANT) {
   AddressPtr source (new AddressMock("source"));
   AddressPtr destination (new AddressMock("destination"));
   AddressPtr sender (new AddressMock("sender"));
   unsigned int type = PacketType::DATA;
   int seqNr = 3;
   const char* payload = "Hello World";
   unsigned int hopCount = 123;

   Packet packet = Packet(source, destination, sender, type, seqNr, payload, strlen(payload), hopCount);
   unsigned int newSequenceNumber = 242342;
   Packet* fant = packet.createFANT(newSequenceNumber);

   CHECK(fant->getSource()->equals(source));
   CHECK(fant->getDestination()->equals(destination));
   // The sender of a FANT will be determined when it is actually send by the ARA client
   CHECK_EQUAL(PacketType::FANT, fant->getType());
   CHECK_EQUAL(newSequenceNumber, fant->getSequenceNumber());
   CHECK_EQUAL(0, fant->getPayloadLength());
   CHECK_EQUAL(hopCount, fant->getHopCount());

   delete fant;
}

TEST(PacketTest, testClone) {
   AddressPtr source (new AddressMock("source"));
   AddressPtr destination (new AddressMock("destination"));
   AddressPtr sender (new AddressMock("sender"));
   char type = PacketType::DATA;
   unsigned int seqNr = 3;
   const char* payload = "Hello World";
   unsigned int hopCount = 123;

   Packet packet = Packet(source, destination, sender, type, seqNr, payload, strlen(payload), hopCount);
   Packet* clone = packet.clone();

   CHECK(clone->getSource()->equals(source));
   CHECK(clone->getDestination()->equals(destination));
   CHECK(clone->getSender()->equals(sender));
   CHECK_EQUAL(type, clone->getType());
   CHECK_EQUAL(seqNr, clone->getSequenceNumber());
   LONGS_EQUAL(strlen(payload), clone->getPayloadLength());
   CHECK_EQUAL(payload, clone->getPayload());
   CHECK_EQUAL(hopCount, clone->getHopCount());
   CHECK(packet.equals(clone));

   delete clone;
}

TEST(PacketTest, testSetHopCount) {
   AddressPtr source (new AddressMock("source"));
   AddressPtr destination (new AddressMock("destination"));
   AddressPtr sender (new AddressMock("sender"));
   unsigned int type = PacketType::DATA;
   int seqNr = 3;
   const char* payload = "Hello World";
   unsigned int hopCount = 123;

   Packet packet = Packet(source, destination, sender, type, seqNr, payload, strlen(payload), hopCount);
   CHECK_EQUAL(hopCount, packet.getHopCount());

   unsigned int newHopCount = 124;
   packet.setHopCount(newHopCount);
   CHECK_EQUAL(newHopCount, packet.getHopCount());
}

TEST(PacketTest, testEquals) {
   unsigned int dataPacket = PacketType::DATA;
   const char* payload = "Hello World";

   AddressPtr source1 (new AddressMock("Source1"));
   AddressPtr source2 (new AddressMock("Source2"));
   AddressPtr destination (new AddressMock("Destination"));
   AddressPtr sender (new AddressMock("Sender"));

   Packet packet            = Packet(source1, destination, sender, dataPacket, 1, payload, strlen(payload));
   Packet samePacket        = Packet(source1, destination, sender, dataPacket, 1, payload, strlen(payload));
   Packet nextSeqPacket     = Packet(source2, destination, sender, dataPacket, 2, payload, strlen(payload));
   Packet otherSourcePacket = Packet(source2, destination, sender, dataPacket, 1, payload, strlen(payload));
   Packet otherPacket       = Packet(source2, destination, sender, dataPacket, 3, payload, strlen(payload));

   CHECK(packet.equals(&packet));
   CHECK(packet.equals(&samePacket));
   CHECK(packet.equals(&nextSeqPacket) == false);
   CHECK(packet.equals(&otherSourcePacket) == false);
   CHECK(packet.equals(&otherPacket) == false);
}

TEST(PacketTest, testCalculatePayloadLength) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender (new AddressMock("sender"));
    char type = PacketType::DATA;
    unsigned int seqNr = 1;
    const char* payload = "Hello World";

    Packet packet = Packet(source, destination, sender, type, seqNr, payload);
    LONGS_EQUAL(11, packet.getPayloadLength())
}

TEST(PacketTest, testSetSender) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender1 (new AddressMock("originalSender"));
    AddressPtr sender2 (new AddressMock("originalSender"));
    char type = PacketType::DATA;
    unsigned int seqNr = 1;
    const char* payload = "Hello World";

    Packet packet = Packet(source, destination, sender1, type, seqNr, payload);
    CHECK(packet.getSender()->equals(sender1));

    packet.setSender(source);   // will delete sender1
    CHECK(packet.getSender()->equals(source));

    // now we test what happens if the original sender has been the source
    packet.setSender(sender2);  // should NOT delete source
    CHECK(packet.getSender()->equals(sender2));
    CHECK(packet.getSource()->equals(source));
}

TEST(PacketTest, testIncreaseHopCount) {
    AddressPtr source (new AddressMock("source"));
    AddressPtr destination (new AddressMock("destination"));
    AddressPtr sender (new AddressMock("originalSender"));
    char type = PacketType::DATA;
    unsigned int seqNr = 1;
    const char* payload = "Hello World";
    unsigned int hopCount = 5;

    Packet packet = Packet(source, destination, sender, type, seqNr, payload, 11, hopCount);
    LONGS_EQUAL(hopCount, packet.getHopCount());

    packet.increaseHopCount();
    LONGS_EQUAL(hopCount+1, packet.getHopCount());

    packet.increaseHopCount();
    LONGS_EQUAL(hopCount+2, packet.getHopCount());
}

TEST(PacketTest, testCreateBANT) {
    AddressPtr originalSource (new AddressMock("source"));
    AddressPtr originalDestination (new AddressMock("destination"));
    AddressPtr originalSender (new AddressMock("sender"));
    unsigned int type = PacketType::FANT;
    int seqNr = 3;
    unsigned int hopCount = 123;

    Packet packet = Packet(originalSource, originalDestination, originalSender, type, seqNr, hopCount);
    unsigned int newSequenceNumber = 12345;
    Packet* bant = packet.createBANT(newSequenceNumber);

    CHECK(bant->getSource()->equals(originalDestination));
    CHECK(bant->getDestination()->equals(originalSource));
    // The sender of the BANT will be determined when it is actually send by the ARA client
    CHECK_EQUAL(PacketType::BANT, bant->getType());
    CHECK_EQUAL(newSequenceNumber, bant->getSequenceNumber());
    CHECK_EQUAL(0, bant->getPayloadLength());
    CHECK_EQUAL(0, bant->getHopCount());

    delete bant;
}

TEST(PacketTest, testGetHashValue) {
    AddressPtr address1 (new AddressMock("A"));
    AddressPtr address2 (new AddressMock("B"));
    Packet packet1 = Packet(address1, address2, PacketType::DATA, 1);
    Packet packet2 = Packet(address1, address2, PacketType::DATA, 2);
    Packet packet3 = Packet(address1, address2, PacketType::DATA, 3);
    Packet packet4 = Packet(address2, address1, PacketType::DATA, 1);
    Packet packet5 = Packet(address2, address1, PacketType::DATA, 1);

    CHECK(packet1.getHashValue() != packet2.getHashValue()); // same source - different seqNr
    CHECK(packet2.getHashValue() != packet3.getHashValue()); // same source - different seqNr
    CHECK(packet1.getHashValue() != packet4.getHashValue()); // different source - same seqNr
    CHECK(packet4.getHashValue() == packet5.getHashValue()); // same source - same seqNr
}

TEST(PacketTest, testCreateDulicatePacket) {
    AddressPtr originalSource (new AddressMock("source"));
    AddressPtr originalDestination (new AddressMock("destination"));
    AddressPtr originalSender (new AddressMock("sender"));
    unsigned int type = PacketType::DATA;
    unsigned int originalseqenceNumber = 3;
    unsigned int originalHopCount = 123;

    Packet packet = Packet(originalSource, originalDestination, originalSender, type, originalseqenceNumber, originalHopCount);
    Packet* duplicateWarning = packet.createDuplicateWarning();

    CHECK(duplicateWarning->getSource()->equals(originalSource));
    CHECK(duplicateWarning->getDestination()->equals(originalDestination));
    // The sender of the packet will be determined when it is actually send by the ARA client
    CHECK_EQUAL(PacketType::DUPLICATE_ERROR, duplicateWarning->getType());
    CHECK_EQUAL(originalseqenceNumber, duplicateWarning->getSequenceNumber());
    CHECK_EQUAL(0, duplicateWarning->getPayloadLength());
    CHECK_EQUAL(originalHopCount+1, duplicateWarning->getHopCount());

    delete duplicateWarning;
}
