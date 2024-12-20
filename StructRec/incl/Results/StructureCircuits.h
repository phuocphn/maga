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

#ifndef STRUCTREC_INCL_STRUCTURE_STRUCTURECIRCUITS_H_
#define STRUCTREC_INCL_STRUCTURE_STRUCTURECIRCUITS_H_

#include "Core/incl/Common/Object.h"
#include "Core/incl/Common/RapidXmlUtils.h"
#include "Core/incl/Common/RapidXml.h"
#include "Core/incl/Circuit/Instance/InstanceId/InstanceId.h"
#include "Core/incl/Circuit/CircuitId/CircuitId.h"
#include "Core/incl/Circuit/Instance/InstanceId/InstanceNamePath.h"
#include "Core/incl/Circuit/Net/NetId/NetId.h"
#include "StructRec/incl/StructureCircuit/Structure/StructureId/StructureName.h"
#include "StructRec/incl/StructureCore.h"
#include <rapidxml_print.hpp>
#include <map>

namespace StructRec {

	class StructureCircuit;
	class StructureInstance;
	class PairLibrary;
	class CircuitRegister;




	/**
	 * @brief This class is 
	 * Note: There is another class with the similar name StructRec:StructureCircuitS (with `s` at the end). Don't be confused!.
	 * @usage: StructureCircuit(s) is used to obtain std::vector<const StructRec::Structure*> via findStructureCircuit(int) in SearchSpace.cpp 
	 * @dependencies:
	 * 	- Core::InstanceId:
	 *  - InstanceNamePath
	 *  - Structure:
	 *  - StructureName
	 *  - StructureId
	 * 	- StructureInstance
	 *  - StructureCore
	 */

	class StructureCircuits : public Core::Object
	{
	public:
		StructureCircuits();
		~StructureCircuits();

		void setCircuitId(Core::CircuitId circuitId);
		void setStructureCore(const StructureCore& structureCore);

		void addStructureCircuit(StructureCircuit & circuit);
		StructureCircuit & findStructureCircuit(const int level);
		const StructureCircuit & findStructureCircuit(const int level) const;

		void addNewInstance(Core::InstanceId instanceId, StructureCircuits & subCircuits);
		StructureInstance & findInstance(Core::InstanceId instanceId) const;

		Core::CircuitId  getCircuitId() const;
		const StructureCore& getStructureCore() const;

		std::vector<StructureInstance*> findAllInstances() const;

		std::map<Core::InstanceId, StructureInstance*> & getInstanceMap();
		const std::map<Core::InstanceId, StructureInstance*> & getInstanceToCircuitMap() const;

		StructureCircuit & findHighestInitializedCircuit();
		const StructureCircuit & findHighestInitializedCircuit() const;
		int getMaxInitializedHierarchyLevel() const;
		int getMaxWithStructuresInitializedHierarchyLevel() const;

		bool hasStructureCircuit(const int level) const;

		std::vector<const Structure*> findConnectedStructures(const Core::NetId& netId) const;
		std::vector<const Structure*> findAllStructures() const;
		std::vector<const Structure*> findStructures(const StructureName& structureName) const;
		std::vector<Structure*> getStructures(const StructureName& structureName);
		std::vector<Structure*> findAllStructuresWithoutParents() const;
		Structure& findStructureInTopCircuit(StructureId structureId) const;


		int findLevel(const Structure& structure) const;

		std::string toStr() const;
		void print(std::ostream & stream, int columns, Core::InstanceNamePath instanceNamePath) const;
		void printSummary(std::ostream& stream) const;

		void writeXml(Core::XmlNode & node, Core::XmlDocument & doc) const;

		void removeUncertainStructures();

	private:
		bool hasInstance(Core::InstanceId id) const;
		void addInstance(StructureInstance & instance);
		bool hasCircuitId() const;

		void printInstances(std::ostream & stream, int columns, Core::InstanceNamePath instanceNamePath) const;
		void printStructures(std::ostream & stream, int columns) const;

		void writeXmlStructures(Core::XmlNode & node, Core::XmlDocument & doc, Core::InstanceNamePath instanceNamePath) const;
		void writeXmlInstances (Core::XmlNode & node, Core::XmlDocument & doc, Core::InstanceNamePath instanceNamePath) const;

		const std::map<const int, StructureCircuit*> & getCircuitMap() const;

		std::string spaces (int columns) const;

		void eraseAllCircuits();
		void eraseAllInstances();

	private:
		const StructureCore* structureCore_;

		// may be the most important property ?
		// for different "hierarchy" circuit map (circuitMap_.size() == 4 == Number of Hierarchy Levels.)
		// in other words, the index of each StructureCircuit corresponds to hierarchyLevel_
		// for `cmd_structure_recognition` we have total of 4 hierarchy levels.
		
		// In the first level H=0 (i.e.  index=0, of circuitMap_[0]), we the 16 elements for circuitMap_[0].structureMap_ (corresponds to 10xMosfetNormalArray + 6xMosfetDiodeArray)
		// 13 wires (n1-n9, /in, /out, /vdd!, /gnd!)

		// In the second Level H=1 (circuitMap_[1].structureMap_ contains 8 elements)
		// 2x MosfetDifferentialPair + 2x MosfetCascodePair + 2x MosfetDiodeStack + 2xMosfetSimpleCurrentMirror  
		// with 13 wires (circuitMap_[1].netMap_)


		// In the Level H=2 (circuitMap_[2].structureMap_ contains 2 elements)
		// 2x MosfetCascodeCurrentMirror
		// with 7 wires (circuitMap_[2].netMap_)

		// In the Level = H=3 (circuitMap_[3].structureMap_ contains 0 elements)
		// with 0 wires (circuitMap_[3].netMap_)
		std::map<const int, StructureCircuit*>  circuitMap_;

		// empty at the time debugging
		std::map<Core::InstanceId, StructureInstance*> instanceMap_;
		
		// circuitId_.name_ == "TOP"
		Core::CircuitId circuitId_;
};


}

#endif /* STRUCTREC_INCL_STRUCTURE_STRUCTURECIRCUITS_H_ */
