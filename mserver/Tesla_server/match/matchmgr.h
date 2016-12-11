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

class Cmatchmgr
{
public:
	Cmatchmgr(){}
	~Cmatchmgr(){}
	int loadmatchdata();
private:
	matchdata m_stmatchdata;
};

#endif   /* ----- #ifndef MATCHMGR_INC  ----- */

