/*
 * =====================================================================================
 *
 *       Filename:  matchmgr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/11/16 12:41:49 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  loganwu (linux), wuxiaolong63@gmail.com
 *        Company:  makedream
 *
 * =====================================================================================
 */

#include "matchmgr.h"
#include <stdlib.h>
#include <sstream>
//#include "sppincl.h"
#include "srpcincl.h"

using namespace std;


int Cmatchmgr::loadmatchdata()
{
	//for test
	for(int i=0;i<10;i++)
	{
		matchdata stmatchdata;
		stringstream strStream;
		strStream<<"P"<<i;
		stmatchdata.strOrderID= strStream.str();
		stmatchdata.bVipLevel = random()%3;
		stmatchdata.iMoney = 100*(random()%20+1);
		stmatchdata.iTime = random()%100;
		m_vpayorder.push_back(stmatchdata);

		stringstream strStream1;
		strStream1<<"G"<<(i+100);
		stmatchdata.strOrderID= strStream1.str();
		stmatchdata.bVipLevel = random()%3;
		stmatchdata.iMoney = 100*(random()%20+1);
		stmatchdata.iTime = random()%100;
		m_vgetorder.push_back(stmatchdata);
	}
	
	NGLOG_INFO("%s: load data ",__FUNCTION__);
	dumpmatchdata(m_vpayorder);
	dumpmatchdata(m_vgetorder);

	m_matchlogic.sortpayorder(m_vpayorder);
	m_matchlogic.sortgetorder(m_vgetorder);
	NGLOG_INFO("%s: After sort",__FUNCTION__);
	dumpmatchdata(m_vpayorder);
	dumpmatchdata(m_vgetorder);

	return 0;

}

int Cmatchmgr::dumpmatchdata(vmatchdata& vdata)
{
	
	for(vmatchdata::iterator it = vdata.begin(); it != vdata.end();++it)
	{
		NGLOG_INFO("%s: orderID: %s vip %d time %d money %d",__FUNCTION__,
				it->strOrderID.c_str(),it->bVipLevel,it->iTime,it->iMoney);
		for(std::vector<childorder>::iterator itchild = it->vChildOrer.begin(); itchild != it->vChildOrer.end();++itchild)
		{
			//NGLOG_INFO("%s: billID %s payID %s GetID %s  matchtime %d money %d",__FUNCTION__,itchild->strBillID.c_str(),
			//		itchild->strPayID.c_str(),itchild->strGetID.c_str(),itchild->iTime,itchild->iMatchMoney);
			NGLOG_INFO("%s: PairorderID: %s matchmoney %d",__FUNCTION__,itchild->strPairOrderID.c_str(),itchild->iMatchMoney);
		}
		NGLOG_INFO("%s:**************************************",__FUNCTION__);
	}
	return 0;
}
 int Cmatchmgr::startmatch()
{
	int iRet = m_matchlogic.startmatch(m_vgetorder,m_vpayorder);
	NGLOG_INFO("%s: After match ret %d",__FUNCTION__,iRet);
	dumpmatchdata(m_vpayorder);
	dumpmatchdata(m_vgetorder);
	if(0 != iRet)
	{
		//TODO
	}
	iRet = m_matchlogic.createbill(m_vgetorder,m_vbilldata);
	
	iRet = dumpbilldata(m_vbilldata);

	return iRet;
}

int Cmatchmgr::dumpbilldata(vbilldata& vdata)
{
	for(vbilldata::iterator it = vdata.begin(); it != vdata.end();++it)
	{
		NGLOG_INFO("%s: orderID: %s vip %d time %d money %d",__FUNCTION__,
				it->strBillID.c_str(),it->strPayID.c_str(),it->strGetID.c_str(),it->iTime,it->iMatchMoney);

	}
	return 0;
}
	
