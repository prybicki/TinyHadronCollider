#include <ResourceManager.hpp>

#include <internal/macros.hpp>

#include <cuda_runtime_api.h>
#include <ResourcePrimitives.hpp>

ResourceManager& rm = ResourceManager::instance();

void ResourceManager::run(ThreadsLayout threads, void *kernel, void **args)
{
	CHECK_CUDA(cudaLaunchKernel(kernel, threads.gridDim, threads.blockDim, args, 0, 0));
	CHECK_CUDA(cudaStreamSynchronize(nullptr));
}

ResourceManager &ResourceManager::instance()
{
	static ResourceManager instance;
	return instance;
}

ThreadsLayout::ThreadsLayout(count_t launchDim, count_t blockDim)
: gridDim(1U + launchDim / blockDim)
, blockDim(static_cast<unsigned>(blockDim))
{}

memory_t ResourceManager::memoryAllocate(ValueType valueType, count_t elements)
{
	void* ptr = nullptr;
	CHECK_CUDA(cudaMalloc(&ptr, valueType.getElementSize() * elements));
	return std::shared_ptr<DeviceMemory>(new DeviceMemory(ptr, elements, valueType));
}