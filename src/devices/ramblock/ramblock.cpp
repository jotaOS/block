#include "ramblock.hpp"
#include <cstdio>
#include <shared_memory>

bool RAMBlock::read(std::SMID smid, size_t lba) {
	lock.acquire();
	std::smAllow(smid, pid);
	bool result = std::rpc(pid, std::ramblock::READ, smid, lba);
	lock.release();
	return result;
}

bool RAMBlock::write(std::SMID smid, size_t lba) {
	lock.acquire();
	std::smAllow(smid, pid);
	bool result = std::rpc(pid, std::ramblock::WRITE, smid, lba);
	lock.release();
	return result;
}



static UUIDmap ramblocks;
static std::mutex ramblocksLock;

std::UUID ramblock::add(std::PID pid) {
	auto ret = randomUUID();
	ramblocksLock.acquire();
	ramblocks[ret.a][ret.b] = (uint64_t)(new RAMBlock(pid));
	ramblocksLock.release();
	return ret;
}

bool ramblock::read(const std::UUID& uuid, std::SMID smid, size_t start) {
	ramblocksLock.acquire();
	auto u64 = ramblocks[uuid.a][uuid.b];
	ramblocksLock.release();

	if(!u64)
		return false;

	auto* ptr = (RAMBlock*)u64;
	return ptr->read(smid, start / PAGE_SIZE);
}

bool ramblock::write(const std::UUID& uuid, std::SMID smid, size_t start) {
	ramblocksLock.acquire();
	auto u64 = ramblocks[uuid.a][uuid.b];
	ramblocksLock.release();

	if(!u64)
		return false;

	auto* ptr = (RAMBlock*)u64;
	return ptr->write(smid, start / PAGE_SIZE);
}
