project(SO-TP2 C)
add_executable(SO-TP2 Kernel/src Kernel/include Kernel/ Userland/SampleCodeModule/src Userland/SampleCodeModule/include Userland/SampleCodeModule/src/syscallTests.c Userland/SampleCodeModule/include/syscallTests.h Userland/SampleCodeModule/include/processes.h)
