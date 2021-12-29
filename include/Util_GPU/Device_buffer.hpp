#ifndef SGM_DEVICE_BUFFER_HPP
#define SGM_DEVICE_BUFFER_HPP

#include <cstddef>
#include "internal.h"

namespace util {

template <typename T>
class DeviceBuffer {

public:
	using value_type = T;

private:
	value_type *m_data;
	size_t m_size;

public:
	DeviceBuffer()
		: m_data(nullptr)
		, m_size(0)
	{ }

	explicit DeviceBuffer(size_t n)
		: m_data(nullptr)
		, m_size(0)
	{
		allocate(n);
		fillZero();
	}

	DeviceBuffer(const DeviceBuffer&) = delete;

	DeviceBuffer(DeviceBuffer&& obj)
		: m_data(obj.m_data)
		, m_size(obj.m_size)
	{
		obj.m_data = nullptr;
		obj.m_size = 0;
	}

	~DeviceBuffer(){
		destroy();
	}


	void allocate(size_t n){
		if(m_data && m_size >= n)
			return;

		destroy();
		CudaSafeCall(cudaMalloc(reinterpret_cast<void **>(&m_data), sizeof(value_type) * n));
		m_size = n;
	}

	void destroy(){
		if(m_data)
			CudaSafeCall(cudaFree(m_data));

		m_data = nullptr;
		m_size = 0;
	}

	void fillZero(){
		CudaSafeCall(cudaMemset(m_data, 0, sizeof(value_type) * m_size));
	}

	DeviceBuffer& operator=(const DeviceBuffer&) = delete;

	DeviceBuffer& operator=(DeviceBuffer&& obj){
		m_data = obj.m_data;
		m_size = obj.m_size;
		obj.m_data = nullptr;
		obj.m_size = 0;
		return *this;
	}


	size_t size() const {
		return m_size;
	}

	const value_type *data() const {
		return m_data;
	}

	value_type *data(){
		return m_data;
	}

};

}

#endif
