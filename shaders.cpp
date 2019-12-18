//#include "cl2.hpp"
#include "shaders.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <CL/opencl.h>

namespace hez { namespace shaders {
	
	cl_device_id device_id;
	cl_context context; 
	cl_command_queue command_queue;
	
	void initialize() {
		cl_platform_id platforms;
		int result;
		cl_uint num_platforms, num_devices;
		result = clGetPlatformIDs(1, &platforms, &num_platforms);
		result = clGetDeviceIDs(platforms, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &num_devices);
		context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &result);
		command_queue = clCreateCommandQueue(context, device_id, 0, &result);
	}
	
	
	
	HEZ_CL_SOURCE loadSourceFromFile(char* fileName) {
		FILE *fp = fopen(fileName, "r");
		if (!fp) {
			fprintf(stderr, "SHADERS.CPP -> LoadSourceFromFile.\n");
			exit(1);
		}
		size_t source_size;
		fseek(fp, 0, SEEK_END);
	    source_size = ftell(fp);
	    rewind(fp);
		char* source_str = (char *)malloc(source_size);
		source_size = fread(source_str, 1, source_size, fp);
		fclose(fp);
		
    	HEZ_CL_SOURCE result;
    	result.size = source_size;
    	result.str = source_str;
    	return result;
	}
	
	cl_kernel createKernelFromSource(const char* kernel_name, HEZ_CL_SOURCE source) {
		int errcode_ret;
		cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source.str, (const size_t *)&source.size, &errcode_ret);
		errcode_ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
		cl_kernel kernel = clCreateKernel(program, kernel_name, &errcode_ret);
		return kernel;
	}
	
} }
