#include "AHCI.hpp"
#include <rpc>
#include <shared_memory>

std::PID AHCI::ahci = 0;

bool AHCI::resolve() {
	if(ahci)
		return true;
	ahci = std::resolve("AHCI");
	return ahci;
}
