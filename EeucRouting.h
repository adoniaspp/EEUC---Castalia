//****************************************************************************
//*  Copyright (c) Federal University of Para, brazil - 2011                 *
//*  Developed at the Research Group on Computer Network and Multimedia      *
//*  Communication (GERCOM)     				                             *
//*  All rights reserved    				                                 *
//*                                                                          *
//*  Permission to use, copy, modify, and distribute this protocol and its   *
//*  documentation for any purpose, without fee, and without written         *
//*  agreement is hereby granted, provided that the above copyright notice,  *
//*  and the author appear in all copies of this protocol.                   *
//*                                                                          *
//*  Module:   An energy-efficient unequal clustering mechanism for 		 *
//*  wireless sensor networks.                                               *
//*  Version:  0.1                                                           *
//*  Author(s): Adonias Pires <adonias@ufpa.br>                              *
//*             Claudio Silva <claudio.silva@itec.ufpa.br>                   *
//****************************************************************************/

#ifndef _EEUCROUTING_H_
#define _EEUCROUTING_H_

#include <queue>
#include <vector>
#include <omnetpp.h>
#include <algorithm>
#include <pthread.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "VirtualRouting.h"
#include "EeucRoutingFrame_m.h"
#include "ApplicationPacket_m.h"
#include "fuzzy.h"
#include "membershipFunction.h"
#include "NoMobilityManager.h"


using namespace std;

enum EeacRoutingTimers {
	NETWORK_SELF_NEW_ROUND = 1,	
	NETWORK_BE_TENTATIVE_HEAD = 2,
	NETWORK_FINAL_HEAD_MSG = 3,
	NETWORK_WAIT_ADV_CH = 4,
	NETWORK_SEND_ADV_CH = 5,
	NETWORK_FORM_TIMEOUT = 6,
	NETWORK_SCHE_TIMEOUT = 7,
	NETWORK_SEND_ADV_RELAY_CH = 8,	
	NETWORK_SEND_SLOT_CH = 9,
	NETWORK_SEND_ENDS_CH = 10,
	TIME_SERVICE = 11,
};

struct CHRec
{
	int id;
	double rssi;	//RETIRAR DEPOIS
	double energy;
	double rComp;
	double posX;
	double poxY;
	double distance;
};

class EeucRouting : public VirtualRouting {
 
 private:
		
	/*--- The .ned file's parameters ---*/
	double probability;
	double roundLength;
	bool isSink;
	int CHReportPower;
	int frameNumber;
	double rMax;
	double cRange;
	double tdMax;
	double kRcomp;
	double slotLength;
	/*--- Custom class parameters ---*/
	double energyTemp;
	NoMobilityManager *mobMgrModule;
	double delaySetupPhase;
	int clusterLength;
	int schedule[MAX_CLUSTER_SIZE];
	bool isCH;
	bool inSlot;
	bool beTentative;	
	vector <CHRec> candidates;
	vector <CHRec> candidatesCH;
	vector <int> children;
	vector <CHRec> replyCH;
	vector<RoutingPacket> bufferAggregate;
	double dMax;
	double dMin;
	double dBs;
	double rangeComp;
	NodeLocation_type *bsLocation;
	bool endFormClus;
	int relayNode;
	queue< cPacket* > TempTXBuffer;
	/*--- The Statistic file's parameters ---*/
	double periodicEnergyCalculationInterval;
	int timeOfGetStats;
	bool EnergyTime;
	bool EnergyMap;
	bool EnergyDissipation;	

 protected:

	/*--- Simulator Functions ---*/
	void startup();
	void fromApplicationLayer(cPacket *, const char *);
	void fromMacLayer(cPacket *, int, double, double);	
	void timerFiredCallback(int);
	void processBufferedPacket();
	/*--- Helper Functions ---*/
	void sendAggregate();
	double distanceTake( double x1, double y1, double x2, double y2);
	/*--- Radio Control Functions ---*/	
	void setRadio_TXPowerLevel(int parLevel, double delay=0.0);
	void setRadio_Sleep(double delay=0.0);
	void setRadio_Listen(double delay=0.0);
	void levelTxPower(int);

};
bool cmpECH_distance(CHRec a, CHRec b);

#endif
