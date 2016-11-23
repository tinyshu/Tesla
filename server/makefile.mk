#####################################################
PLATFORM=linux

EXT_LIB_ROOT=${BUILD_ROOT_PATH}/extlib
#include begin
INC_MYSQL = -I$(EXT_LIB_ROOT)/mysql/include
#INC_LIBEVENT= -I$(EXT_LIB_ROOT)/libevent/include/
#INC_PROBUFEVENT= -I$(EXT_LIB_ROOT)/protobuf/include/
INC_JCE = -I$(EXT_LIB_ROOT)/jce
INC_SPP = -I$(EXT_LIB_ROOT)/spp3.0.1/include -I$(EXT_LIB_ROOT)/spp3.0.1/include/spp_incl -I$(EXT_LIB_ROOT)/spp3.0.1/include/sync_incl

INC_EXT =  $(INC_LIBEVENT) $(INC_PROBUFEVENT) $(INC_JCE) $(INC_SPP)
#include end

#lib begin
LIB_MYSQL = -L$(EXT_LIB_ROOT)/mysql/lib -lmysqlclient -lz
#ifeq ($(PLATFORM),macosx)
#	LIB_EVENT= -L$(EXT_LIB_ROOT)/libevent/lib/ -levent 
#else #linux
#	LIB_EVENT= -L$(EXT_LIB_ROOT)/libevent/lib/ -levent -lrt #linux libevent depends on librt
#endif
#LIB_PROBUF= -L$(EXT_LIB_ROOT)/protobuf/lib/ -lprotobuf 

LIB_SPP = -L$(EXT_LIB_ROOT)/spp3.0.1/lib -lasync_epoll -lsppasync -lmt -lmtspp
LIB_EXT = $(LIB_EVENT) $(LIB_PROBUF) $(LIB_SPP)
#lib end

CC              = g++
CXX             = g++ 
RANLIB          = ranlib
AR              = ar

#debug with generate call graph profile data
CFLAGS = -ggdb -DDEBUG -Wall -Wextra -pipe 

#release 
#CFLAGS += -g -Wall -Wextra -O3 -pipe 

#####################################################

