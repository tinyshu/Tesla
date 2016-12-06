#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#include <stdint.h>

#ifndef _BEGIN_DO_WHILE_FLASE_
#define _BEGIN_DO_WHILE_FLASE_  do{
#define _END_DO_WHILE_FALSE_    }while(false);
#endif


typedef int64_t LONG;
typedef uint64_t ULONG;

typedef std::map<std::string, std::string>	CStr2Map;
typedef std::set<std::string> CStrSet;

#endif