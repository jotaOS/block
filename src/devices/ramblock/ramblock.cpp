#include "ramblock.hpp"
#include <cstdio>
#include <shared_memory>

RAMBlock::RAMBlock(std::PID pid_) {
	pid = pid_;

	std::SMID smid = std::smMake();
	buffer = (uint8_t*)std::smMap(smid);
	std::smAllow(smid, pid);
	ok = std::rpc(pid, std::ramblock::CONNECT, smid);
}

bool RAMBlock::read(uint8_t* data, size_t lba) {
	lock.acquire();
	if(!ok) {
		lock.release();
		return false;
	}

	if(!std::rpc(pid, std::ramblock::READ, lba)) {
		lock.release();
		return false;
	}

	memcpy(data, buffer, PAGE_SIZE);
	lock.release();
	return true;
}

bool RAMBlock::write(uint8_t* data, size_t lba) {
	lock.acquire();
	if(!ok) {
		lock.release();
		return false;
	}

	memcpy(buffer, data, PAGE_SIZE);

	bool result = std::rpc(pid, std::ramblock::WRITE, lba);
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

bool ramblock::read(const std::UUID& uuid, uint8_t* data, size_t start, size_t sz) {
	ramblocksLock.acquire();
	auto u64 = ramblocks[uuid.a][uuid.b];
	ramblocksLock.release();

	if(!u64)
		return false;

	if(sz != PAGE_SIZE) {
		std::printf("[block] Error: ramblock read size is not PAGE_SIZE\n");
		return false;
	}

	auto* ptr = (RAMBlock*)u64;
	return ptr->read(data, start / PAGE_SIZE);
}

bool ramblock::write(const std::UUID& uuid, uint8_t* data, size_t start, size_t sz) {
	ramblocksLock.acquire();
	auto u64 = ramblocks[uuid.a][uuid.b];
	ramblocksLock.release();

	if(!u64)
		return false;

	if(sz != PAGE_SIZE) {
		std::printf("[block] Error: ramblock read size is not PAGE_SIZE\n");
		return false;
	}

	auto* ptr = (RAMBlock*)u64;
	return ptr->write(data, start / PAGE_SIZE);
}
