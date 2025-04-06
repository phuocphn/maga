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

#include "Synthesis/incl/TopologyLibraryGeneration.h"
#include "Synthesis/incl/Library/FunctionalBlockLibrary.h"
#include "Synthesis/incl/Library/HierarchyLevel5/OpAmps.h"
#include "Synthesis/incl/Results/HSPICEOutputFile.h"

#include "Core/incl/Circuit/Circuit.h"
#include "Core/incl/FlatCircuitRecursion/FlatCircuitRecursion.h"

#include "HSpice/incl/OutputFile/OutputFile.h"

#include "Control/incl/FileFormat/OutputFileFormat.h"

#include "AutomaticSizing/incl/CircuitInformation/CircuitInformation.h"
#include "AutomaticSizing/incl/CircuitInformation/CircuitParameter.h"

#include "StructRec/incl/Results/StructureCircuits.h"
#include "StructRec/incl/Results/Result.h"
#include "StructRec/incl/Library/Library.h"
#include "StructRec/incl/LibraryFile/LibraryFile.h"

#include "Partitioning/incl/Partitioning.h"

#include "Core/incl/Common/BacktraceAssert.h"

#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

/** TODO: Use src/pymaga/AcstUtility.h */
// #include "Partitioning/incl/Results/Result.h"
// #include "src/pymaga/AcstUtility.h"

#include "Control/incl/File/OutputFile.h"
#include "Core/incl/DeviceTypeRegister/DeviceTypesFile.h"


#include "StructRec/incl/Library/Library.h"
#include "StructRec/incl/LibraryFile/LibraryFile.h"


#include "StructRec/incl/LibraryFile/LibraryFile.h"
#include "StructRec/incl/LibraryFile/ArrayLibraryParser.h"
#include "StructRec/incl/LibraryFile/PairLibraryParser.h"
#include "StructRec/incl/Library/Library.h"
#include "StructRec/incl/Library/ArrayLibrary.h"
#include "StructRec/incl/Library/PairLibrary.h"
#include "StructRec/incl/StructureCore.h"


#include "StructRec/incl/LibraryFile/ArrayLibraryParser.h"
#include "StructRec/incl/LibraryFile/ArrayLibraryItemParser.h"
#include "StructRec/incl/Library/ArrayLibrary.h"
#include "StructRec/incl/Library/LibraryItem/ArrayLibraryItem/ArrayLibraryItem.h"
#include "Core/incl/DeviceTypeRegister/DeviceTypeRegister.h"
#include "Control/incl/FileSystemPath.h"
#include "Core/incl/Common/BacktraceAssert.h"

#include "HSpice/incl/InputFile/InputFile.h"
#include "HSpice/incl/InputFile/SupplyNet/SupplyNetFile.h"
#include "Core/incl/DeviceTypeRegister/DeviceTypeRegister.h"
#include "Partitioning/incl/Results/Result.h"

#include "StructRec/incl/Results/Result.h"


namespace Synthesis {

	TopologyLibraryGeneration::TopologyLibraryGeneration() :
		            Control::Analysis<LocalOptionsTopologyLibraryGeneration>(),
					complementaryOpAmpFilePath_(""),
					fullyDifferentialOpAmpFilePath_(""),
					singleOutputOpAmpFilePath_(""),
					structureLibrary_(nullptr),
					structRecResult_(nullptr)
    {
    }

	TopologyLibraryGeneration::~TopologyLibraryGeneration()
    {
    }

	void TopologyLibraryGeneration::initialize()
	{
		std::string singleOutputOpAmpsDirectoryPath(getLocalOptions().getHSPICENetlistDirectoryPath() + "/SingleOutputOpAmps");
    	mkdir(singleOutputOpAmpsDirectoryPath.c_str(), ACCESSPERMS);
    	singleOutputOpAmpFilePath_ = singleOutputOpAmpsDirectoryPath;

    	std::string fullyDifferentialOpAmpsDirectoryPath(getLocalOptions().getHSPICENetlistDirectoryPath() + "/FullyDifferentialOpAmps");
    	mkdir(fullyDifferentialOpAmpsDirectoryPath.c_str(), ACCESSPERMS);
    	fullyDifferentialOpAmpFilePath_ = fullyDifferentialOpAmpsDirectoryPath;

    	std::string complementaryOpAmpsDirectoryPath(getLocalOptions().getHSPICENetlistDirectoryPath() + "/ComplementaryOpAmps");
    	mkdir(complementaryOpAmpsDirectoryPath.c_str(), ACCESSPERMS);
    	complementaryOpAmpFilePath_ = complementaryOpAmpsDirectoryPath;
	}

    void TopologyLibraryGeneration::compute()
    {
		createAllSimpleOpAmps();
		createAllFullyDifferentialOpAmps();
		createAllComplementaryOpAmps();
    }

    void TopologyLibraryGeneration::write()
    {
    }

	void TopologyLibraryGeneration::createAllSimpleOpAmps()
	{
		AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
		AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;

		circuitInformation->setCircuitParameter(*circuitParameter);

		FunctionalBlockLibrary * functionalBlockLibrary = new FunctionalBlockLibrary(*circuitInformation);

		createThreeStageOpAmps(*functionalBlockLibrary,*circuitParameter);

		logDebug("Delete Circuit informations");
		delete circuitInformation;
		logDebug("Delete functional block library");
		delete functionalBlockLibrary;
		logDebug("Finish creating all op amps");
	}

	void TopologyLibraryGeneration::createAllFullyDifferentialOpAmps()
	{
		AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
		AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;

		circuitParameter->setFullyDifferential(true);
		circuitInformation->setCircuitParameter(*circuitParameter);

		logDebug("Create functional block library fully differential op amps");
		FunctionalBlockLibrary * functionalBlockLibrary = new FunctionalBlockLibrary(*circuitInformation);

		logDebug("Create fully differential op amps");
		createOpAmps(*functionalBlockLibrary,*circuitParameter);

		delete circuitInformation;
		delete functionalBlockLibrary;
	}

	void TopologyLibraryGeneration::createAllComplementaryOpAmps()
	{
		AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
		AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;

		circuitParameter->setComplementary(true);
		circuitInformation->setCircuitParameter(*circuitParameter);

		FunctionalBlockLibrary * functionalBlockLibrary = new FunctionalBlockLibrary(*circuitInformation);

		createOpAmps(*functionalBlockLibrary,*circuitParameter);

		delete circuitInformation;
		delete functionalBlockLibrary;
	}

	void TopologyLibraryGeneration::createOpAmps(const FunctionalBlockLibrary & library, const AutomaticSizing::CircuitParameter & circuitParameter)
	{
		std::vector<const Core::Circuit*> oneStageOpAmps;
		std::vector<const Core::Circuit*> symmetricalOpAmps;

		oneStageOpAmps.empty();
		symmetricalOpAmps.empty();

		int caseNumber = 1;
		int indexComplementary = 1;
		int indexFullyDifferential = 1;
		int indexSymmetrical = 1;
		int indexSingleOutput = 1;

		do
		{
			if (circuitParameter.isComplementary())
			{
				oneStageOpAmps = library.getOpAmps().createComplementaryOpAmps(caseNumber, indexComplementary);
			}
			else if(circuitParameter.isFullyDifferential())
			{
				oneStageOpAmps = library.getOpAmps().createFullyDifferentialOneStageOpAmps(caseNumber, indexFullyDifferential);
			}
			else
			{
				// case #1: simple op-amps goes here ....
				oneStageOpAmps = library.getOpAmps().createSimpleOneStageOpAmps(caseNumber, indexSingleOutput);
				symmetricalOpAmps = library.getOpAmps().createSymmetricalOpAmps(caseNumber, indexSymmetrical);

				// case #2: generate symmetrical three-stage opamps directly
				// in this case, do not initialize oneStageOpAmps
				//symmetricalOpAmps = library.getOpAmps().createSymmetricalThreeStageOpAmps(caseNumber, indexSymmetrical);
			}

			for(auto & oneStageOpAmp : oneStageOpAmps)
			{
				const Core::Circuit & flatOneStageOpAmp = createFlatCircuit(*oneStageOpAmp);
				writeHSpiceFile(flatOneStageOpAmp, circuitParameter);

				if(!circuitParameter.isComplementary())
				{
					std::vector<const Core::Circuit*> twoStageOpAmps;
					if(circuitParameter.isFullyDifferential())
					{
						twoStageOpAmps = library.getOpAmps().createFullyDifferentialTwoStageOpAmps(*oneStageOpAmp);
					}
					else
					{
						twoStageOpAmps = library.getOpAmps().createSimpleTwoStageOpAmps(*oneStageOpAmp);
					}
					
					for(auto & twoStageOpAmp : twoStageOpAmps)
					{

						std::ostringstream oneStageOpAmpId;
						oneStageOpAmpId << oneStageOpAmp->getCircuitIdentifier().getId();
						const Core::Circuit & flatTwoStageOpAmp = createFlatCircuit(*twoStageOpAmp);
						writeHSpiceFile(flatTwoStageOpAmp, circuitParameter, oneStageOpAmpId.str());

						std::vector<const Core::Circuit*> threeStageOpAmps;
						if(circuitParameter.isFullyDifferential())
						{
							threeStageOpAmps = library.getOpAmps().createFullyDifferentialThreeStageOpAmps(*oneStageOpAmp, *twoStageOpAmp);
						}
						else
						{
							threeStageOpAmps = library.getOpAmps().createSimpleThreeStageOpAmps(*oneStageOpAmp, *twoStageOpAmp);
						}

						for(auto & threeStageOpAmp : threeStageOpAmps)
						{

							std::ostringstream spiceFilePath;
							spiceFilePath << oneStageOpAmpId.str() << "_" << twoStageOpAmp->getCircuitIdentifier().getId();
							const Core::Circuit & flatThreeStageOpAmp = createFlatCircuit(*threeStageOpAmp);
							writeHSpiceFile(flatThreeStageOpAmp, circuitParameter, spiceFilePath.str());

							delete threeStageOpAmp;
							delete &flatThreeStageOpAmp;

						}
						delete &flatTwoStageOpAmp;
						delete twoStageOpAmp;
					}

				}

				delete oneStageOpAmp;
				delete &flatOneStageOpAmp;
			}

			for(auto & symmetricalOpAmp : symmetricalOpAmps)
			{
				const Core::Circuit & flatSymmetricalOpAmp = createFlatCircuit(*symmetricalOpAmp);
				writeHSpiceFile(flatSymmetricalOpAmp, circuitParameter);
				delete symmetricalOpAmp;
				delete &flatSymmetricalOpAmp;
			}


			caseNumber ++;
			// break;
			// delete &oneStageOpAmps;


			// delete twoStageOpAmps;
			// delete threeStageOpAmps;
		} while (!oneStageOpAmps.empty() || !symmetricalOpAmps.empty());

	}

	void TopologyLibraryGeneration::createThreeStageOpAmps(const FunctionalBlockLibrary & library, const AutomaticSizing::CircuitParameter & circuitParameter)
	{
		std::vector<const Core::Circuit*> oneStageOpAmps;
		std::vector<const Core::Circuit*> threeStageOpAmps;
		std::vector<const Core::Circuit*> symmetricalOpAmps;

		oneStageOpAmps.empty();
		symmetricalOpAmps.empty();

		int caseNumber = 1;
		int indexComplementary = 1;
		int indexFullyDifferential = 1;
		int indexSymmetrical = 1;
		int indexSingleOutput = 1;

		do
		{
			if (circuitParameter.isComplementary())
			{
				oneStageOpAmps = library.getOpAmps().createComplementaryOpAmps(caseNumber, indexComplementary);
			}
			else if(circuitParameter.isFullyDifferential())
			{
				oneStageOpAmps = library.getOpAmps().createFullyDifferentialOneStageOpAmps(caseNumber, indexFullyDifferential);
			}
			else
			{
				// case #1: simple op-amps goes here ....
				oneStageOpAmps = library.getOpAmps().createSimpleOneStageOpAmps(caseNumber, indexSingleOutput);
				symmetricalOpAmps = library.getOpAmps().createSymmetricalOpAmps(caseNumber, indexSymmetrical);

				// case #2: generate symmetrical three-stage opamps directly
				// in this case, do not initialize oneStageOpAmps
				//symmetricalOpAmps = library.getOpAmps().createSymmetricalThreeStageOpAmps(caseNumber, indexSymmetrical);
			}

			for(auto & oneStageOpAmp : oneStageOpAmps)
			{
				const Core::Circuit & flatOneStageOpAmp = createFlatCircuit(*oneStageOpAmp);
				// writeHSpiceFile(flatOneStageOpAmp, circuitParameter);
				std::ostringstream oneStageOpAmpId;
				oneStageOpAmpId << oneStageOpAmp->getCircuitIdentifier().getId();
				if(!circuitParameter.isComplementary())
				{
					std::vector<const Core::Circuit*> twoStageOpAmps;
					if(circuitParameter.isFullyDifferential())
					{
						threeStageOpAmps = library.getOpAmps().createFullyDifferentialThreeStageOpAmps(*oneStageOpAmp, *oneStageOpAmp);
					}
					else
					{
						threeStageOpAmps = library.getOpAmps().createSimpleThreeStageOpAmps(*oneStageOpAmp, *oneStageOpAmp);
					}
					

					for(auto & threeStageOpAmp : threeStageOpAmps)
					{

						std::ostringstream spiceFilePath;
						spiceFilePath << oneStageOpAmpId.str();
						const Core::Circuit & flatThreeStageOpAmp = createFlatCircuit(*threeStageOpAmp);
						writeHSpiceFile(flatThreeStageOpAmp, circuitParameter, spiceFilePath.str());

						delete threeStageOpAmp;
						delete &flatThreeStageOpAmp;

					}

				}

				delete oneStageOpAmp;
				delete &flatOneStageOpAmp;
			}

			for(auto & symmetricalOpAmp : symmetricalOpAmps)
			{
				const Core::Circuit & flatSymmetricalOpAmp = createFlatCircuit(*symmetricalOpAmp);
				writeHSpiceFile(flatSymmetricalOpAmp, circuitParameter);
				delete symmetricalOpAmp;
				delete &flatSymmetricalOpAmp;
			}


			caseNumber ++;
			// break;
			// delete &oneStageOpAmps;


			// delete twoStageOpAmps;
			// delete threeStageOpAmps;
		} while (!oneStageOpAmps.empty() || !symmetricalOpAmps.empty());

	}

	void TopologyLibraryGeneration::writeHSpiceFile(const Core::Circuit &  circuit, const AutomaticSizing::CircuitParameter & circuitParameter, std::string fileNamePrefx)
	{
		std::ostringstream opAmpId;
		std::string opAmpName = circuit.getCircuitIdentifier().getName();
		int opAmpNum = circuit.getCircuitIdentifier().getId();

		if(fileNamePrefx == "empty")
		{
			opAmpId << opAmpName << opAmpNum;
		}
		else
		{
			opAmpId << opAmpName << "_" << fileNamePrefx << "_" << opAmpNum;
		}

		std::ostringstream stringFilePath;

		if(circuitParameter.isComplementary())
		{
			stringFilePath << complementaryOpAmpFilePath_ + "/" << opAmpId.str() << ".ckt";
		}
		else if(circuitParameter.isFullyDifferential())
		{
			stringFilePath << fullyDifferentialOpAmpFilePath_ + "/" << opAmpId.str() << ".ckt";
		}
		else
		{
			stringFilePath << singleOutputOpAmpFilePath_ + "/" << opAmpId.str() << ".ckt";
		}

		HSPICEOutputFile outputFile;
		outputFile.setPath(stringFilePath.str());
		outputFile.setId(opAmpId.str());

		if(getLocalOptions().hasStructureLibraryFile())
		{
			const Partitioning::Result * partitioningResult = analyseCircuit(circuit);
			outputFile.write(circuit, partitioningResult);
			delete partitioningResult;
			deleteStructRecResult();
		}
		else
		{
			outputFile.write(circuit);
		}

	}

	 const Partitioning::Result * TopologyLibraryGeneration::analyseCircuit(const Core::Circuit & circuit)
	{
		const StructRec::StructureCircuits * structRecResults = createStructRecResult(circuit);
		const Partitioning::Result * partitioningResult = createPartitioningResult(*structRecResults);
		return partitioningResult;
	}

	const Core::Circuit & TopologyLibraryGeneration::createFlatCircuit(const Core::Circuit & circuit) const
	{
		Core::FlatCircuitRecursion * flatCircuitRecursion = new Core::FlatCircuitRecursion;
		const Core::Circuit & flatCircuit = *flatCircuitRecursion->createNewFlatCopy(circuit);
		delete flatCircuitRecursion;

		return flatCircuit;
	}

    StructRec::Library * TopologyLibraryGeneration::parseStructRecLibrary() const
    {
   		StructRec::LibraryFile structRecLibraryFile = getLocalOptions().getStructRecLibraryFile();
   		structRecLibraryFile.setDeviceTypeRegister(*getLocalOptions().readDeviceTypeRegister());
   		StructRec::Library* structRecLibrary = structRecLibraryFile.parse();
       	return structRecLibrary;
    }

	const StructRec::StructureCircuits * TopologyLibraryGeneration::createStructRecResult(const Core::Circuit & circuit)
	{
		assert(structRecResult_ == nullptr, "Structure result should only be created if old one is deleted");

		StructRec::Library * structRecLibrary = parseStructRecLibrary();
		StructRec::Result* result = structRecLibrary->recognize(circuit);
		setStructRecResult(*result);
		delete structRecLibrary;
		return &result->getTopLevelResults();
	}

	 const Partitioning::Result * TopologyLibraryGeneration::createPartitioningResult(const StructRec::StructureCircuits & strucRecResult) const
	{
		Partitioning::Partitioning * partitioning = new Partitioning::Partitioning;
		Partitioning::Result * partitioningResult = & partitioning->compute(strucRecResult);
		delete partitioning;
		return partitioningResult;
	}

	void TopologyLibraryGeneration::setStructRecResult(StructRec::Result & strucRecResult)
	{
		structRecResult_ = & strucRecResult;
	}

	void TopologyLibraryGeneration::deleteStructRecResult()
	{
		delete structRecResult_;
		structRecResult_ = nullptr;
	}

}




