#include <cstdio>
#include <devices/AHCI/AHCI.hpp>
#include <common.hpp>

UUIDmap devices;
std::vector<std::UUID> uuids;

bool genericRead(const std::UUID& uuid, size_t type, uint8_t* data, size_t start, size_t sz) {
	switch(type) {
	case DeviceTypes::AHCIATAPI:
		return AHCI::readATAPI(uuid, data, start, sz);
	default:
		return false;
	}
}

extern "C" void _start() {
	for(auto const& x : AHCI::probeATAPI()) {
		devices[x.a][x.b] = DeviceTypes::AHCIATAPI;
		uuids.push_back(x);
	}

	exportProcedures();
	std::enableRPC();
	std::publish("block");
	std::halt();
}
