#ifndef RAMBLOCK_HPP
#define RAMBLOCK_HPP

#include <vector>
#include <UUID.hpp>
#include <userspace/ramblock.hpp>
#include <mutex>

class RAMBlock {
private:
	std::PID pid = 0;
	std::mutex lock;

public:
	RAMBlock(std::PID pid) : pid(pid) {}
	bool read(std::SMID smid, size_t lba);
	bool write(std::SMID smid, size_t lba);
};

namespace ramblock {
	std::UUID add(std::PID pid);
	bool read(const std::UUID&, std::SMID smid, size_t start);
	bool write(const std::UUID&, std::SMID smid, size_t start);
}

#endif
