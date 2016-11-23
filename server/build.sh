#!/bin/sh
echo "BUILD_ROOT_PATH=`pwd`" > ./BUILD_ROOT

cpu_num=$(cat /proc/cpuinfo | grep processor | wc -l)
BUILD_LOG=Build.log

function MakeServer()
{
	echo "make svr parameter $@, para1 $1"
    if [ "debug" == $1 ];then
        echo "incremental-build project mode(DEBUG) [$2] at `date +"%Y-%m-%d %H:%M:%S"`" | tee -a $BUILD_LOG
		make -j ${cpu_num} -C  src
	
    elif [ "debugclean" == $1 ];then
        echo "debugclean project mode(CLEAN) [all] at `date +"%Y-%m-%d %H:%M:%S"`" | tee -a $BUILD_LOG
		make -C src clean
	fi

	return 1;
}

function MakeProto()
{
	echo "make proto parameter $@, para1 $1"
}

usage(){
    echo
    echo "Usage:"
    echo "  build.sh [command],commands are:"
    echo "  proto                  proto    "
    echo "  debug                  debug   " 
    echo "  debugclean             debugclean"
    echo
}

case $1 in
    proto)
       MakeProto $@
    ;;
    debug)
        MakeServer $@
    ;;
    debugclean)
        MakeServer $@
    ;;
    *)
        usage
esac
