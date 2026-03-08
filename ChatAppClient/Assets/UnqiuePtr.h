#pragma once
// to unallow something at compile time, use function = delete
// do constexpr if statements & functions
template <typename T>
class UniquePtr {
private:
	T* m_ptr = nullptr;
public:
	UniquePtr() : m_ptr(new T) {}
	UniquePtr(T m_value) {
		if constexpr (std::is_same_v<T, UserMessage> || std::is_same_v<T, GroupMessage>) {
			m_ptr = new T(m_value.getName());
		} 
		else {
			m_ptr = new T(m_value);
		}
	}

	UniquePtr(UniquePtr&) = delete;
	UniquePtr& operator=(const UniquePtr*) = delete;

	UniquePtr(UniquePtr&& ptr) {
		m_ptr = ptr.m_ptr;
		ptr.m_ptr = nullptr;
	}

	UniquePtr& operator=(UniquePtr&& ptr) {
		delete m_ptr;
		m_ptr = ptr.m_ptr;
		ptr.m_ptr = nullptr;
		return *this;
	}

	~UniquePtr() {
		delete m_ptr;
	}

	UniquePtr& operator=(T value) {
		*(this->m_ptr) = value;
		return *this;
	}

	T& operator*() {
		return *(this->m_ptr);
	}

	T* operator->() {
		return this->m_ptr;
	}
};