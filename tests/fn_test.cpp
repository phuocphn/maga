#include <iostream>

#include "Core/incl/Common/Backtrace.h"
#include "Control/incl/Control.h"
#include "Log/incl/LogMacros.h"
#include "Log/incl/Logger.h"

#include <iostream>
#include <time.h>
#include <thread>
#include <vector>
#include <mutex>

#include "AutomaticSizing/incl/CircuitInformationFiles/TechnologieFileEKV.h"
#include "AutomaticSizing/incl/CircuitInformationFiles/TechnologieFileSHM.h"
#include "AutomaticSizing/incl/CircuitInformation/CircuitInformation.h"
#include "AutomaticSizing/incl/CircuitInformation/CircuitParameter.h"
#include "Synthesis/incl/CircuitSpecificationFile/CircuitSpecificationFile.h"

#include "Partitioning/incl/Partitioning.h"
#include "Synthesis/incl/Library/FunctionalBlockLibrary.h"



#include "Core/incl/Common/Object.h"
#include <iostream>
#include <gtest/gtest.h>
// #include <vector>
#include <typeinfo>





#include "Control/incl/Control.h"
#include "Control/incl/FactoryMap/AnalysesFactoryMap.h"
#include "Control/incl/Options/OptionsBase.h"
#include "Control/incl/Options/CircuitReadInOptions.h"
#include "Control/incl/Analysis/AnalysisFactory.h"
#include "Control/incl/Analysis/CircuitAnalysis.h"
#include "Control/incl/File/OutputFile.h"
#include "Control/incl/FileFormat/OutputFileFormat.h"
#include "Control/incl/AbstractCircuitSource.h"
#include "Core/incl/Circuit/Circuit.h"
#include "Core/incl/Common/BacktraceAssert.h"
#include "Log/incl/Logger.h"
#include "Log/incl/LogMacros.h"


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

void create_fuctional_block_library()
{
    AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
    AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;
    circuitInformation->setCircuitParameter(*circuitParameter);

    std::string libFilePath = "/mnt/home/pham/code/maga/StructRec/xml/AnalogLibrary.xml";
    std::string deviceFilePath = "/mnt/home/pham/code/maga/examples/SynthesisSmallLibrary/deviceTypes.xcat";

    StructRec::LibraryFile structRecLibraryFile;
    structRecLibraryFile.setPath(libFilePath);

    // create device register
    Core::DeviceTypesFile deviceTypesFile; 
    deviceTypesFile.setPath(deviceFilePath);
    Core::DeviceTypeRegister * deviceTypeRegister_ = deviceTypesFile.parse();
    Core::DeviceTypeRegister &  deviceTypeRegister_x = *deviceTypeRegister_;

    structRecLibraryFile.setDeviceTypeRegister(deviceTypeRegister_x);
    StructRec::Library* structRecLibrary = structRecLibraryFile.parse();

	Synthesis::FunctionalBlockLibrary * functionalBlockLibrary = new Synthesis::FunctionalBlockLibrary(*circuitInformation);
    functionalBlockLibrary->setStructRecLibrary(*structRecLibrary);
    std::cout << functionalBlockLibrary->toStr();
    //delete functionalBlockLibrary;
    //delete circuitInformation;
}

void read_hspice_netlist_to_core_circuit()
{
    const std::string circuitFilePath = "examples/StructureRecognition/input.ckt"; 
    const std::string supplyNetFilePath = "examples/StructureRecognition/supplyNets.xcat";
    const std::string mappingFilePath = "examples/StructureRecognition/HSpiceMapping.xcat";
    const std::string deviceFilePath = "examples/StructureRecognition/deviceTypes.xcat";


    HSpice::InputFile::InputFile* hspiceInputFile = new HSpice::InputFile::InputFile();
    hspiceInputFile->setPath(circuitFilePath);

    // Create and set supplyNetIdentifiers
    HSpice::InputFile::SupplyNetIdentifiers supplyNetIds;
    supplyNetIds.initializeEmpty();

    HSpice::InputFile::SupplyNetFile supplyNetFile;
    supplyNetFile.setPath(supplyNetFilePath);
    supplyNetFile.setSupplyNetIdentifier(supplyNetIds);
    supplyNetFile.parse();
    hspiceInputFile->setSupplyNetIds(supplyNetIds);

    // Set deviceTypeRegister
    Core::DeviceTypesFile deviceTypesFile; 
    deviceTypesFile.setPath(deviceFilePath);
    Core::DeviceTypeRegister* deviceTypeRegister = deviceTypesFile.parse();
    hspiceInputFile->setDeviceTypeRegister(*deviceTypeRegister);

    // Create deviceLineMapper
    HSpice::DeviceLineMappingFile deviceLineMappingFile;
    deviceLineMappingFile.setPath(mappingFilePath);
    HSpice::DeviceLineMapper* deviceLineMapper = deviceLineMappingFile.parse();
    hspiceInputFile->setDeviceLineMapper(*deviceLineMapper);

    Core::Circuit& circuit = *hspiceInputFile->readNewCircuit();
    delete hspiceInputFile;
    // return circuit;
}

int main(int argc, char *argv[]) {
	
    read_hspice_netlist_to_core_circuit();
    return 0;
}


