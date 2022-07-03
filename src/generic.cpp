#include <common.hpp>
#include <devices/AHCI/AHCI.hpp>
#include <devices/ramblock/ramblock.hpp>

bool genericRead(const std::UUID& uuid, size_t type, uint8_t* data, size_t start, size_t sz) {
	switch(type) {
	case std::block::DeviceTypes::AHCIATAPI:
		return AHCI::readATAPI(uuid, data, start, sz);
	case std::block::DeviceTypes::RAMBLOCK:
		return ramblock::read(uuid, data, start, sz);
	default:
		return false;
	}
}

bool genericWrite(const std::UUID& uuid, size_t type, uint8_t* data, size_t start, size_t sz) {
	switch(type) {
	case std::block::DeviceTypes::RAMBLOCK:
		return ramblock::write(uuid, data, start, sz);
	default:
		return false;
	}
}
