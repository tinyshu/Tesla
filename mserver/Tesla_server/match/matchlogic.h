/*
 * =====================================================================================
 * 
 *       Filename:  matchlogic.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  12/10/16 18:59:15 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  loganwu (linux), wuxiaolong63@gmail.com
 *        Company:  makedream
 * 
 * =====================================================================================
 */

#ifndef  MATCHLOGIC_INC
#define  MATCHLOGIC_INC
#include "matchdata.h"

class Cmatchlogic
{
public:
	Cmatchlogic(){}
	~Cmatchlogic(){}
	int startmatch(vmatchdata& vLmatchdata, vmatchdata& vRmatchdata );
	int sortpayorder(vmatchdata& rvmatchdata);	
	int sortgetorder(vmatchdata& rvmatchdata);	
	int createbill(vmatchdata& rvmatchdata,vbilldata& rvbilldata );
private:
	int createchildorder(matchdata& ldata, matchdata& rdata,int iMatchMoney);

};
#endif   /* ----- #ifndef MATCHLOGIC_INC  ----- */

