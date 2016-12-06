########MAKEFILE##########
PB_INC=../include/pb
PROTOC=../lib/protoc
FRAME_INC_PATH=../include
SO_PATH=/msec/p2p_cpp/MainLogic/bin/msec.so

DEBUG= -g

INCCOMM=-I$(FRAME_INC_PATH) -I$(PB_INC)  -I$(FRAME_INC_PATH)/spp_incl/ -I$(FRAME_INC_PATH)/sync_incl -I$(FRAME_INC_PATH)/internal $(FRAME_INC_PATH)/r3c -I$(FRAME_INC_PATH)/hiredis -I$(FRAME_INC_PATH)/mysqlclient -I$(FRAME_INC_PATH)/config
#LIBCOMM= -L../lib $(shell ls ../lib | egrep "^lib" | egrep -v "libmt.a|libnlbapi.a|libpb32.a|libpb64.a|librpc_clt_comm.a" | cut -b 4- | cut -d . -f 1 | awk '{print "-l"$$0}')
LIBCOMM= -L../lib $(shell ls ../lib | egrep "^lib" | egrep -v "libmt.a|libnlbapi.a|libpb32.a|libpb64.a|librpc_clt_comm.a|libhiredis.a|libmysqlclient.a|libr3c.a|libreadconf.a" | cut -b 4- | cut -d . -f 1 | awk '{print "-l"$$0}') -lmysqlclient -lr3c -lhiredis -lreadconf
BINARY = ../bin/Tesla.so

### rules
ALLPROTOBUF=$(wildcard ./protocal/*.proto)
ALLPROTOBUF_H=$(patsubst %.proto,%.pb.h,$(ALLPROTOBUF))
ALLPROTOBUF_OBJ:= $(patsubst %.cc,%.o,$(wildcard ./protocal/*.cc))
#ALLPROTOBUF_OBJ=$(patsubst %.proto,%.pb.o,$(ALLPROTOBUF))


EXCLUDE_DIRS := comm.tiny
DIRS := $(shell find . -maxdepth 2 -type d |egrep -v '\.git')
DIRS := $(basename $(patsubst ./%,%,$(DIRS)))
DIRS := $(filter-out $(EXCLUDE_DIRS),$(DIRS))

INCCOMM += $(foreach it, $(DIRS), -I./$(it))
SRCS := $(foreach it,$(DIRS),$(wildcard $(it)/*.cpp)) 
OBJS =$(patsubst %.cpp,%.o,$(SRCS)) $(ALLPROTOBUF_OBJ) 
#SRCS := $(foreach it,$(DIRS),$(wildcard $(it)/*.cc)) 
#OBJS +=$(patsubst %.cc,%.o,$(SRCS)) 

# Comment the following line if you are not using the gnu c compiler
C_ARGS = -Wall -g -fPIC -rdynamic

ifeq ($(ARCH),32)
	C_ARGS +=  -march=pentium4 -m32 -DSUS_LINUX -pthread
else
	C_ARGS +=  -m64 -DSUS_LINUX -pthread
endif

# You might have to change this if your c compiler is not cc
CC = g++

# You shouldn't need to make any more changes below this line.
all: proto $(BINARY) 

### shell color config
RED = \\e[1m\\e[31m
DARKRED = \\e[31m
GREEN = \\e[1m\\e[32m
DARKGREEN = \\e[32m
BLUE = \\e[1m\\e[34m
DARKBLUE = \\e[34m
YELLOW = \\e[1m\\e[33m
DARKYELLOW = \\e[33m
MAGENTA = \\e[1m\\e[35m
DARKMAGENTA = \\e[35m
CYAN = \\e[1m\\e[36m
DARKCYAN = \\e[36m
RESET = \\e[m
CRESET =  ;echo -ne \\e[m; test -s $@

%.o: %.cpp
	@echo -e Compiling $(GREEN)$<$(RESET) ...$(RED)
	@$(CC) $(C_ARGS) -c -o $@ $< $(INCCOMM) $(CRESET)

%.o: %.cc
	@echo -e Compiling $(GREEN)$<$(RESET) ...$(RED)
	@$(CC) $(C_ARGS) -c -o $@ $< $(INCCOMM) $(CRESET)

%.o: %.c
	@echo -e Compiling $(GREEN)$<$(RESET) ...$(RED)
	@$(CC) $(C_ARGS) -c -o $@ $< $(INCCOMM) $(CRESET)  

%.o:%.S
	@echo -e Compiling $(GREEN)$<$(RESET) ...$(RED)
	@$(CC) $(C_ARGS) -c -o $@ $< $(INCCOMM) $(CRESET) 

%.pb.h:%.proto
	@echo -e Generating $(BLUE)$@$(RESET)...
	@chmod a+x $(PROTOC)
	@$(PROTOC)  $<  --cpp_out=$(shell dirname $<)  --proto_path=$(shell dirname $<) \
	--proto_path=$(PB_INC) --proto_path=$(FRAME_INC_PATH)

%.pb.o:%.proto
	@echo -e Compiling $(GREEN)$<$(RESET) ...$(RED)
	@$(CC) $(C_ARGS) -c -o $@ ` echo $< | sed 's/.proto$$/.pb.cc/g' ` $(INCCOMM) $(CRESET)

clean:
	@#@echo "obj: $(OBJS) pb: $(ALLPROTOBUF) pbh: $(ALLPROTOBUF_H) pbobj: $(ALLPROTOBUF_OBJ)"
	rm -f $(BINARY) $(OBJS) 

proto: $(ALLPROTOBUF_H) 


$(BINARY): $(OBJS)
	@echo "src: $(SRCS)"
	@echo "obj: $(OBJS)"
	@echo "inc: $(INCCOMM)"
	@echo -e  Linking $(CYAN)$@$(RESET) ...$(RED)
	@-rm -f $@ 
	@$(CC) -o $@ $^ -shared -ldl $(C_ARGS) $(LIBCOMM) -Wl,-z,interpose -Wl,-rpath,../bin/lib $(CRESET)
	@chmod +x $@ 
	cp -f $(BINARY) $(SO_PATH)
