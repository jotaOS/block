#ifndef RAMBLOCK_HPP
#define RAMBLOCK_HPP

#include <vector>
#include <UUID.hpp>
#include <userspace/ramblock.hpp>
#include <mutex>

class RAMBlock {
private:
	std::PID pid = 0;
	uint8_t* buffer = nullptr;
	bool ok = false;
	std::mutex lock;

public:
	RAMBlock(std::PID);
	bool read(uint8_t* data, size_t lba);
	bool write(uint8_t* data, size_t lba);
};

namespace ramblock {
	std::UUID add(std::PID pid);
	bool read(const std::UUID&, uint8_t* data, size_t start, size_t sz);
	bool write(const std::UUID&, uint8_t* data, size_t start, size_t sz);
}

#endif
