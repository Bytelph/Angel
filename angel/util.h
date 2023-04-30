#ifndef __ANGEL__UTIL_H__
#define __ANGEL__UTIL_H__

#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<stdio.h>
#include<stdint.h>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

#include <yaml-cpp/yaml.h>

namespace angel
{
pid_t GetThreadId();
uint32_t GetFiberId();

template<class T>
const char* TypeToName() {
    static const char* s_name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);
    return s_name;
}
}

#endif
