/*
 * =====================================================================================
 *
 *       Filename:  matchlogic.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/10/16 18:59:29 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  loganwu (linux), wuxiaolong63@gmail.com
 *        Company:  makedream
 *
 * =====================================================================================
 */

#include "matchlogic.h"
#include <algorithm>

bool payorderpriority(matchdata ldata, matchdata rdata)
{
	if(ldata.bVipLevel > rdata.bVipLevel) //大师优先
		return true;
	else if(ldata.iTime < rdata.iTime) // 时间先后
		return true;
	
	return false;
}

bool getorderpriority(matchdata ldata, matchdata rdata)
{
	if(ldata.iTime < rdata.iTime) //时间先后
		return true;
	else if(ldata.bVipLevel < rdata.bVipLevel) //初级优先
		return true;
	
	return false;
}

int Cmatchlogic::startmatch(vmatchdata& vLmatchdata, vmatchdata& vRmatchdata)
{
	for(vmatchdata::iterator it = vLmatchdata.begin(); it != vLmatchdata.end();++it)
	{
	
	}
	return 0;
}

int Cmatchlogic::sortpayorder(vmatchdata& rvmatchdata)	
{
	std::sort(rvmatchdata.begin(),rvmatchdata.end(),payorderpriority);	
	return 0;
}
int Cmatchlogic::sortgetorder(vmatchdata& rvmatchdata)	
{
	std::sort(rvmatchdata.begin(),rvmatchdata.end(),getorderpriority);	
	return 0;
}
