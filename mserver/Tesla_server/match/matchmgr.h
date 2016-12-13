/*
 * =====================================================================================
 * 
 *       Filename:  matchmgr.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  12/11/16 12:41:44 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), wuxiaolong63@gmail.com
 *        Company:  makedream
 * 
 * =====================================================================================
 */

#ifndef  MATCHMGR_INC
#define  MATCHMGR_INC
#include "matchdata.h"
#include "matchlogic.h"

class Cmatchmgr
{
public:
	Cmatchmgr(){}
	~Cmatchmgr(){}
	int loadmatchdata();
	int dumpmatchdata(vmatchdata& vdata);
	int startmatch();
private:
	vmatchdata  m_vpayorder;
	vmatchdata  m_vgetorder;
	vbilldata m_vbilldata;
	Cmatchlogic m_matchlogic;
};

#endif   /* ----- #ifndef MATCHMGR_INC  ----- */

