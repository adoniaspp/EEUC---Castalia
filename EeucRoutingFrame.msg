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

cplusplus {{
#include "RoutingPacket_m.h"
}}

class RoutingPacket;


enum EEUCRouting_FrameType
{
	EEUC_ROUTING_COMPETE_HEAD_FRAME	= 1;
	EEUC_ROUTING_FINALHEAD_FRAME 	= 2;		
	EEUC_ROUTING_QUITELECTION_FRAME	= 3;		
	EEUC_ROUTING_CH_ADV_FRAME	= 4;
	EEUC_ROUTING_CH_ADV_RELAY_FRAME	= 5;
	EEUC_ROUTING_JOIN_FRAME	= 6;
	EEUC_ROUTING_SCHE_FRAME = 7;
	EEUC_ROUTING_AGGR_FRAME = 8;
	EEUC_ROUTING_DATA_FRAME = 9;	
	MAX_CLUSTER_SIZE 		= 500;
};

packet EEUCRouting_DataFrame extends RoutingPacket
{	
	int EeucRoutingPacketKind enum (EEUCRouting_FrameType);
	int clusterLength;
	int schedule[MAX_CLUSTER_SIZE];		//Order of nodes in the TDMA schedule
	RoutingPacket aggregate[];	//Aggregated Data
	double energy;			
	double rcomp;
	double distanceBS;
	double posX;
	double posY;
};

