// Copyright � 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS2.
* @copyright    Copyright � 2018-2019 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>
#include <CL/cl.hpp>
using namespace ::ul2;


int
main()
{
    FBlock block( 256, 256, ULIS2_FORMAT_ARGBF );
    for( int i = 0; i < block.SamplesPerPixel(); ++i )
        std::cout << (int)block.RedirectedIndex( i ) << std::endl;
    FBlock block2( 256, 256, ULIS2_FORMAT_ARGBF );
    FPixel color( ULIS2_FORMAT_LabAF, { 0.5, 0.2, 0.7, 1.0 } );
    FPixel rgb( ULIS2_FORMAT_RGB8,    { 255, 81, 255 }      );
    FPixel lab( 0, UEncodeLabA( 100, 64, -20, 1.0 ) );
    Conv( rgb, lab );
    FThreadPool pool;
    FPerf perf( Perf_Best_CPU & Perf_SSE4_2 );
    ClearRaw( &block );
    CopyRaw( &block2, &block );
    Clear( pool, &block2 );
    Copy( pool, &block2, &block );
    Swap( pool, &block2, 0, 2 );
    Fill( pool, &block, color, perf );
    FPerf perf2( Perf_Best_CPU );
    Blend( pool, &block, &block2, FPoint(), BM_NORMAL, AM_NORMAL, 1.f, perf2, true );
    FPixelProxy prox = block.PixelProxy( 240, 0 );
    std::cout << (float)prox.RF() << std::endl;
    std::cout << (float)prox.GF() << std::endl;
    std::cout << (float)prox.BF() << std::endl;
    std::cout << (float)prox.AF() << std::endl;
    auto dummy = 0;

    // OpenCL
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

     //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout<< "Num devices: "<<all_devices.size()<<"\n";
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

    cl::Context context({default_device});
    cl::Program::Sources sources;

    // kernel calculates for each element C=A+B
    std::string kernel_code =
            "   void kernel simple_add(global const int* A, global const int* B, global int* C){    "
            "       C[get_global_id(0)]=A[get_global_id(0)]+B[get_global_id(0)];                    "
            "   }                                                                                   ";
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }

    // create buffers on the device
    cl::Buffer buffer_A( context,CL_MEM_READ_WRITE,sizeof(int)*10 );
    cl::Buffer buffer_B( context,CL_MEM_READ_WRITE,sizeof(int)*10 );
    cl::Buffer buffer_C( context,CL_MEM_READ_WRITE,sizeof(int)*10 );

    int A[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int B[] = {0, 1, 2, 0, 1, 2, 0, 1, 2, 0};

    //create queue to which we will push commands for the device.
    cl::CommandQueue queue(context,default_device);
    //write arrays A and B to the device
    queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(int)*10,A);
    queue.enqueueWriteBuffer(buffer_B,CL_TRUE,0,sizeof(int)*10,B);

    cl::make_kernel< cl::Buffer, cl::Buffer, cl::Buffer > simple_add( cl::Kernel( program, "simple_add" ) );
    cl::EnqueueArgs eargs(queue, cl::NullRange, cl::NDRange(10), cl::NullRange);
    simple_add(eargs, buffer_A, buffer_B, buffer_C).wait();

    int C[10];
    //read result C from the device to array C
    queue.enqueueReadBuffer(buffer_C,CL_TRUE,0,sizeof(int)*10,C);

    std::cout<<" result: \n";
    for(int i=0;i<10;i++){
        std::cout<<C[i]<< " " << std::endl;
    }

    cl_int err;
    cl::size_t< 3 > origin;
    cl::size_t< 3 > region;
    region[0] = 256;
    region[1] = 256;
    cl::Image2D resource( context, CL_MEM_USE_HOST_PTR, { CL_RGBA, CL_FLOAT }, 256, 256, 0, block.DataPtr(), &err );
    std::cout << err << std::endl;
    std::cout << resource.getInfo< CL_MEM_SIZE >( &err ) << std::endl;
    std::cout << err << std::endl;
    auto _dummy = 0;

    return  0;
}
