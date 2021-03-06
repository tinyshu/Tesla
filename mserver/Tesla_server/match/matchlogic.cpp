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
#include "srpcincl.h"
#include <algorithm>
#include <stdio.h>

bool payorderpriority(matchdata ldata, matchdata rdata)
{
	if(ldata.bVipLevel > rdata.bVipLevel) //大师优先
		return true;
	else if(ldata.bVipLevel < rdata.bVipLevel)
		return false;
	else if(ldata.iTime < rdata.iTime) // 时间先后
		return true;
	
	return false;
}

bool getorderpriority(matchdata ldata, matchdata rdata)
{
	if(ldata.iTime < rdata.iTime) //时间先后
		return true;
	else if(ldata.iTime >  rdata.iTime) //时间先后
		return false;
	else if(ldata.bVipLevel < rdata.bVipLevel) //初级优先
		return true;
	
	return false;
}

class vector_finder_equal
{
	public:
		vector_finder_equal(const int iM):m_iMoney(iM){}
		bool operator ()(const std::vector<matchdata>::value_type &value)
		{
			return value.iMoney == m_iMoney;
		}
	private:
		int m_iMoney;                    
};


class vector_finder_greater
{
	public:
		vector_finder_greater(const int iM):m_iMoney(iM){}
		bool operator ()(const std::vector<matchdata>::value_type &value)
		{
			return value.iMoney >  m_iMoney;
		}
	private:
		int m_iMoney;                    
};

//遍历left,在right中寻找匹配对象
int Cmatchlogic::startmatch(vmatchdata& vLmatchdata, vmatchdata& vRmatchdata)
{
	for(vmatchdata::iterator it = vLmatchdata.begin(); it != vLmatchdata.end();)
	{
		matchdata& ldata = *it;	
		vmatchdata::iterator itfind =  std::find_if(vRmatchdata.begin(),vRmatchdata.end(),vector_finder_equal(ldata.iMoney));	
		if(itfind == vRmatchdata.end())//not match
		{
			//scan secondly,find one entry >0
			itfind =  std::find_if(vRmatchdata.begin(),vRmatchdata.end(),vector_finder_greater(0));	
			if(itfind == vRmatchdata.end())//所有Pay订单都匹配完成，Get订单还剩余
			{
				//ALARM TODO
				ATTR_REPORT("Match_failed_lackofmoney");
				NGLOG_ERROR("%s: match failed: lorderID %s money %d ",__FUNCTION__,ldata.strOrderID.c_str(),ldata.iMoney);
				return -1;
			}
		}

		matchdata& rdata = *itfind;
		int iMatchMoney = (ldata.iMoney>rdata.iMoney)?rdata.iMoney : ldata.iMoney;
		NGLOG_INFO("%s: match one: lorderID %s rorderID %s money %d ",__FUNCTION__,ldata.strOrderID.c_str(),rdata.strOrderID.c_str(),iMatchMoney);
		if(ldata.iMoney <= iMatchMoney) //这个GET单已经匹配完全
		{
			++it;
			//NGLOG_INFO("%s: next item  lorderID %s ",__FUNCTION__,it->strOrderID.c_str());
		}
		createchildorder(ldata,rdata,iMatchMoney);

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

int Cmatchlogic::createchildorder(matchdata& ldata, matchdata& rdata,int iMatchMoney)
{
	childorder stchildorder;
	stchildorder.iMatchMoney = iMatchMoney;
	
	stchildorder.strPairOrderID = rdata.strOrderID;
	ldata.iMoney -= iMatchMoney;
	ldata.vChildOrer.push_back(stchildorder);
	
	stchildorder.strPairOrderID = ldata.strOrderID;
	rdata.iMoney -= iMatchMoney;
	rdata.vChildOrer.push_back(stchildorder);

	return 0;
}

int Cmatchlogic::createbill(vmatchdata& rvmatchdata, vbilldata& rvbilldata)
{
	int iSeq = 0;
	for(vmatchdata::iterator it = rvmatchdata.begin(); it != rvmatchdata.end();++it)
	{
		for(std::vector<childorder>::iterator itchild = it->vChildOrer.begin(); itchild != it->vChildOrer.end();++itchild)
		{
			//M(1)+(保留)10+setID(2)+time(11)+sequence(8)
			int iTime = time(NULL);
			char cBillId[33];
			snprintf(cBillId,33,"M012345678900%011d%08d",iTime,iSeq++);
			//stringstream strStream;
			//strStream<<"M012345678900"<<iTime<<iSeq++;
			billdata stbilldata;
			stbilldata.strBillID= cBillId;
			stbilldata.strPayID = itchild->strPairOrderID;
			stbilldata.strGetID = it->strOrderID;
			stbilldata.iTime = iTime;
			stbilldata.iMatchMoney = itchild->iMatchMoney;
			rvbilldata.push_back(stbilldata);
		}
	}

	return 0;
}

