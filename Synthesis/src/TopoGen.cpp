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

#include "Synthesis/incl/TopoGen.h"
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

const Partitioning::Result * getPartitioningResultXX(const Core::Circuit & circuit)
{	
    // create structRecResult
    // const StructRec::StructureCircuits * structRecResults = createStructRecResult(circuit);
    std::string libFilePath = "StructRec/xml/AnalogLibrary.xml";
    std::string deviceFilePath = "examples/SynthesisSmallLibrary/deviceTypes.xcat";

    StructRec::LibraryFile structRecLibraryFile;
    structRecLibraryFile.setPath(libFilePath);

    // create device register
    Core::DeviceTypesFile deviceTypesFile; 
    deviceTypesFile.setPath(deviceFilePath);
    Core::DeviceTypeRegister * deviceTypeRegister = deviceTypesFile.parse();

    structRecLibraryFile.setDeviceTypeRegister(*deviceTypeRegister);
    StructRec::Library* structRecLibrary = structRecLibraryFile.parse();

    StructRec::Result* result = structRecLibrary->recognize(circuit);
    // setStructRecResult(*result);
    delete structRecLibrary;
    const StructRec::StructureCircuits * structRecResults = &result->getTopLevelResults();

    //const Partitioning::Result * partitioningResult = createPartitioningResult(*structRecResults);
    Partitioning::Partitioning * partitioning = new Partitioning::Partitioning;
    const Partitioning::Result * partitioningResult = & partitioning->compute(*structRecResults);

    delete partitioning;
    return partitioningResult;
}


namespace Synthesis {

	// TopoGen::TopoGen() :
	// 				complementaryOpAmpFilePath_(""),
	// 				fullyDifferentialOpAmpFilePath_(""),
	// 				singleOutputOpAmpFilePath_(""),
	// 				structureLibrary_(nullptr),
	// 				structRecResult_(nullptr)
	TopoGen::TopoGen()
    {
		// make sure the output dir paths are defined correctly!
    	singleOutputOpAmpFilePath_ = "outputs/TopologyGen/SingleOutputOpAmps";
    	fullyDifferentialOpAmpFilePath_ = "outputs/TopologyGen/FullyDifferentialOpAmps";
    	complementaryOpAmpFilePath_ = "outputs/TopologyGen/ComplementaryOpAmps";

		structrecResultFilePath_ = "outputs/TopologyGen/structrec";
		partitionResultFilePath_ = "outputs/TopologyGen/partitioning";

    }

	TopoGen::~TopoGen()
    {
    }

	void TopoGen::initialize()
	{
		// make sure the output dir paths are defined correctly!
    	// singleOutputOpAmpFilePath_ = "TopologyGen/SingleOutputOpAmps";
    	// fullyDifferentialOpAmpFilePath_ = "TopologyGen/FullyDifferentialOpAmps";
    	// complementaryOpAmpFilePath_ = "TopologyGen/ComplementaryOpAmps";
	}

    void TopoGen::compute()
    {
		//createAllSimpleOpAmps();
		// createAllFullyDifferentialOpAmps();
		// createAllComplementaryOpAmps();
    }

    void TopoGen::write()
    {
    }

	void TopoGen::createAllSimpleOpAmps()
	{
		AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
		AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;

		circuitInformation->setCircuitParameter(*circuitParameter);
		FunctionalBlockLibrary * functionalBlockLibrary = new FunctionalBlockLibrary(*circuitInformation);
		createOpAmps(*functionalBlockLibrary,*circuitParameter);

		delete circuitInformation;
		delete functionalBlockLibrary;
	}

	void TopoGen::createAllFullyDifferentialOpAmps()
	{
		AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
		AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;

		circuitParameter->setFullyDifferential(true);
		circuitInformation->setCircuitParameter(*circuitParameter);

		FunctionalBlockLibrary * functionalBlockLibrary = new FunctionalBlockLibrary(*circuitInformation);
		createOpAmps(*functionalBlockLibrary,*circuitParameter);

		delete circuitInformation;
		delete functionalBlockLibrary;
	}

	void TopoGen::createAllComplementaryOpAmps()
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

	void TopoGen::createOpAmps(const FunctionalBlockLibrary & library, const AutomaticSizing::CircuitParameter & circuitParameter)
	{
		std::vector<const Core::Circuit*> oneStageOpAmps;
		std::vector<const Core::Circuit*> symmetricalOpAmps;

		oneStageOpAmps.empty();
		symmetricalOpAmps.empty();

		int caseNumber = 1;
		int indexComplementary = 1;
		int indexFullyDifferential = 1;
		int indexSingleOutput = 1;
		int indexSymmetrical = 1;

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
				oneStageOpAmps = library.getOpAmps().createSimpleOneStageOpAmps(caseNumber, indexSingleOutput);
				// symmetricalOpAmps = library.getOpAmps().createSymmetricalOpAmps(caseNumber, indexSymmetrical);
			}

			// creating two-stage opamps from one-stage opamps
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

					
					// create three-stage opamps from two-stage opamps
					for(auto & twoStageOpAmp : twoStageOpAmps)
					{

						/*** BEGIN_TWO_STAGE_OPAMPS_EXPORTING: write hspice netlist / partitioning result for two-stage opamps */
						std::ostringstream oneStageOpAmpId;
						oneStageOpAmpId << oneStageOpAmp->getCircuitIdentifier().getId();
						const Core::Circuit & flatTwoStageOpAmp = createFlatCircuit(*twoStageOpAmp);
						writeHSpiceFile(flatTwoStageOpAmp, circuitParameter, oneStageOpAmpId.str());

						// std::ostringstream twoStageOpAmpFilePath;
						// twoStageOpAmpFilePath << partitionResultFilePath_ << "/" << flatTwoStageOpAmp.getCircuitIdentifier().getName() << "_" << oneStageOpAmpId.str() << "_" << flatTwoStageOpAmp.getCircuitIdentifier().getId() << ".xml";o
						// Control::OutputFile outputFile;
						// outputFile.setPath(twoStageOpAmpFilePath.str());
						// const Partitioning::Result * partioningTwoStageOpAmpResult = getPartitioningResultXX(flatTwoStageOpAmp);
						// partioningTwoStageOpAmpResult->writeXmlPartitioningResult(outputFile);	
						// delete partioningTwoStageOpAmpResult;		
						/*** END_TWO_STAGE_OPAMPS_EXPORTING */

						
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

							/*** BEGIN_THREE_STAGE_OPAMPS_EXPORTING: write hspice netlist / partitioning result for two-stage opamps */
							std::ostringstream spiceFilePath;
							spiceFilePath << oneStageOpAmpId.str() << "_" << twoStageOpAmp->getCircuitIdentifier().getId();
							const Core::Circuit & flatThreeStageOpAmp = createFlatCircuit(*threeStageOpAmp);
							writeHSpiceFile(flatThreeStageOpAmp, circuitParameter, spiceFilePath.str());
							
							if (1)
							{
								std::ostringstream threeStageOpAmpFilePath;
								threeStageOpAmpFilePath << partitionResultFilePath_ << "/" << threeStageOpAmp->getCircuitIdentifier().getName() << "_" << oneStageOpAmpId.str() << "_" << twoStageOpAmp->getCircuitIdentifier().getId() << "_" << threeStageOpAmp->getCircuitIdentifier().getId() << ".xml";
								// outputFile.setPath(threeStageOpAmpFilePath.str());
								// const Partitioning::Result * partioningThreeStageOpAmpResult =  getPartitioningResultXX(flatThreeStageOpAmp);
								// partioningThreeStageOpAmpResult->writeXmlPartitioningResult(outputFile);	
								// delete partioningThreeStageOpAmpResult;	
								/*** END_THREE_STAGE_OPAMPS_EXPORTING */


								delete threeStageOpAmp;
								delete &flatThreeStageOpAmp;
							}
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

				try{
						std::ostringstream twoStageOpAmpFilePath;
						twoStageOpAmpFilePath << partitionResultFilePath_ << "/" << flatSymmetricalOpAmp.getCircuitIdentifier().getName() << "_" << symmetricalOpAmp->getCircuitIdentifier().getId(); // << "_" << flatSymmetricalOpAmp.getCircuitIdentifier().getId();
						Control::OutputFile outputFile1;
						outputFile1.setPath(twoStageOpAmpFilePath.str());
						const Partitioning::Result * partioningSymmetricalOpAmpResult = getPartitioningResultXX(flatSymmetricalOpAmp);
						partioningSymmetricalOpAmpResult->writeXmlPartitioningResult(outputFile1);	
						delete partioningSymmetricalOpAmpResult;
				}	
				catch (...) {
					std::cout << "Error when do partitioning with: " << flatSymmetricalOpAmp.getCircuitIdentifier().getName() << "_" << symmetricalOpAmp->getCircuitIdentifier().getId(); 
				}
				

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

	void TopoGen::writeHSpiceFile(const Core::Circuit &  circuit, const AutomaticSizing::CircuitParameter & circuitParameter, std::string fileNamePrefx)
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
		// TODO: 
		//if(getLocalOptions().hasStructureLibraryFile())
		if (0)
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
		
		std::cout << "netlist: " <<  stringFilePath.str() << " has been saved!" << std::endl;
	}

	 const Partitioning::Result * TopoGen::analyseCircuit(const Core::Circuit & circuit)
	{
		const StructRec::StructureCircuits * structRecResults = createStructRecResult(circuit);
		const Partitioning::Result * partitioningResult = createPartitioningResult(*structRecResults);
		return partitioningResult;
	}

	const Core::Circuit & TopoGen::createFlatCircuit(const Core::Circuit & circuit) const
	{
		Core::FlatCircuitRecursion * flatCircuitRecursion = new Core::FlatCircuitRecursion;
		const Core::Circuit & flatCircuit = *flatCircuitRecursion->createNewFlatCopy(circuit);
		delete flatCircuitRecursion;

		return flatCircuit;
	}

    StructRec::Library * TopoGen::parseStructRecLibrary() const
    {
		// TODO: 
   		// StructRec::LibraryFile structRecLibraryFile; // = getLocalOptions().getStructRecLibraryFile();
   		// //structRecLibraryFile.setDeviceTypeRegister(*getLocalOptions().readDeviceTypeRegister());
   		// StructRec::Library* structRecLibrary = structRecLibraryFile.parse();
       	// return structRecLibrary;

		std::string libFilePath = "StructRec/xml/AnalogLibrary.xml";
		std::string deviceFilePath = "examples/SynthesisSmallLibrary/deviceTypes.xcat";
	
		StructRec::LibraryFile structRecLibraryFile;
		structRecLibraryFile.setPath(libFilePath);
	
		// create device register
		Core::DeviceTypesFile deviceTypesFile; 
		deviceTypesFile.setPath(deviceFilePath);
		Core::DeviceTypeRegister * deviceTypeRegister = deviceTypesFile.parse();
	
		structRecLibraryFile.setDeviceTypeRegister(*deviceTypeRegister);
		StructRec::Library* structRecLibrary = structRecLibraryFile.parse();
   
		return structRecLibrary;
    }

	const StructRec::StructureCircuits * TopoGen::createStructRecResult(const Core::Circuit & circuit)
	{
		assert(structRecResult_ == nullptr, "Structure result should only be created if old one is deleted");

		StructRec::Library * structRecLibrary = parseStructRecLibrary();
		StructRec::Result* result = structRecLibrary->recognize(circuit);
		setStructRecResult(*result);
		delete structRecLibrary;
		return &result->getTopLevelResults();
	}

	 const Partitioning::Result * TopoGen::createPartitioningResult(const StructRec::StructureCircuits & strucRecResult) const
	{
		Partitioning::Partitioning * partitioning = new Partitioning::Partitioning;
		Partitioning::Result * partitioningResult = & partitioning->compute(strucRecResult);
		delete partitioning;
		return partitioningResult;
	}

	void TopoGen::setStructRecResult(StructRec::Result & strucRecResult)
	{
		structRecResult_ = & strucRecResult;
	}

	void TopoGen::deleteStructRecResult()
	{
		delete structRecResult_;
		structRecResult_ = nullptr;
	}

}




