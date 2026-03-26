/*************************************************************/
/*
*  @file    shOCLManager.cpp
*  @author  MarcoEsparza <maeafinn14@gmail.com>
*  @date    2025/05/13
*  @brief
*
*
*
*  @bug     No bug known.
*/
/*************************************************************/

/*************************************************************/
/*
*  Includes
*/
/*************************************************************/
#include "shOCLManager.h"
//#include "CL/cl.h"
//#include "CL/cl.hpp"
//#include "shLogger.h"

namespace shEngineSDK {
//void
//OCLManager::onStartUp()
//{
//  Logger& logger = g_logger();
//
//  cl_platform_id platformID;
//  cl_uint numPlatforms;
//
//  cl_int errorCode = clGetPlatformIDs(0, nullptr, &numPlatforms);
//  if (errorCode != CL_SUCCESS) {
//    SH_ASSERT("Failed to get the number of platforms");
//  }
//
//  cl_platform_id* platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id) * numPlatforms);
//  errorCode = clGetPlatformIDs(numPlatforms, platforms, nullptr);
//  if (errorCode != CL_SUCCESS) {
//    SH_ASSERT("Failed to get the platforms");
//  }
//
//  for (cl_uint i = 0; i < numPlatforms; ++i) {
//    char platformName[128];
//    errorCode = clGetPlatformInfo(platforms[i],
//                                  CL_PLATFORM_NAME,
//                                  sizeof(platformName),
//                                  platformName,
//                                  nullptr);
//    if (errorCode != CL_SUCCESS) {
//      SH_ASSERT("Failed to get the platforms");
//    }
//  }
//
//  
//
//  //
//  /*cl::Platform platform;
//  Vector<cl::Platform> platforms;
//  cl::Device device;
//  cl::Context context;
//  cl::CommandQueue queue;
//
//  cl::Platform::get(&platforms);
//  platform = platforms[0];
//
//  Vector<cl::Device> devices;
//  platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
//  device = devices[0];
//
//  context = cl::Context({ device });
//  queue = cl::CommandQueue(context, device);
//
//  String kernelSource = R"CLC(
//  __kernel void histogram(
//    __global const uchar* datos,
//    __global uint* histoGlobal,
//    const uint numElementos)
//  {
//    const int gid = get_global_id(0);
//    const int lid = get_local_id(0);
//    const int lsize = get_local_size(0);
//
//    __local uint histoLocal[256];
//
//    for (int i = lid; i < 256; i += lsize)
//        histoLocal[i] = 0;
//    barrier(CLK_LOCAL_MEM_FENCE);
//
//    if (gid < numElementos) {
//        uchar valor = datos[gid];
//        atomic_inc(&histoLocal[valor]);
//    }
//    barrier(CLK_LOCAL_MEM_FENCE);
//
//    for (int i = lid; i < 256; i += lsize) {
//        atomic_add(&histoGlobal[i], histoLocal[i]);
//    }
//  }
//  )CLC";
//
//  cl::Program program(context, kernelSource);
//  try {
//    program.build({ device });
//  }
//  catch (...) {
//    String buildLog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
//    logger.Log(buildLog);
//  }
//
//  Vector<unchar> data = { '1','2','3' };
//  SIZE_T numData = data.size();
//  Vector<cl_uint> histogram(256, 0);
//
//  cl::Buffer bufData(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, numData, data.data());
//  cl::Buffer bufHistogram(context,
//                          CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
//                          sizeof(cl_uint) * 256,
//                          histogram.data());
//
//  cl::Kernel kernel(program, "histogram");
//  kernel.setArg(0, bufData);
//  kernel.setArg(1, bufHistogram);
//  kernel.setArg(2, static_cast<cl_uint>(numData));
//
//  size_t localSize = 64;
//  size_t globalSize = ((numData + localSize - 1) / localSize) * localSize;
//
//  queue.enqueueNDRangeKernel(kernel,
//                             cl::NullRange,
//                             cl::NDRange(globalSize),
//                             cl::NDRange(localSize));
//  queue.finish();
//
//  queue.enqueueReadBuffer(bufHistogram, CL_TRUE, 0, sizeof(cl_uint) * 256, histogram.data());
//
//  for (int i = 0; i < 256; ++i) {
//    std::cout << "Valor " << i << ": " << histogram[i] << '\n';
//  }*/
//}
}
