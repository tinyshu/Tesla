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
	}
	return 0;
}
