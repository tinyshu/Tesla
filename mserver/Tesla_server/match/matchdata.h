/*
 * =====================================================================================
 * 
 *       Filename:  matchdata.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  12/11/16 12:53:19 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), wuxiaolong63@gmail.com
 *        Company:  makedream
 * 
 * =====================================================================================
 */

#ifndef  MATCHDATA_INC
#define  MATCHDATA_INC
#include <stdint.h>
#include <vector>

struct childorder
{
	uint32_t uPairOrderID;
	int iMatchMoney;
};

struct matchdata
{
	uint32_t uOrderID;
	uint8_t bVipLevel;
	int iMoney;
	std::vector<childorder> vChildOrer;

	matchdata()
	{
		uOrderID = 0;
		bVipLevel = 0;
		iMoney = 0;
	}
};

#endif   /* ----- #ifndef MATCHDATA_INC  ----- */

