/**************************************************************************************************
 *  Main authors:
 *     Inga Abel <inga.abel@tum.de>, 
 *	   Maximilian Neuner <maximilian.neuner@tum.de>, 
 *     Michael Eick <michael.eick@tum.de>
 *
 * 
 *  Copyright (C) 2021
 *  Chair of Electronic Design Automation
 *  Univ.-Prof. Dr.-Ing. Ulf Schlichtmann
 *  TU Muenchen
 *  Arcisstrasse 21
 *  D-80333 Muenchen
 *  Germany
 *
 *  This file is part of acst, a analog circuit analysis, sizing and synthesis enviroment:
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *************************************************************************************************/
#include "Synthesis/incl/Library/HierarchyLevel2/AnalogInverters.h"
#include "Synthesis/incl/Library/HierarchyLevel2/StructuralLevel.h"
#include "Synthesis/incl/Library/HierarchyLevel2/CurrentBiases.h"

#include "Core/incl/Circuit/Circuit.h"
#include "Core/incl/Circuit/CircuitId/CircuitIds.h"
#include "Core/incl/Circuit/Net/NetId/NetName.h"
#include "Core/incl/Circuit/Instance/InstanceTerminal.h"
#include "Core/incl/Circuit/Device/TechType.h"

#include "Core/incl/FlatCircuitRecursion/FlatCircuitRecursion.h"
#include "Synthesis/incl/Library/HierarchyLevel1/DeviceLevel.h"
#include "Synthesis/incl/Library/HierarchyLevel1/NormalTransistor.h"
#include "Synthesis/incl/Library/HierarchyLevel1/DiodeTransistor.h"


#include <sstream>


namespace Synthesis {

    const Core::TerminalName AnalogInverters::OUTPUT_TERMINAL_ = Core::TerminalName("Output");
			
	const Core::TerminalName AnalogInverters::SOURCE_CURRENTBIASNMOS_TERMINAL_ = Core::TerminalName("SourceNmos");
	const Core::TerminalName AnalogInverters::SOURCE_CURRENTBIASPMOS_TERMINAL_ = Core::TerminalName("SourcePmos");
			
	const Core::TerminalName AnalogInverters::IN_CURRENTBIASNMOS_TERMINAL_ = Core::TerminalName("InCurrentBiasNmos");
	const Core::TerminalName AnalogInverters::IN_CURRENTBIASPMOS_TERMINAL_ = Core::TerminalName("InCurrentBiasPmos");

	const Core::TerminalName AnalogInverters::INSOURCE_CURRENTBIASNMOS_TERMINAL_ = Core::TerminalName("InSourceCurrentBiasNmos");
	const Core::TerminalName AnalogInverters::INOUTPUT_CURRENTBIASNMOS_TERMINAL_ = Core::TerminalName("InOutputCurrentBiasNmos");
	const Core::TerminalName AnalogInverters::INOUTPUT_CURRENTBIASPMOS_TERMINAL_ = Core::TerminalName("InOutputCurrentBiasPmos");
	const Core::TerminalName AnalogInverters::INSOURCE_CURRENTBIASPMOS_TERMINAL_ = Core::TerminalName("InSourceCurrentBiasPmos");

	const Core::TerminalName AnalogInverters::INNER_CURRENTBIASNMOS_TERMINAL_ = Core::TerminalName("InnerCurrentBiasNmos");
	const Core::TerminalName AnalogInverters::INNER_CURRENTBIASPMOS_TERMINAL_ = Core::TerminalName("InnerCurrentBiasPmos");

    // in Inverting Stage, "CurrentBiasNmos" will serve as Stage Bias
    const Core::InstanceName AnalogInverters::CURRENTBIASNMOS_ = Core::InstanceName("CurrentBiasNmos");

    // in Inverting Stage, "CurrentBiasPmos" will serve as transconductance
	const Core::InstanceName AnalogInverters::CURRENTBIASPMOS_ = Core::InstanceName("CurrentBiasPmos");

    const Core::InstanceName AnalogInverters::IN_PMOS = Core::InstanceName("In_PMOS");
    const Core::InstanceName AnalogInverters::OUT_PMOS = Core::InstanceName("Out_PMOS");
    const Core::InstanceName AnalogInverters::IN_NMOS = Core::InstanceName("In_NMOS");
    const Core::InstanceName AnalogInverters::OUT_NMOS = Core::InstanceName("Out_NMOS");


			
	const Core::NetId AnalogInverters::OUTPUT_NET_ = Core::NetName("output").createRootIdentifier();
	const Core::NetId AnalogInverters::FIRST_INNER_DRAIN_NET_ = Core::NetName("firstInnerDrain").createRootIdentifier();
	const Core::NetId AnalogInverters::SELF_BIAS_GATE_NET_ = Core::NetName("selfBiasGate").createRootIdentifier();

    
			
	const Core::NetId AnalogInverters::SOURCE_CURRENTBIASNMOS_NET_ = Core::NetName("sourceCurrentBiasNmos").createRootIdentifier();
	const Core::NetId AnalogInverters::SOURCE_CURRENTBIASPMOS_NET_ = Core::NetName("sourceCurrentBiasPmos").createRootIdentifier();
	
    // for simple current bias
	const Core::NetId AnalogInverters::IN_CURRENTBIASNMOS_NET_ = Core::NetName("inCurrentBiasNmos").createRootIdentifier();
	const Core::NetId AnalogInverters::IN_CURRENTBIASPMOS_NET_ = Core::NetName("inCurrentBiasPmos").createRootIdentifier();

    // for cascode current bias
	const Core::NetId AnalogInverters::INSOURCE_CURRENTBIASNMOS_NET_ = Core::NetName("inSourceCurrentBiasNmos").createRootIdentifier();
	const Core::NetId AnalogInverters::INOUTPUT_CURRENTBIASNMOS_NET_ = Core::NetName("inOutputCurrentBiasNmos").createRootIdentifier();
	const Core::NetId AnalogInverters::INOUTPUT_CURRENTBIASPMOS_NET_ = Core::NetName("inOutputCurrentBiasPmos").createRootIdentifier();
	const Core::NetId AnalogInverters::INSOURCE_CURRENTBIASPMOS_NET_ = Core::NetName("inSourceCurrentBiasPmos").createRootIdentifier();

	const Core::NetId AnalogInverters::INNER_CURRENTBIASNMOS_NET_ = Core::NetName("innerCurrentBiasNmos").createRootIdentifier();
	const Core::NetId AnalogInverters::INNER_CURRENTBIASPMOS_NET_= Core::NetName("innerCurrentBiasPmos").createRootIdentifier();


	AnalogInverters::AnalogInverters(StructuralLevel & structuralLevel)
    {
		initializeAnalogInverters(structuralLevel);
        initializeSelfBiasNonInverters(structuralLevel);
    }
	
    AnalogInverters::~AnalogInverters()
    {
        eraseAllAnalogInverters();
    }

	std::vector<const Core::Circuit*>  AnalogInverters::getAnalogInverters() const
    {
        assert(!analogInverters_.empty());
        return analogInverters_;
    }
			
	std::vector<const Core::Circuit*>  AnalogInverters::getAnalogSelfBiasInverters() const
    {
        assert(!analogSelfBiasInverters_.empty());
        return analogSelfBiasInverters_;
    }
			
	std::string AnalogInverters::toStr() const
    {
        std::ostringstream oss;
        oss << "<<<<<<<<<<<<<<<<<<<<<<<<<<< Analog Inverters: " << std::endl;
        for(auto & inverter : analogInverters_)
        {
            Core::FlatCircuitRecursion * flatCircuitRecursion = new Core::FlatCircuitRecursion;
            Core::Circuit * flatCircuit = flatCircuitRecursion->createNewFlatCopy(*inverter);
            oss << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << inverter->getCircuitIdentifier().toStr()<< std::endl;
            oss << inverter->toStr() << std::endl;
            delete flatCircuitRecursion;
            delete flatCircuit;
        }
    
       

        return oss.str();
    }

	
	void AnalogInverters::initializeAnalogInverters(StructuralLevel & structuralLevel)
    {
        std::vector<const Core::Circuit *> analogInverters;
        std::vector<const Core::Circuit *> currentBiasesPmos = structuralLevel.getCurrentBiases().getAllCurrentBiasesPmos();
        std::vector<const Core::Circuit *> currentBiasesNmos = structuralLevel.getCurrentBiases().getAllCurrentBiasesNmos();

        int  index = 1;
        for(auto & currentBiasPmos : currentBiasesPmos)
        {
            for(auto & currentBiasNmos : currentBiasesNmos)
            {
                if(currentBiasPmos->findInstances().size() == 2 && currentBiasNmos->findInstances().size() == 2)
                {
                    if(!(currentBiasPmos->getGateNetsNotConnectedToADrain().size() ==1 
                                && currentBiasNmos->getGateNetsNotConnectedToADrain().size() ==1 ))
                    {
                        const Core::Circuit & analogInverter = createNewAnalogInverter(*currentBiasPmos, *currentBiasNmos, index);
                        analogInverters.push_back(&analogInverter);
                    }
                }
                else
                {
                        const Core::Circuit & analogInverter = createNewAnalogInverter(*currentBiasPmos, *currentBiasNmos, index);
                        analogInverters.push_back(&analogInverter);
                }
            }
        }
        analogInverters_ = analogInverters;
    }
	
	void AnalogInverters::initializeSelfBiasNonInverters(StructuralLevel & structuralLevel)
    {
        std::vector<const Core::Circuit *> analogInverters;


        // case 1: Schematic-of-a-three-stage-NMC-OpAmp (1)
        // Pmos = transconductance, diode-connected Nmos as stage bias
        int  index = 1;

        const Core::Circuit & normalTransistorPmos = structuralLevel.getDeviceLevel().getNormalTransistor().getNormalTransistorPmosCircuit();
        const Core::Circuit & normalTransistorNmos = structuralLevel.getDeviceLevel().getNormalTransistor().getNormalTransistorNmosCircuit();
        const Core::Circuit & diodeTransistorNmos = structuralLevel.getDeviceLevel().getDiodeTransistor().getDiodeTransistorNmosCircuit();

        const Core::Circuit & analogInverter = createSelfBiasNonInverter(normalTransistorPmos, diodeTransistorNmos, normalTransistorNmos, index);
        analogInverters.push_back(&analogInverter);



        const Core::Circuit & diodeTransistorPmos = structuralLevel.getDeviceLevel().getDiodeTransistor().getDiodeTransistorPmosCircuit();
        // const Core::Circuit & normalTransistorPmos = structuralLevel.getDeviceLevel().getNormalTransistor().getNormalTransistorPmosCircuit();
        // const Core::Circuit & normalTransistorNmos = structuralLevel.getDeviceLevel().getNormalTransistor().getNormalTransistorNmosCircuit();
        const Core::Circuit & selfBiasPmosDiodeNonInverter = createselfBiasPmosDiodeNonInverter(diodeTransistorPmos, normalTransistorPmos, normalTransistorNmos, index);
        analogInverters.push_back(&selfBiasPmosDiodeNonInverter);


        analogSelfBiasInverters_ = analogInverters;
    }


    const Core::Circuit & AnalogInverters::createSelfBiasNonInverter(const Core::Circuit & normalTransistorPmos,  const Core::Circuit & diodeTransistorNmos, const Core::Circuit & normalTransistorNmos, int & index)
    {
        // Core::Instance & currentBiasPmosInstance = createInstance(currentBiasPmos, CURRENTBIASPMOS_);
        // Core::Instance & currentBiasNmosInstance = createInstance(currentBiasNmos, CURRENTBIASNMOS_);
        Core::Instance & inPmosInstance = createInstance(normalTransistorPmos, IN_PMOS);
        Core::Instance & outPmosInstance = createInstance(normalTransistorPmos, OUT_PMOS);
        Core::Instance & inDiodeNmosInstance = createInstance(diodeTransistorNmos, IN_NMOS);
        Core::Instance & outNmosInstance = createInstance(normalTransistorNmos, OUT_NMOS);
        

        Core::Circuit * analogInverter = new Core::Circuit;

        Core::CircuitIds circuitIds;
        Core::CircuitId analogInverterId = circuitIds.analogInverter(index);
        analogInverterId.setTechType(Core::TechType::undefined());
		analogInverter->setCircuitIdentifier(analogInverterId);

        std::vector<Core::NetId> netNames;
        std::map<Core::TerminalName, Core::NetId> terminalToNetMap;

        netNames.push_back(OUTPUT_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(OUTPUT_TERMINAL_, OUTPUT_NET_));

        netNames.push_back(SOURCE_CURRENTBIASNMOS_NET_);
        netNames.push_back(SOURCE_CURRENTBIASPMOS_NET_);
        netNames.push_back(FIRST_INNER_DRAIN_NET_);
        netNames.push_back(SELF_BIAS_GATE_NET_);
        
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(SOURCE_CURRENTBIASNMOS_TERMINAL_, SOURCE_CURRENTBIASNMOS_NET_));
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(SOURCE_CURRENTBIASPMOS_TERMINAL_, SOURCE_CURRENTBIASPMOS_NET_));


 
        netNames.push_back(IN_CURRENTBIASPMOS_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(IN_CURRENTBIASPMOS_TERMINAL_, IN_CURRENTBIASPMOS_NET_));
    
        netNames.push_back(IN_CURRENTBIASNMOS_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(IN_CURRENTBIASNMOS_TERMINAL_, IN_CURRENTBIASNMOS_NET_));



		addNetsToCircuit(*analogInverter, netNames);
        addTerminalsToCircuit(*analogInverter, terminalToNetMap);

        analogInverter->addInstance(inPmosInstance);
        analogInverter->addInstance(outPmosInstance);
        analogInverter->addInstance(inDiodeNmosInstance);
        analogInverter->addInstance(outNmosInstance);


        connectInstanceTerminals_SelfBias(*analogInverter, inPmosInstance, outPmosInstance, inDiodeNmosInstance, outNmosInstance);
        inPmosInstance.setCircuit(*analogInverter);
        outPmosInstance.setCircuit(*analogInverter);
        inDiodeNmosInstance.setCircuit(*analogInverter);
        outNmosInstance.setCircuit(*analogInverter);

		
        index++;

        return *analogInverter;
    }

    
    const Core::Circuit & AnalogInverters::createselfBiasPmosDiodeNonInverter(const Core::Circuit & diodeTransistorPmos,  const Core::Circuit & normalTransistorPmos, const Core::Circuit & normalTransistorNmos, int & index)
    {
        Core::Instance & inDiodePmosInstance = createInstance(diodeTransistorPmos, IN_PMOS);
        Core::Instance & outPmosInstance = createInstance(normalTransistorPmos, OUT_PMOS);
        Core::Instance & inNmosInstance = createInstance(normalTransistorNmos, IN_NMOS);
        Core::Instance & outNmosInstance = createInstance(normalTransistorNmos, OUT_NMOS);
        

        Core::Circuit * analogInverter = new Core::Circuit;

        Core::CircuitIds circuitIds;
        Core::CircuitId analogInverterId = circuitIds.analogInverter(index);
        analogInverterId.setTechType(Core::TechType::undefined());
		analogInverter->setCircuitIdentifier(analogInverterId);

        std::vector<Core::NetId> netNames;
        std::map<Core::TerminalName, Core::NetId> terminalToNetMap;

        netNames.push_back(OUTPUT_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(OUTPUT_TERMINAL_, OUTPUT_NET_));

        netNames.push_back(SOURCE_CURRENTBIASNMOS_NET_);
        netNames.push_back(SOURCE_CURRENTBIASPMOS_NET_);
        netNames.push_back(FIRST_INNER_DRAIN_NET_);
        netNames.push_back(SELF_BIAS_GATE_NET_);
        
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(SOURCE_CURRENTBIASNMOS_TERMINAL_, SOURCE_CURRENTBIASNMOS_NET_));
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(SOURCE_CURRENTBIASPMOS_TERMINAL_, SOURCE_CURRENTBIASPMOS_NET_));


 
        netNames.push_back(IN_CURRENTBIASPMOS_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(IN_CURRENTBIASPMOS_TERMINAL_, IN_CURRENTBIASPMOS_NET_));
    
        netNames.push_back(IN_CURRENTBIASNMOS_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(IN_CURRENTBIASNMOS_TERMINAL_, IN_CURRENTBIASNMOS_NET_));



		addNetsToCircuit(*analogInverter, netNames);
        addTerminalsToCircuit(*analogInverter, terminalToNetMap);

        analogInverter->addInstance(inDiodePmosInstance);
        analogInverter->addInstance(outPmosInstance);
        analogInverter->addInstance(inNmosInstance);
        analogInverter->addInstance(outNmosInstance);


        // connectInstanceTerminals_SelfBias(*analogInverter, inPmosInstance, outPmosInstance, inDiodeNmosInstance, outNmosInstance);
        // -----------------------------------
        Core::Circuit & circuit = *analogInverter;
        Core::TerminalId terminalIdOut;
        Core::TerminalId terminalDiodeIdOut;
        terminalIdOut.setTerminalName(NormalTransistor::DRAIN_TERMINAL_);
        terminalDiodeIdOut.setTerminalName(DiodeTransistor::DRAIN_TERMINAL_);


        Core::InstanceTerminal & outTerminalCurrentBiasPmos = outPmosInstance.findInstanceTerminal(terminalIdOut);
        outTerminalCurrentBiasPmos.connect(circuit.findNet(OUTPUT_NET_));
        Core::InstanceTerminal & outTerminalCurrentBiasNmos = outNmosInstance.findInstanceTerminal(terminalIdOut);
        outTerminalCurrentBiasNmos.connect(circuit.findNet(OUTPUT_NET_));

        Core::InstanceTerminal & v5 = inNmosInstance.findInstanceTerminal(terminalIdOut);
        v5.connect(circuit.findNet(FIRST_INNER_DRAIN_NET_));
        Core::InstanceTerminal & v6 = inDiodePmosInstance.findInstanceTerminal(terminalDiodeIdOut);
        v6.connect(circuit.findNet(FIRST_INNER_DRAIN_NET_));

        
        Core::TerminalId terminalIdSource;
        Core::TerminalId terminalDiodeIdSource;
        terminalIdSource.setTerminalName(NormalTransistor::SOURCE_TERMINAL_);
        terminalDiodeIdSource.setTerminalName(DiodeTransistor::SOURCE_TERMINAL_);

        Core::InstanceTerminal & sourceTerminalCurrentBiasPmos = inDiodePmosInstance.findInstanceTerminal(terminalIdSource);
        sourceTerminalCurrentBiasPmos.connect(circuit.findNet(SOURCE_CURRENTBIASPMOS_NET_));
        Core::InstanceTerminal & v2 = outPmosInstance.findInstanceTerminal(terminalIdSource);
        v2.connect(circuit.findNet(SOURCE_CURRENTBIASPMOS_NET_));


        Core::InstanceTerminal & sourceTerminalCurrentBiasNmos = inNmosInstance.findInstanceTerminal(terminalDiodeIdSource);
        sourceTerminalCurrentBiasNmos.connect(circuit.findNet(SOURCE_CURRENTBIASNMOS_NET_));
        Core::InstanceTerminal & v3 = outNmosInstance.findInstanceTerminal(terminalIdSource);
        v3.connect(circuit.findNet(SOURCE_CURRENTBIASNMOS_NET_));

        Core::TerminalId terminalDiodeIdGate;
        Core::TerminalId terminalIdIn;

        terminalDiodeIdGate.setTerminalName(DiodeTransistor::GATE_TERMINAL_);
        terminalIdIn.setTerminalName(NormalTransistor::GATE_TERMINAL_);

        Core::InstanceTerminal & v7 = inDiodePmosInstance.findInstanceTerminal(terminalDiodeIdGate);
        v7.connect(circuit.findNet(SELF_BIAS_GATE_NET_));
        Core::InstanceTerminal & v8 = outPmosInstance.findInstanceTerminal(terminalIdIn);
        v8.connect(circuit.findNet(SELF_BIAS_GATE_NET_));
        

        // Core::TerminalId terminalIdInSource;
        // terminalIdInSource.setTerminalName(CurrentBiases::INSOURCE_TERMINAL_);
        Core::InstanceTerminal & inTerminalCurrentBiasPmos = inNmosInstance.findInstanceTerminal(terminalIdIn);
        inTerminalCurrentBiasPmos.connect(circuit.findNet(IN_CURRENTBIASPMOS_NET_));
        
        Core::InstanceTerminal & inTerminalCurrentBiasNmos = outNmosInstance.findInstanceTerminal(terminalIdIn);
        inTerminalCurrentBiasNmos.connect(circuit.findNet(IN_CURRENTBIASNMOS_NET_));
        // -----------------------------------


        inDiodePmosInstance.setCircuit(*analogInverter);
        outPmosInstance.setCircuit(*analogInverter);
        inNmosInstance.setCircuit(*analogInverter);
        outNmosInstance.setCircuit(*analogInverter);

		
        index++;

        return *analogInverter;
    }



    const Core::Circuit & AnalogInverters::createNewAnalogInverter(const Core::Circuit & currentBiasPmos, const Core::Circuit & currentBiasNmos, int & index)
    {
        Core::Instance & currentBiasPmosInstance = createInstance(currentBiasPmos, CURRENTBIASPMOS_);
        Core::Instance & currentBiasNmosInstance = createInstance(currentBiasNmos, CURRENTBIASNMOS_);
        
        Core::Circuit * analogInverter = new Core::Circuit;

        Core::CircuitIds circuitIds;
        Core::CircuitId analogInverterId = circuitIds.analogInverter(index);
        analogInverterId.setTechType(Core::TechType::undefined());
		analogInverter->setCircuitIdentifier(analogInverterId);

        std::vector<Core::NetId> netNames;
        std::map<Core::TerminalName, Core::NetId> terminalToNetMap;

        netNames.push_back(OUTPUT_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(OUTPUT_TERMINAL_, OUTPUT_NET_));

        netNames.push_back(SOURCE_CURRENTBIASNMOS_NET_);
        netNames.push_back(SOURCE_CURRENTBIASPMOS_NET_);
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(SOURCE_CURRENTBIASNMOS_TERMINAL_, SOURCE_CURRENTBIASNMOS_NET_));
        terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(SOURCE_CURRENTBIASPMOS_TERMINAL_, SOURCE_CURRENTBIASPMOS_NET_));


        if(currentBiasPmos.findInstances().size() == 2)
        {
            netNames.push_back(INSOURCE_CURRENTBIASPMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(INSOURCE_CURRENTBIASPMOS_TERMINAL_, INSOURCE_CURRENTBIASPMOS_NET_));
            netNames.push_back(INOUTPUT_CURRENTBIASPMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(INOUTPUT_CURRENTBIASPMOS_TERMINAL_, INOUTPUT_CURRENTBIASPMOS_NET_));
            netNames.push_back(INNER_CURRENTBIASPMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(INNER_CURRENTBIASPMOS_TERMINAL_, INNER_CURRENTBIASPMOS_NET_));
        }
        else if(currentBiasPmos.findInstances().size() == 1)
        {
            netNames.push_back(IN_CURRENTBIASPMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(IN_CURRENTBIASPMOS_TERMINAL_, IN_CURRENTBIASPMOS_NET_));
        }
        if(currentBiasNmos.findInstances().size() == 2)
        {
            netNames.push_back(INSOURCE_CURRENTBIASNMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(INSOURCE_CURRENTBIASNMOS_TERMINAL_, INSOURCE_CURRENTBIASNMOS_NET_));
            netNames.push_back(INOUTPUT_CURRENTBIASNMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(INOUTPUT_CURRENTBIASNMOS_TERMINAL_, INOUTPUT_CURRENTBIASNMOS_NET_));
            netNames.push_back(INNER_CURRENTBIASNMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(INNER_CURRENTBIASNMOS_TERMINAL_, INNER_CURRENTBIASNMOS_NET_));
        }
        else if(currentBiasNmos.findInstances().size() == 1)
        {
            netNames.push_back(IN_CURRENTBIASNMOS_NET_);
            terminalToNetMap.insert(std::pair<Core::TerminalName, Core::NetId>(IN_CURRENTBIASNMOS_TERMINAL_, IN_CURRENTBIASNMOS_NET_));
        }



		addNetsToCircuit(*analogInverter, netNames);
        addTerminalsToCircuit(*analogInverter, terminalToNetMap);

        analogInverter->addInstance(currentBiasPmosInstance);
        analogInverter->addInstance(currentBiasNmosInstance);


        connectInstanceTerminals(*analogInverter, currentBiasNmosInstance, currentBiasPmosInstance);
        currentBiasPmosInstance.setCircuit(*analogInverter);
        currentBiasNmosInstance.setCircuit(*analogInverter);

		
        index++;

        return *analogInverter;
    }

    void AnalogInverters::connectInstanceTerminals(Core::Circuit & circuit, Core::Instance & currentBiasNmosInstance, 
									Core::Instance & currentBiasPmosInstance)
    {
        Core::TerminalId terminalIdOut;
        terminalIdOut.setTerminalName(CurrentBiases::OUT_TERMINAL_);

        Core::InstanceTerminal & outTerminalCurrentBiasPmos = currentBiasPmosInstance.findInstanceTerminal(terminalIdOut);
        outTerminalCurrentBiasPmos.connect(circuit.findNet(OUTPUT_NET_));

        Core::InstanceTerminal & outTerminalCurrentBiasNmos = currentBiasNmosInstance.findInstanceTerminal(terminalIdOut);
        outTerminalCurrentBiasNmos.connect(circuit.findNet(OUTPUT_NET_));


        Core::TerminalId terminalIdSource;
        terminalIdSource.setTerminalName(CurrentBiases::SOURCE_TERMINAL_);

        Core::InstanceTerminal & sourceTerminalCurrentBiasPmos = currentBiasPmosInstance.findInstanceTerminal(terminalIdSource);
        sourceTerminalCurrentBiasPmos.connect(circuit.findNet(SOURCE_CURRENTBIASPMOS_NET_));

        Core::InstanceTerminal & sourceTerminalCurrentBiasNmos = currentBiasNmosInstance.findInstanceTerminal(terminalIdSource);
        sourceTerminalCurrentBiasNmos.connect(circuit.findNet(SOURCE_CURRENTBIASNMOS_NET_));
       
       
        Core::TerminalId terminalIdIn;
        terminalIdIn.setTerminalName(CurrentBiases::IN_TERMINAL_);

        Core::TerminalId terminalIdInner;
        terminalIdInner.setTerminalName(CurrentBiases::INNER_TERMINAL_);

        Core::TerminalId terminalIdInOutput;
        terminalIdInOutput.setTerminalName(CurrentBiases::INOUTPUT_TERMINAL_);

        Core::TerminalId terminalIdInSource;
        terminalIdInSource.setTerminalName(CurrentBiases::INSOURCE_TERMINAL_);
       
        if(currentBiasPmosInstance.getMaster().findInstances().size() == 2)
        {
            Core::InstanceTerminal & inSourceTerminalCurrentBiasPmos = currentBiasPmosInstance.findInstanceTerminal(terminalIdInSource);
            inSourceTerminalCurrentBiasPmos.connect(circuit.findNet(INSOURCE_CURRENTBIASPMOS_NET_));

            Core::InstanceTerminal & inOutputTerminalCurrentBiasPmos = currentBiasPmosInstance.findInstanceTerminal(terminalIdInOutput);
            inOutputTerminalCurrentBiasPmos.connect(circuit.findNet(INOUTPUT_CURRENTBIASPMOS_NET_));

            Core::InstanceTerminal & innerTerminalCurrentBiasPmos = currentBiasPmosInstance.findInstanceTerminal(terminalIdInner);
            innerTerminalCurrentBiasPmos.connect(circuit.findNet(INNER_CURRENTBIASPMOS_NET_));
        }
        else if(currentBiasPmosInstance.getMaster().findInstances().size() == 1)
        {
            Core::InstanceTerminal & inTerminalCurrentBiasPmos = currentBiasPmosInstance.findInstanceTerminal(terminalIdIn);
            inTerminalCurrentBiasPmos.connect(circuit.findNet(IN_CURRENTBIASPMOS_NET_));
        }

        if(currentBiasNmosInstance.getMaster().findInstances().size() == 2)
        {
            Core::InstanceTerminal & inSourceTerminalCurrentBiasNmos = currentBiasNmosInstance.findInstanceTerminal(terminalIdInSource);
            inSourceTerminalCurrentBiasNmos.connect(circuit.findNet(INSOURCE_CURRENTBIASNMOS_NET_));

            Core::InstanceTerminal & inOutputTerminalCurrentBiasNmos = currentBiasNmosInstance.findInstanceTerminal(terminalIdInOutput);
            inOutputTerminalCurrentBiasNmos.connect(circuit.findNet(INOUTPUT_CURRENTBIASNMOS_NET_));

            Core::InstanceTerminal & innerTerminalCurrentBiasNmos = currentBiasNmosInstance.findInstanceTerminal(terminalIdInner);
            innerTerminalCurrentBiasNmos.connect(circuit.findNet(INNER_CURRENTBIASNMOS_NET_));
        }
        else if(currentBiasNmosInstance.getMaster().findInstances().size() == 1)
        {
            Core::InstanceTerminal & inTerminalCurrentBiasNmos = currentBiasNmosInstance.findInstanceTerminal(terminalIdIn);
            inTerminalCurrentBiasNmos.connect(circuit.findNet(IN_CURRENTBIASNMOS_NET_));
        }

    }
    void AnalogInverters::connectInstanceTerminals_SelfBias(Core::Circuit & circuit, Core::Instance & inPmosInstance, Core::Instance & outPmosInstance, Core::Instance & inDiodeNmosInstance, Core::Instance & outNmosInstance)
    {
        Core::TerminalId terminalIdOut;
        terminalIdOut.setTerminalName(NormalTransistor::DRAIN_TERMINAL_);
        Core::TerminalId terminalDiodeIdOut;
        terminalDiodeIdOut.setTerminalName(DiodeTransistor::DRAIN_TERMINAL_);

        Core::InstanceTerminal & outTerminalCurrentBiasPmos = outPmosInstance.findInstanceTerminal(terminalIdOut);
        outTerminalCurrentBiasPmos.connect(circuit.findNet(OUTPUT_NET_));

        Core::InstanceTerminal & outTerminalCurrentBiasNmos = outNmosInstance.findInstanceTerminal(terminalIdOut);
        outTerminalCurrentBiasNmos.connect(circuit.findNet(OUTPUT_NET_));

        Core::InstanceTerminal & v5 = inPmosInstance.findInstanceTerminal(terminalIdOut);
        v5.connect(circuit.findNet(FIRST_INNER_DRAIN_NET_));

        Core::InstanceTerminal & v6 = inDiodeNmosInstance.findInstanceTerminal(terminalDiodeIdOut);
        v6.connect(circuit.findNet(FIRST_INNER_DRAIN_NET_));

        
        Core::TerminalId terminalIdSource;
        terminalIdSource.setTerminalName(NormalTransistor::SOURCE_TERMINAL_);
        Core::TerminalId terminalDiodeIdSource;
        terminalDiodeIdSource.setTerminalName(DiodeTransistor::SOURCE_TERMINAL_);

        Core::InstanceTerminal & sourceTerminalCurrentBiasPmos = inPmosInstance.findInstanceTerminal(terminalIdSource);
        sourceTerminalCurrentBiasPmos.connect(circuit.findNet(SOURCE_CURRENTBIASPMOS_NET_));
        Core::InstanceTerminal & v2 = outPmosInstance.findInstanceTerminal(terminalIdSource);
        v2.connect(circuit.findNet(SOURCE_CURRENTBIASPMOS_NET_));


        Core::InstanceTerminal & sourceTerminalCurrentBiasNmos = inDiodeNmosInstance.findInstanceTerminal(terminalDiodeIdSource);
        sourceTerminalCurrentBiasNmos.connect(circuit.findNet(SOURCE_CURRENTBIASNMOS_NET_));
        Core::InstanceTerminal & v3 = outNmosInstance.findInstanceTerminal(terminalIdSource);
        v3.connect(circuit.findNet(SOURCE_CURRENTBIASNMOS_NET_));

        Core::TerminalId terminalDiodeIdGate;
        terminalDiodeIdGate.setTerminalName(DiodeTransistor::GATE_TERMINAL_);

    
        
        Core::TerminalId terminalIdIn;
        terminalIdIn.setTerminalName(NormalTransistor::GATE_TERMINAL_);


        Core::InstanceTerminal & v7 = inDiodeNmosInstance.findInstanceTerminal(terminalDiodeIdGate);
        v7.connect(circuit.findNet(SELF_BIAS_GATE_NET_));
        Core::InstanceTerminal & v8 = outNmosInstance.findInstanceTerminal(terminalIdIn);
        v8.connect(circuit.findNet(SELF_BIAS_GATE_NET_));
        

        // Core::TerminalId terminalIdInSource;
        // terminalIdInSource.setTerminalName(CurrentBiases::INSOURCE_TERMINAL_);
        Core::InstanceTerminal & inTerminalCurrentBiasPmos = inPmosInstance.findInstanceTerminal(terminalIdIn);
        inTerminalCurrentBiasPmos.connect(circuit.findNet(IN_CURRENTBIASPMOS_NET_));
        
        Core::InstanceTerminal & inTerminalCurrentBiasNmos = outPmosInstance.findInstanceTerminal(terminalIdIn);
        inTerminalCurrentBiasNmos.connect(circuit.findNet(IN_CURRENTBIASNMOS_NET_));



    }
    void AnalogInverters::eraseAllAnalogInverters()
    {
        for(auto & analogInverter : analogInverters_)
        {
            delete analogInverter;
        }
    }

}
