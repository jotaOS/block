#include "AHCI.hpp"
#include <rpc>
#include <shared_memory>

std::PID AHCI::ahci = 0;
char* AHCI::buffer = nullptr;

bool AHCI::resolve() {
	if(ahci)
		return true;
	ahci = std::resolve("AHCI");
	return ahci;
}

bool AHCI::connect() {
	if(!resolve())
		return false;
	if(buffer)
		return true;

	std::SMID smid = std::smMake();
	buffer = (char*)std::smMap(smid);
	std::smAllow(smid, ahci);
	return std::rpc(ahci, std::AHCI::CONNECT, smid);
}
