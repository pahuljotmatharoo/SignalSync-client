#pragma once
#include <semaphore>
class SemaphoreLockGuard {
private:
	std::counting_semaphore<INT_MAX>& m_semaphore;
public:
	SemaphoreLockGuard(std::counting_semaphore<INT_MAX>& t_semaphore) : m_semaphore(t_semaphore) {
		m_semaphore.acquire();
	}
	~SemaphoreLockGuard() {
		m_semaphore.release();
	}
};

