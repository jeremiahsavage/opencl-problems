#define CL_TARGET_OPENCL_VERSION 220
#define CL_HPP_TARGET_OPENCL_VERSION 200
#include<CL/cl2.hpp>
#include<iostream>
#include <fstream>

using namespace std;

int main()
{
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    cout << "Platforms:" << endl;
    for (vector<cl::Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it) {
      cout << "\t" << (*it).getInfo<CL_PLATFORM_NAME>() << endl;
    }
    cl::Platform platform = platforms.front();

    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

    cout << "Devices:" << endl;
    for (vector<cl::Device>::iterator itd = devices.begin(); itd != devices.end(); ++itd) {
      cout << "\t" << (*itd).getInfo<CL_DEVICE_NAME>() << endl;
    }
    cl::Device device = devices.front();

    std::ifstream helloWorldFile("hello.cl");
    std::string src(std::istreambuf_iterator<char>(helloWorldFile), (std::istreambuf_iterator<char>()));

    cl::Program::Sources sources;
    sources.push_back({src.c_str(), src.length() + 1});


    cl::Context context({device});
    cl::Program program(context, sources);

    auto err = program.build({device});

    char buf[16];
    cl::Buffer memBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(buf));
    cl::Kernel kernel(program, "HelloWorld", &err);
    kernel.setArg(0, memBuf);

    cl::CommandQueue queue(context, device);
    queue.enqueueTask(kernel);
    queue.enqueueReadBuffer(memBuf, CL_TRUE, 0, sizeof(buf), buf);

    std::cout << "hello";
    std::cin.get();

    return 0;
}
