#include <cstdio>
#include <devices/AHCI/AHCI.hpp>
#include <devices/ramblock/ramblock.hpp>
#include <common.hpp>

UUIDmap deviceTypes;
std::vector<std::UUID> uuids;

extern "C" void _start() {
	for(auto const& x : AHCI::probeATAPI()) {
		deviceTypes[x.a][x.b] = std::block::DeviceTypes::AHCIATAPI;
		uuids.push_back(x);
	}

	std::PID ram = std::resolve("ramblock");
	if(!ram) {
		std::printf("[block] Error: could not resolve ramblock\n");
		std::exit(99);
	}
	auto ramuuid = ramblock::add(ram);
	deviceTypes[ramuuid.a][ramuuid.b] = std::block::DeviceTypes::RAMBLOCK;
	uuids.push_back(ramuuid);

	exportProcedures();
	std::enableRPC();
	std::publish("block");
	std::halt();
}
