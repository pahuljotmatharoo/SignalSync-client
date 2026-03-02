#pragma once
#include <semaphore>
class LockGuard {
private:
	std::binary_semaphore& m_semaphore;
public:
	LockGuard(std::binary_semaphore& t_semaphore) : m_semaphore(t_semaphore){
		m_semaphore.acquire();
	}
	~LockGuard() {
		m_semaphore.release();
	}
};