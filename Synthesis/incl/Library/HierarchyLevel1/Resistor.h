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

#ifndef SYNTHESIS_INCL_LIBRARY_HIERARCHYLEVEL1_RESISTOR_H_
#define SYNTHESIS_INCL_LIBRARY_HIERARCHYLEVEL1_RESISTOR_H_


#include "Core/incl/Circuit/Device/DeviceId/DeviceId.h"
#include "Core/incl/Circuit/Instance/InstanceId/InstanceName.h"
#include "Core/incl/Circuit/Net/NetId/NetId.h"
#include "Core/incl/Circuit/Terminal/TerminalId/TerminalName.h"

#include "Synthesis/incl/Library/LibraryItem.h"


namespace Core
{
	class Circuit;
	class Instance;
	class Device;
}


namespace Synthesis
{


	class Resistor : public LibraryItem
	{
		public:
			Resistor();
			~Resistor();

			Core::Instance & createNewResistorInstance(Core::InstanceName instanceName) const;
			const Core::Circuit & getResistorCircuit() const;
			
            std::string toStr() const;

		public:
			static const Core::TerminalName MINUS_TERMINAL_;
			static const Core::TerminalName PLUS_TERMINAL_;

		private:
			void initializeResistor();

			void addNetsToCircuit(Core::Circuit & circuit);
			void createPinConnections(Core::Circuit & circuit, Core::Device & device);
			void addTerminalsToCircuit(Core::Circuit & circuit);

		private:
			static const Core::DeviceId RESISTOR_DEVICEID_;

			static const Core::NetId MINUS_NET_;
			static const Core::NetId PLUS_NET_;



			Core::Circuit * resistor_;

	};



}
#endif /* SYNTHESIS_INCL_LIBRARY_HIERARCHYLEVEL1_Capacitor_H_ */