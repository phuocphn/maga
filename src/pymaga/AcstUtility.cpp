#include "src/pymaga/AcstUtility.h"

const Partitioning::Result * getPartitioningResult(const Core::Circuit & circuit)
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
    return partitioningResult;
}
