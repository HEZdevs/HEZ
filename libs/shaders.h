#ifndef HEZ_SHADERS_H
#define HEZ_SHADERS_H

#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_CL_1_2_DEFAULT_BUILD 
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

//#include "cl2.hpp"
#include <CL/opencl.h>

namespace hez { namespace shaders {
	extern cl_device_id device_id;
	extern cl_context context; 
	extern cl_command_queue command_queue;
	struct HEZ_CL_SOURCE {
		size_t size;
		char* str;
	};
	void initialize();
	HEZ_CL_SOURCE loadSourceFromFile(char* fileName);
	cl_kernel createKernelFromSource(const char* kernel_name, HEZ_CL_SOURCE source);
} }

#endif
