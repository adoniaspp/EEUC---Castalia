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

#include "EeucRouting.h"

Define_Module(EeucRouting);

#define MOTES_BOOT_TIMEOUT 0.05
#define WAIT_FOR_JOIN 1
#define SINK 0

void EeucRouting::startup()
{	

	/*--- The .ned file's parameters ---*/
	probability = par("probability");
	roundLength = par("roundLength");
	CHReportPower = par("CHReportPower");
	isSink = par("isSink");
	periodicEnergyCalculationInterval = par("periodicEnergyCalculationInterval");
	rMax = par("rMax");
	cRange = par("cRange");
	tdMax = par("tdMax");
	kRcomp = par("kRcomp");	
	slotLength = par("slotLength");

	/*--- The Statistic file's parameters ---*/
	EnergyTime = par("EnergyTime");
	EnergyMap = par("EnergyMap");
	EnergyDissipation = par("EnergyDissipation");

	/*--- Custom class parameters ---*/
	mobMgrModule = check_and_cast <NoMobilityManager*>(getParentModule()->getParentModule()->getSubmodule("MobilityManager"));	
	timeOfGetStats = 0;
	candidates.clear();
	candidatesCH.clear();
	replyCH.clear();
	children.clear();	
	currentSequenceNumber = 0;
	delaySetupPhase = 2;
	inSlot = false;
	isCH = false;
	endFormClus = false;
	beTentative = false;
    	/*inicia o processo e cada nodo obtem a posicao da BS*/
	if(!isSink)
	{
	   cTopology *topo;
	   topo = new cTopology("topo");
	   topo->extractByNedTypeName(cStringTokenizer("node.Node").asVector());
	   VirtualMobilityManager *nodeMobilityModule =	check_and_cast<VirtualMobilityManager*>	(topo->getNode(0)->getModule()->getSubmodule("MobilityManager"));
	   bsLocation = new NodeLocation_type[1];
	   bsLocation[0] = nodeMobilityModule->getLocation();
	}
	
	/*--- timers boot ---*/	
	if(!isSink) setTimer(NETWORK_SELF_NEW_ROUND, 0);
	setTimer(TIME_SERVICE, 0);
}

void EeucRouting::fromApplicationLayer(cPacket *pkt, const char *dstAddr)
{
	
	EEUCRouting_DataFrame *netPacket =
	    new EEUCRouting_DataFrame("Cluster routing data packet", NETWORK_LAYER_PACKET);
	netPacket->setEeucRoutingPacketKind(EEUC_ROUTING_DATA_FRAME);
	netPacket->setSource(SELF_NETWORK_ADDRESS);
	encapsulatePacket(netPacket, pkt);
	netPacket->setSequenceNumber(currentSequenceNumber);
	currentSequenceNumber++;

	if(!isCH && endFormClus) {
		char buff1[256];
		string next;
		sprintf(buff1, "%i", candidatesCH[0].id);
		next.assign(buff1);
		netPacket->setDestination(next.c_str());
		if (inSlot) {
			bufferPacket(netPacket); 
			processBufferedPacket();
		}
		else bufferPacket(netPacket);
	} 
	else if (!isCH && !endFormClus) 
	{
		TempTXBuffer.push(netPacket);
	}
	else if (isCH) 
	{
		bufferAggregate.push_back(*netPacket);
	}
}

void EeucRouting::fromMacLayer(cPacket *pkt, int srcMacAddress, double RSSI, double LQI){
	EEUCRouting_DataFrame *netPacket = dynamic_cast <EEUCRouting_DataFrame*>(pkt);

	if (!netPacket)	return;
	string dst(netPacket->getDestination());

	switch (netPacket->getEeucRoutingPacketKind()) {
	
		case EEUC_ROUTING_DATA_FRAME:
		{

			if (isCH && dst.compare(SELF_NETWORK_ADDRESS) == 0){
				trace() << "[Node_"<< self <<"] t= " << simTime() << ": Aggregate Data Frame \n";
					bufferAggregate.push_back(*netPacket);			
			}
			break;
		}
	
		case EEUC_ROUTING_AGGR_FRAME:
		{

			if (dst.compare("0") == 0 && isSink) 
			{
				trace() << "[Node_"<< self <<"] t= " << simTime() << ": Received aggregated frame\n";
				int size = netPacket->getAggregateArraySize();
				for(int i = 0; i<size; i++)
				{
					toApplicationLayer(decapsulatePacket(&(netPacket->getAggregate(i))));
				}	
			}
			if (isCH && dst.compare(SELF_NETWORK_ADDRESS) == 0) 
			{
				trace() << "[Node_"<< self <<"] t= " << simTime() << ": Rebroadcast this packet since we are not its destination\n";
				EEUCRouting_DataFrame *dupPacket = netPacket->dup();
				char buff1[256];
				string next;
				sprintf(buff1, "%i", relayNode);
				next.assign(buff1);
				dupPacket->setDestination(next.c_str());
				toMacLayer(dupPacket, BROADCAST_MAC_ADDRESS);
			}
			break;
		}

		case EEUC_ROUTING_COMPETE_HEAD_FRAME:
		{
			//trace() << "Received COMPETE_HEAD_MSG " << "\n";
			CHRec temp;
			double distIJ ;
			distIJ = distanceTake(netPacket->getPosX(), netPacket->getPosY(), mobMgrModule->getX(), mobMgrModule->getY());
			if( (distIJ  < netPacket->getRcomp()) || (distIJ < rangeComp) )
			{					 
				temp.id = atoi(netPacket->getSource());
				temp.energy = netPacket->getEnergy();
				temp.rComp = netPacket->getRcomp();
				candidates.push_back(temp);
			}
		break;
		}
		
		case EEUC_ROUTING_FINALHEAD_FRAME:
		{
			//trace() << "Received Finalhead " << "\n";
		     	int id;
			for( id = 0; id < candidates.size(); id++)
			{
				if (atoi(netPacket->getSource()) == candidates[id].id)
				{
					char buff[50];
					sprintf(buff, "Node[%i] sending QuitElectionMsg", self);
					EEUCRouting_DataFrame *setupMsg = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
					setupMsg->setEeucRoutingPacketKind(EEUC_ROUTING_QUITELECTION_FRAME);
					setupMsg->setSource(SELF_NETWORK_ADDRESS);
					setupMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
					toMacLayer( setupMsg, BROADCAST_MAC_ADDRESS );
					//trace() << "[Node_"<< self <<"] t= " << "Sending Quit Election Msg";
				}
			}
		break;
		}
		
		case EEUC_ROUTING_QUITELECTION_FRAME:
		{
			int id;
			for( id = 0; id < candidates.size(); id++)
			{		
				if ( atoi(netPacket->getSource()) == candidates.at(id).id )
				{
					candidates.erase( candidates.begin() + (id));
				}
			}	
					
		break;
		}
		
		case EEUC_ROUTING_CH_ADV_FRAME:
		{
			//trace() << "Receive CH_ADV_MSG " << "\n";
			if (!isCH) 
			{
				CHRec temp;
				double distIJ ;
				distIJ = distanceTake(netPacket->getPosX(), netPacket->getPosY(), mobMgrModule->getX(), mobMgrModule->getY());
				temp.id = atoi(netPacket->getSource());
				temp.distance = distIJ;	
				candidatesCH.push_back(temp);
			}
		break;
		}

		case EEUC_ROUTING_CH_ADV_RELAY_FRAME:
		{
			//trace() << "Receive CH_ADV_MSG " << "\n";
			//double NodeBs = distanceTake( mobMgrModule->getX(),mobMgrModule->getY(), bsLocation[0].x, bsLocation[0].y);
			//double NeighborBS = distanceTake( netPacket->getPosX(), netPacket->getPosY(), bsLocation[0].x, bsLocation[0].y);
			double distIJ = distanceTake(netPacket->getPosX(), netPacket->getPosY(), mobMgrModule->getX(), mobMgrModule->getY());
			/*if (isCH && tdMax > dBs) 
			{
				relayNode = 0;					
			}*/
			if (isCH && distIJ <= (kRcomp * rangeComp) && netPacket->getDistanceBS() < dBs)
			{
				CHRec temp;
				temp.id = atoi(netPacket->getSource());
				temp.distance = distIJ;	
				temp.rssi = RSSI;
				replyCH.push_back(temp);
			}
		break;
		}
		
		case EEUC_ROUTING_JOIN_FRAME: 
		{
			if(isCH && dst.compare(SELF_NETWORK_ADDRESS) == 0) 
			{
				trace() << "[Node_"<< self <<"] t= " << simTime() << ": Received a join, is CH, I'm the destination. Adding to children\n";
				children.push_back(atoi(netPacket->getSource()));
				
			}
	 	break;
		}
		
		case EEUC_ROUTING_SCHE_FRAME: 
		{
			if(isCH || isSink) break;
			else {
				int i=0;
				while (netPacket->getSchedule(i)!=atoi(SELF_NETWORK_ADDRESS) && i<MAX_CLUSTER_SIZE-1) i++;
				
				if (netPacket->getSchedule(i)==atoi(SELF_NETWORK_ADDRESS)) 
				{
					//start TDMA slot timer
					setTimer(NETWORK_SEND_SLOT_CH, i*slotLength);
					clusterLength = netPacket->getClusterLength();	
					setRadio_Sleep(0);
					trace() << "[Node_"<< self <<"] t= " << simTime() << ": Received Schedule, starting appropriate timer, I am: "<< i << "th \n";
					//trace() << "[Node_"<< self <<"] "<< (double) i*slotLength << "\n";
				}
			}
	 	break;
		}
	}
}

void EeucRouting::timerFiredCallback(int index){
	switch (index) {

		case NETWORK_SELF_NEW_ROUND:
		{
			setRadio_Listen(0);
			setRadio_TXPowerLevel(0, 0.0);
			candidates.clear();
			candidatesCH.clear();
			replyCH.clear();
			children.clear();
			endFormClus = false;
			inSlot = false;
			isCH = false;	
			beTentative = false;
			if (getTimer(NETWORK_SEND_SLOT_CH) != -1) 
			{
				cancelTimer(NETWORK_SEND_SLOT_CH);
			}
			double random = genk_dblrand(0);
			if (random<probability)
			{
				trace() << "Win";
				beTentative = true;
				setTimer(NETWORK_BE_TENTATIVE_HEAD, (0.1+genk_dblrand(0)));	
				setTimer(NETWORK_FINAL_HEAD_MSG, (1.5+genk_dblrand(0)));			
			}
			if (!beTentative) 
			{
				setRadio_Sleep(0);
				setTimer(NETWORK_WAIT_ADV_CH, 3);
			}
			setTimer(NETWORK_SEND_ADV_CH, (3.1+genk_dblrand(0)));
			setTimer(NETWORK_SEND_ADV_RELAY_CH, (4+genk_dblrand(0)));
			setTimer(NETWORK_FORM_TIMEOUT, (5+genk_dblrand(0)));
			setTimer(NETWORK_SCHE_TIMEOUT, (6.5+genk_dblrand(0)));	
			
			setTimer(NETWORK_SELF_NEW_ROUND, roundLength);
		break;
		}

		case NETWORK_BE_TENTATIVE_HEAD:
		{
			char buff[50];
			sprintf(buff, "Node[%i] Be Tentative Head announcement", self);				
			double energy = resMgrModule->getRemainingEnergy();
			
			/*realizar calculo da distancia maxima*/
			double xFieldSize , yFieldSize ;
			xFieldSize = getParentModule()->getParentModule()->getParentModule()->par("field_x");
			yFieldSize = getParentModule()->getParentModule()->getParentModule()->par("field_y");
			dMax = distanceTake( bsLocation[0].x , bsLocation[0].y, xFieldSize , yFieldSize);
			dMin = 1.0;	//cvalor estipulado como minimo

			/*distancia do nodo para a bs*/
			dBs = distanceTake( mobMgrModule->getX(),mobMgrModule->getY(), bsLocation[0].x, bsLocation[0].y);

			rangeComp = (1-cRange*((dMax- dBs)/(dMax -dMin)))*rMax;				

			EEUCRouting_DataFrame *setupMsg = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
			setupMsg->setEeucRoutingPacketKind(EEUC_ROUTING_COMPETE_HEAD_FRAME);
			setupMsg->setSource(SELF_NETWORK_ADDRESS);
			setupMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
			setupMsg->setEnergy(energy);
			setupMsg->setPosX(mobMgrModule->getX());
			setupMsg->setPosY(mobMgrModule->getY());
			setupMsg->setRcomp( rangeComp );

			energyTemp = resMgrModule->getRemainingEnergy();
	
			toMacLayer(setupMsg, BROADCAST_MAC_ADDRESS);
			trace() << "Sent Compete Head Msg ";
		break;
		}
		
		case NETWORK_FINAL_HEAD_MSG:
		{	
			int re;
			bool energySmaller= false;
		
			for( re = 1; re < candidates.size(); ++re){		
				if (energyTemp < candidates.at(re).energy)
				{
					energySmaller=true;
					break;
				}
				else if(energyTemp == candidates.at(re).energy && atoi(SELF_NETWORK_ADDRESS) > candidates.at(re).id) 
				{
					energySmaller=true;
					break;
				}
			}
			if (!energySmaller) 
			{
				char buff[50];
				sprintf(buff, "Node[%i] Final Head Msg", self);
				EEUCRouting_DataFrame *setupMsg = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
				setupMsg->setEeucRoutingPacketKind(EEUC_ROUTING_FINALHEAD_FRAME);
				setupMsg->setSource(SELF_NETWORK_ADDRESS);
				setupMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
				toMacLayer(setupMsg, BROADCAST_MAC_ADDRESS);
				isCH = true;
				trace()<< "Sent Final Msg";
			}
		break;
		}
		
		case NETWORK_WAIT_ADV_CH:
		{
			if(!isCH)
			{
				setRadio_Listen(0);
				trace() << "In Listen Mode";
			}
		break;
		}
		
		case NETWORK_SEND_ADV_CH:
		{
			if(isCH)
			{
				char buff[50];
				sprintf(buff, "Node[%i] ADV Head Msg", self);
				EEUCRouting_DataFrame *setupMsg = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
				setupMsg->setEeucRoutingPacketKind(EEUC_ROUTING_CH_ADV_FRAME);
				setupMsg->setSource(SELF_NETWORK_ADDRESS);
				setupMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
				setupMsg->setPosX(mobMgrModule->getX());
				setupMsg->setPosY(mobMgrModule->getY());
				toMacLayer(setupMsg, BROADCAST_MAC_ADDRESS);
				trace()<< "Sent CH_ADV_MSG";
			}
		break;
		}

		case NETWORK_SEND_ADV_RELAY_CH:
		{
			if(isCH)
			{
				char buff[50];
				sprintf(buff, "Node[%i] ADV Head Msg", self);
				EEUCRouting_DataFrame *setupMsg = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
				setupMsg->setEeucRoutingPacketKind(EEUC_ROUTING_CH_ADV_RELAY_FRAME);
				setupMsg->setSource(SELF_NETWORK_ADDRESS);
				setupMsg->setDestination(BROADCAST_NETWORK_ADDRESS);
				setupMsg->setPosX(mobMgrModule->getX());
				setupMsg->setPosY(mobMgrModule->getY());
				setupMsg->setDistanceBS(dBs);
				toMacLayer(setupMsg, BROADCAST_MAC_ADDRESS);
				trace()<< "Sent CH_ADV_RELAY_MSG";
			}
		break;
		}
			
		case NETWORK_FORM_TIMEOUT:
		{
			if(!isCH)
			{
				endFormClus = true;
				sort(candidatesCH.begin(), candidatesCH.end(), cmpECH_distance);
				EEUCRouting_DataFrame *setupPkt = new EEUCRouting_DataFrame("Cluster routing join packet", NETWORK_LAYER_PACKET);
				setupPkt->setEeucRoutingPacketKind(EEUC_ROUTING_JOIN_FRAME);	
				setupPkt->setSource(SELF_NETWORK_ADDRESS);
				char buff1[256];
				string next;
				sprintf(buff1, "%i", candidatesCH[0].id);
				next.assign(buff1);
				setupPkt->setDestination(next.c_str());			
				toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
				trace()<< "Sent CH_JOIN_MSG";
			}else {
				if (isCH && tdMax > dBs) 
				{
					relayNode = 0;					
				}
				else if(replyCH.size()!=0) 
				{
					sort(replyCH.begin(), replyCH.end(), cmpECH_distance);
					relayNode = replyCH[0].id;
				}
				else relayNode = 0;
			}	
		break;
		}

		case NETWORK_SCHE_TIMEOUT:
		{
			if(isCH) 
			{
				if (children.size()!=0)
				{
					trace() << "Membros: " << children.size() << "\n";
					char buff[50];
					sprintf(buff, "Node[%i] sending TDMA schedule", self);
					EEUCRouting_DataFrame *setupPkt = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
					setupPkt->setEeucRoutingPacketKind(EEUC_ROUTING_SCHE_FRAME);
					setupPkt->setSource(SELF_NETWORK_ADDRESS);
					setupPkt->setDestination(BROADCAST_NETWORK_ADDRESS);				
					setupPkt->setClusterLength(children.size());
					clusterLength = children.size();
					for(int i=0;i<children.size() && i<MAX_CLUSTER_SIZE; i++) setupPkt->setSchedule(i,children[i]);
						trace() << ": Sending schedule: ";
					for (int i=0;i<children.size() && i<MAX_CLUSTER_SIZE; i++)
					{
						trace() << children[i] << " ";
					}
					toMacLayer(setupPkt, BROADCAST_MAC_ADDRESS);
					setTimer(NETWORK_SEND_SLOT_CH, children.size()*slotLength);
				}else {
					setTimer(NETWORK_SEND_SLOT_CH, slotLength);							 
				      }
			}
			break;
		}	
		
		case NETWORK_SEND_SLOT_CH:
		{
			if(isCH && children.size()==0) setTimer(NETWORK_SEND_SLOT_CH, slotLength);	
			else setTimer(NETWORK_SEND_SLOT_CH, clusterLength*slotLength);	
			if (isCH) {
				setRadio_TXPowerLevel(0, 0.0);
				sendAggregate(); 
				setTimer(NETWORK_SEND_ENDS_CH, slotLength);
			}
			if (!isCH){
				int power = 0 - ((candidatesCH[0].rssi)-(-95));
				levelTxPower(power);
				processBufferedPacket();
				setTimer(NETWORK_SEND_ENDS_CH, slotLength);
			}
			inSlot=true;
			break;
		}
		case NETWORK_SEND_ENDS_CH:
		{
			if (!isSink && !isCH) {
				setRadio_Sleep(0);
			}
			inSlot = false;
			break;
		}			

		case TIME_SERVICE:{
			//if(EnergyTime)
			//trace_location() << simTime() << " " << "[" << self << "] " << resMgrModule->getRemainingEnergy();
			if(EnergyTime)
			trace_location() << timeOfGetStats << " " << resMgrModule->getRemainingEnergy();
			if(EnergyMap)
			trace_location() << simTime() << " " << "[" << self << "] " << mobMgrModule->getX() << " " << mobMgrModule->getY() << " " << resMgrModule->getRemainingEnergy();
			if(EnergyDissipation)
			trace_location() << simTime() << " " << "[" << self << "] " << resMgrModule->getSpentEnergy();
			timeOfGetStats += periodicEnergyCalculationInterval;
			setTimer(TIME_SERVICE, periodicEnergyCalculationInterval);
		break;
		}

	}
}

void EeucRouting::sendAggregate()
{
	trace() << "[Node_" << self << "] t= " << SIMTIME_DBL(simTime()) << ": Sent Pkt Aggr"  << "\n";
	//powerDrawn(1000);
	char buff[50];
	sprintf(buff, "Node[%i] sending Aggredated Packet", self);
	EEUCRouting_DataFrame *aggrPacket = new EEUCRouting_DataFrame(buff, NETWORK_LAYER_PACKET);
	aggrPacket->setAggregateArraySize(bufferAggregate.size());						
	for (int i = 0; i<bufferAggregate.size(); i++)
		aggrPacket->setAggregate(i, bufferAggregate[i]);
	aggrPacket->setEeucRoutingPacketKind(EEUC_ROUTING_AGGR_FRAME);
	aggrPacket->setSource(SELF_NETWORK_ADDRESS);
	char buff1[256];
	string next;
	sprintf(buff1, "%i", relayNode);
	next.assign(buff1);
	aggrPacket->setDestination(next.c_str());
	if (bufferPacket(aggrPacket)) processBufferedPacket();
	bufferAggregate.clear();
}

void EeucRouting::processBufferedPacket()
{
	if(!isCH) 
	{
		while (!TempTXBuffer.empty())
		{
			cPacket *pkt = TempTXBuffer.front();	
			EEUCRouting_DataFrame *netPacket = dynamic_cast <EEUCRouting_DataFrame*>(pkt);
			char buff1[256];
			string next;
			sprintf(buff1, "%i", candidatesCH[0].id);
			next.assign(buff1);
			netPacket->setDestination(next.c_str());
			bufferPacket(netPacket); 
			TempTXBuffer.pop();
		}
	}

	while (!TXBuffer.empty()) 
	{
		trace() << "[Node_" << self << "] t= " << SIMTIME_DBL(simTime()) << ": Sending data "<< "\n";
		toMacLayer(TXBuffer.front(), BROADCAST_MAC_ADDRESS);
		TXBuffer.pop();
	}
}

double EeucRouting::distanceTake( double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void EeucRouting::setRadio_Listen(double delay){
	if(delay == 0.0)
		send(createRadioCommand(SET_STATE, RX), "toMacModule");
	else
		sendDelayed(createRadioCommand(SET_STATE, RX), delay, "toMacModule");
}

void EeucRouting::setRadio_TXPowerLevel(int parLevel, double delay){
	if(delay == 0.0)
		send(createRadioCommand(SET_TX_OUTPUT, parLevel), "toMacModule");
	else
		sendDelayed(createRadioCommand(SET_TX_OUTPUT, parLevel), delay, "toMacModule");
}

void EeucRouting::setRadio_Sleep(double delay){
	if(delay == 0.0)
		send(createRadioCommand(SET_STATE, SLEEP), "toMacModule");
	else
		sendDelayed(createRadioCommand(SET_STATE, SLEEP), delay, "toMacModule");
}

void EeucRouting::levelTxPower(int linkBudget){
	int powers[8] = {0, -1, -3, -5, -7, -10, -15, -25};
	vector <int> txLevelPower(powers, powers+8);
	vector<int>::iterator constIterator;
	sort(txLevelPower.begin(), txLevelPower.end());
	for (constIterator = txLevelPower.begin();
		constIterator != txLevelPower.end();
	        constIterator++ ) {
		if(*constIterator > (linkBudget))
		{
			setRadio_TXPowerLevel(*constIterator, 0.0);
			trace() << linkBudget << "\n";
			trace() << *constIterator << "\n";
			break;
		}
	}
}

bool cmpECH_distance(CHRec a, CHRec b){
	return (a.rssi > b.rssi);
}

bool cmpECH_id(CHRec a, CHRec b){
	return (a.id > b.id);
}
