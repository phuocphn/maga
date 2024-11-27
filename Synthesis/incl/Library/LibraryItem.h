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

#ifndef SYNTHESIS_INCL_LIBRARY_LIBRARYITEM_H_
#define SYNTHESIS_INCL_LIBRARY_LIBRARYITEM_H_

#include "Core/incl/Common/Object.h"
#include "Core/incl/Circuit/Instance/InstanceId/InstanceName.h"
#include "Core/incl/Circuit/Terminal/TerminalId/TerminalName.h"
#include "Core/incl/Circuit/Terminal/TerminalId/TerminalId.h"
#include "Core/incl/Circuit/Net/NetId/NetId.h"
#include "Core/incl/Circuit/Device/DeviceId/DeviceId.h"

#include <vector>


namespace Core
{
	class Instance;
	class Circuit;
}

namespace Synthesis
{

	class LibraryItem : public Core::Object
	{
	public:
		LibraryItem();
		~LibraryItem();

		/**
		 * @brief Create a Instance object, with instance.master = circuit, 
		 * instance.id(entificer) is created from `instanceName` 
		 * 
		 * @param circuit 
		 * @param instanceName 
		 * @return Core::Instance& 
		 */
		Core::Instance & createInstance(const Core::Circuit & circuit, Core::InstanceName instanceName) const;
		
		/**
		 * @brief Create a Instance Id object from InstanceName
		 * 
		 * @param instanceName 
		 * @return Core::InstanceId 
		 */
		Core::InstanceId createInstanceId(Core::InstanceName instanceName) const;

		virtual std::string toStr() const = 0;
	protected:
			/**
			 * @brief create InstanceID from `instanceName`, after that set instance.identifier to the newly created InstanceID. 
			 * 
			 * @param instance 
			 * @param instanceName 
			 * @updated_params: instance.identifitier
			 */
			void createInstanceId(Core::Instance & instance, Core::InstanceName instanceName) const;
			
			/**
			 * @brief Create a Terminal Id object from TerminalName
			 * 
			 * @param terminalName 
			 * @return Core::TerminalId 
			 */
			Core::TerminalId createTerminalId(Core::TerminalName terminalName) const;
			
			/**
			 * @brief Iterate over instance.master.terminals (Core::TerminalName), convert each one of them to
			 * Core::InstanceTerminal and then insert to instance.instanceTerminalsMap 
			 * 
			 * @param instance : the instance object that wants to add InstanceTerminals.
			 * @updated_params: instance.instanceTerminals
			 */
			void addInstanceTerminals(Core::Instance & instance) const;

			/**
			 * @brief From each NetId in `netNames`, create a tupple <NetId, Core::Net> and
			 * insert into circuit.netMap_
			 * 
			 * @param circuit : The `circuit` object that wants to update its .netMap_ with `nameNames`
			 * @param netNames 
			 * @updated_params: circuit.netMap_
			 */
			void addNetsToCircuit(Core::Circuit & circuit, std::vector<Core::NetId> netNames)  const;

			/**
			 * @brief Add TerminalId to `circuit` and then connect them to corresponding `NetId`. 
			 * Note that the `terminalToNetMap` has the structure of std::map<Core::TerminalName, Core::NetId>,
			 * for each [key // first item] Core::TerminalName -> construct TerminalId (terminalId),
			 * for each[value // second item] Core::NetId -> use it to find the Net in `circuit` (fnet)
			 * 
			 * Add `terminalId` into circuit via `circuit.addTerminal()`
			 * Set Terminal net to `fnet` and terminal type as `input()`
			 * 
			 * @param circuit 
			 * @param terminalToNetMap 
			 */
			void addTerminalsToCircuit(Core::Circuit & circuit, std::map<Core::TerminalName, Core::NetId> terminalToNetMap) const;
			
			/**
			 * @brief Use netId to find the net in the `circuit` via circuit.findNet (fnet)
			 * Use terminalName to construct terminalID and use it to find InstanceTerminal in `instance` (finstanceTerminal)
			 * connect instanceTerminal (finstanceTerminal) to the found net via Core:InstanceTerminal.connect(.)
			 * 
			 * @param circuit 
			 * @param instance 
			 * @param terminalName 
			 * @param netId 
			 * @updated_params: fnet.instanceTerminals_ -> finstanceTerminal.net_ = fnet
			 */
			void connectInstanceTerminal(Core::Circuit & circuit, Core::Instance & instance, Core::TerminalName terminalName, Core::NetId netId) const;
			
			
			
			/**
			 * @brief use netName/netId to find the net in the `circuit` via circuit.findNet (fnet)
			 * Use terminalName to to construct TerminalId use add it to circuit via .addTerminal(), got Core::Terminal (rTerminal)
			 * set rTerminal.Type == input()
			 * 
			 * NO INSTANCE object involved!!!
			 * 
			 * @param circuit 
			 * @param netName 
			 * @param terminalName 
			 */
			void addAdditionalTerminalToCircuit(Core::Circuit & circuit, Core::NetId netName, Core::TerminalName terminalName);

			bool transistorsWithSameGateNetAreNotSourceConnected(const Core::Circuit & circuit,std::vector<Core::NetId> gateNets) const;
			bool transistorsAreSourceConnected(const Core::Circuit & circuit, std::vector<Core::DeviceId > devices ) const;

			/**
			 * @brief Get devices (Core::DeviceId) that are mosfet.gate() and connected via each gateNet (in gateNets) 
			 * 
			 * @param circuit 
			 * @param gateNets 
			 * @return std::vector<Core::DeviceId> 
			 */
			std::vector<Core::DeviceId> getGateConnectedFlatDevices(const Core::Circuit & circuit, std::vector<Core::NetId> gateNets) const;
			
			
			/**
			 * @brief Get the Device Names Of Flat Circuit's Devices
			 * 
			 * @param circuit 
			 * @return std::vector<Core::DeviceId> 
			 */
			std::vector<Core::DeviceId> getDeviceNamesOfFlatCircuit(const Core::Circuit & circuit) const;

			bool isSingleDiodeTransistor(const Core::Circuit & circuit) const;

			/**
			 * @brief Check the .master of two [Core::Instance (transistorStack1,transistorStack2)] from loadPart.master is equivalent to circuitIds.voltageBias()
			 * 
			 * @param loadPart 
			 * @return true if .master of both transistorStack1 and transistorStack2 == circuitIds.voltageBias()
			 * @return false otherwise
			 */
			bool bothTransistorStacksAreVoltageBiases(const Core::Instance & loadPart) const;
			bool hasGCC(const Core::Instance & load) const;
			bool sourceTransistorIsDiodeTransistor(const Core::Circuit & transistorStack) const;
			bool outputTransistorIsDiodeTransistor(const Core::Circuit & transistorStack) const;
	};

}
#endif /* SYNTHESIS_INCL_LIBRARY_LIBRARYITEM_H_ */
