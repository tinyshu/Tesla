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
#include <string>

struct billdata
{
	std::string strBillID;
	std::string strPayID;
	std::string strGetID;
	int iTime; //match time
	int iMatchMoney;
};

struct childorder
{
	std::string strPairOrderID;
	int iMatchMoney;
};

struct matchdata
{
	std::string strOrderID;
	uint8_t bVipLevel;
	int iMoney;
	int iTime;
	std::vector<childorder> vChildOrer;

	matchdata()
	{
		strOrderID = "";
		bVipLevel = 0;
		iMoney = 0;
		iTime = 0;
		//vChildOrer.clear();
	}
};
typedef std::vector<matchdata> vmatchdata;
typedef std::vector<billdata> vbilldata;
#endif   /* ----- #ifndef MATCHDATA_INC  ----- */

