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

package node.communication.routing.EeucRouting;

simple EeucRouting like node.communication.routing.iRouting
{
parameters:
	bool collectTraceInfo;
	bool collectTraceStats = default (false);
	int maxNetFrameSize = default (0);
	int netDataFrameOverhead = default (14);
	int netBufferSize = default (32);		
	double probability;
	double roundLength @unit(s);
	int CHReportPower;
	bool isSink = default (false);
	double rMax;
	double cRange;
	double tdMax = default (30.0);
	double kRcomp = default (2.0);
	double slotLength;

	//Estatísticas
	double periodicEnergyCalculationInterval = default (10); //Intervalo de tempo para obter energia
	bool EnergyTime = default (false); //Para Geração do gráfico de energia em função do tempo.
	bool EnergyMap = default (false);  //Para Geração do mapa de energia da rede.
	bool EnergyDissipation = default (false); //Para Geração do gráfico de dissipação de energia.

gates:
	output toCommunicationModule;
	output toMacModule;
	input fromCommunicationModule;
	input fromMacModule;
	input fromCommModuleResourceMgr;
}

