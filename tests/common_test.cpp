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



// TEST: 7
#include "AutomaticSizing/incl/CircuitInformationFiles/TechnologieFileEKV.h"
#include "AutomaticSizing/incl/CircuitInformationFiles/TechnologieFileSHM.h"
#include "AutomaticSizing/incl/CircuitInformation/CircuitInformation.h"
#include "AutomaticSizing/incl/CircuitInformation/CircuitParameter.h"
#include "Synthesis/incl/CircuitSpecificationFile/CircuitSpecificationFile.h"




#include "Synthesis/incl/Library/HierarchyLevel1/DeviceLevel.h"

#include "Synthesis/incl/Library/HierarchyLevel3/Loads.h"
#include "Synthesis/incl/Library/HierarchyLevel3/LoadParts.h"
#include "Synthesis/incl/Library/HierarchyLevel3/Transconductances.h"
#include "Synthesis/incl/Library/HierarchyLevel3/StageBiases.h"
#include "Synthesis/incl/Library/HierarchyLevel2/StructuralLevel.h"



class TestObject : public Core::Object {
    // TestObject inherits from Object to use its toStr method and overloaded operators.
    // Additional functionality can be added if required.
};


void playing_with_object_cls()
{

    // Test the Object itself
    TestObject obj;

    // Test the toStr method
    std::cout << "Testing toStr method:" << std::endl;
    std::cout << obj.toStr() << std::endl;

    // Test the << operator with object reference
    std::cout << "Testing << operator with object reference:" << std::endl;
    std::cout << obj << std::endl;

    // Test the << operator with object pointer
    TestObject *objPtr = &obj;
    std::cout << "Testing << operator with object pointer:" << std::endl;
    std::cout << objPtr << std::endl;

    // Test the << operator with null pointer
    TestObject *nullPtr = nullptr;
    std::cout << "Testing << operator with null pointer:" << std::endl;
    std::cout << nullPtr << std::endl;

    // Test the + operator with string and object reference
    std::cout << "Testing + operator with string and object reference:" << std::endl;
    std::string str1 = "Object info: ";
    std::cout << str1 + obj << std::endl;

    // Test the + operator with string and object pointer
    std::cout << "Testing + operator with string and object pointer:" << std::endl;
    std::string str2 = "Object pointer info: ";
    std::cout << str2 + objPtr << std::endl;

    // Test the + operator with string and null pointer
    std::cout << "Testing + operator with string and null pointer:" << std::endl;
    std::cout << str2 + nullPtr << std::endl;

}

void playing_with_AnalysisFactory_cls()
{
    std::string analysisType = "synthesis";
    Control::AnalysesFactoryMap* analysisFactoryMap = Control::AnalysesFactoryMap::getInstance();
    Control::AnalysisFactory* analysisFactory = analysisFactoryMap->find(Control::FactoryMapKey(analysisType));
    std::cout << "analysisFactory:" << analysisFactory << std::endl;
}

// Test #1
TEST (CoreCommonTests, Object_Printing){
    playing_with_object_cls();
}

// Test #2
TEST (Control_FactoryMap, AnalysisFactory){
    playing_with_AnalysisFactory_cls();
}

// Test #3
TEST (Core, DeviceTypesFile)
{   
    // Core::DeviceTypeRegister* HspiceOptions::readDeviceTypeRegister() const (HspiceOptions)
    // Core::DeviceTypesFile deviceTypesFile = Control::HspiceOptions::findDeviceTypeFile();

    // findDeviceTypeFile
    Core::DeviceTypesFile deviceTypesFile; 
    std::string filePath = "InputFileExamples/SynthesisSmallLibrary/deviceTypes.xcat"; //findStringOptionValue(DEVICETYPES_FILE_OPTION_);
    deviceTypesFile.setPath(filePath);

    Core::DeviceTypeRegister* parsedData = deviceTypesFile.parse();

    // std::cout << "type of deviceTypeMap_:" << typeof(parsedData->getdeviceTypeMap_()) << std::endl;
    // out:  typeof(parsedData->getdeviceTypeMap_())  = DeviceTypeMap = typedef std::map<DeviceTypeName, const DeviceType*>
    std::cout << "number of devices:" << parsedData->getdeviceTypeMap_().size() << std::endl;

    // show all keys in deviceTypeMap_
    for (const auto& d : parsedData->getdeviceTypeMap_() ) {
        std::cout <<  typeid(d.first).name()  << ": "<< d.first << std::endl; // "Karl", "George"

        // d.second is DeviceType object.
        std::cout <<  typeid(d.second).name()  << ": "<< d.second<< std::endl; // "Karl", "George"
        std::cout << d.second->getName() << std::endl;
        // std::cout << d.second-> << std::endl;
        std::vector<Core::PinType>  pins = d.second->getPinTypes();
        for (const auto& pt : pins ) {
            std::cout << " pt.toStr(): " <<   pt.toStr() << "  hasAutoConnection/isOptional/isInit:  " << pt.hasAutoConnection() << pt.isOptional() <<  pt.isInitialized() << std::endl;
        }
        std::cout << "****************************" << std::endl;
    }
    std::cout << "parsedData completed! " << std::endl;
}

// Test #4
TEST (StructRec, Library)
{
    // try {
    //     setting_up_structrec();
    //     SUCCEED();
    // }
    // catch (std::exception const & err) {
    //     std::cout << "erro.what(): " << err.what();
    //     FAIL() << err.what();
    // }
    StructRec::LibraryFile structRecLibraryFile; // = getLocalOptions().getStructRecLibraryFile();

    // inside: getStructRecLibraryFile()
    // StructRec::LibraryFile LocalOptions::getStructRecLibraryFile() const
    // {
    // 	 std::string optionValue = findStringOptionValue(XML_STRUCTREC_LIBRARY_FILE_OPTION_);
    // 	 StructRec::LibraryFile xmlLibraryFile;
    // 	 xmlLibraryFile.setPath(optionValue);
    // 	 return xmlLibraryFile;
    // }
    std::string optionValue = "StructRec/xml/AnalogLibrary.xml";
    structRecLibraryFile.setPath(optionValue);

    // original:
    // structRecLibraryFile.setDeviceTypeRegister(getDeviceTypeRegister());

    // converted:
    Core::DeviceTypesFile deviceTypesFile; 
    std::string filePath = "InputFileExamples/SynthesisSmallLibrary/deviceTypes.xcat"; //findStringOptionValue(DEVICETYPES_FILE_OPTION_);
    deviceTypesFile.setPath(filePath);
    Core::DeviceTypeRegister * deviceTypeRegister_ = deviceTypesFile.parse();
    Core::DeviceTypeRegister &  deviceTypeRegister_x = *deviceTypeRegister_;
    structRecLibraryFile.setDeviceTypeRegister(deviceTypeRegister_x);


    StructRec::Library* structRecLibrary = structRecLibraryFile.parse();

    std::cout << "StructRec::Library*: parsed done!" << std::endl;


}

// Test #5
TEST(StructRec, ArrayLibraryParser)
{
    // test method: LibraryFile.cpp
    // **********************************************************************************
	// ArrayLibrary* LibraryFile::parseArrayLibrary(const std::string& filePath) const
	// {
	// 	ArrayLibraryParser arrayLibraryParser;
	// 	arrayLibraryParser.setPath(filePath);
	// 	arrayLibraryParser.setDeviceTypeRegister(getDeviceTypeRegister());
	// 	return arrayLibraryParser.parse();
	// }
    // **********************************************************************************


    StructRec::ArrayLibraryParser arrayLibraryParser;

    std::string structrect_filepath = "StructRec/xml/Array/ArrayLibrary.xml";
    arrayLibraryParser.setPath(structrect_filepath);


    Core::DeviceTypesFile deviceTypesFile; 
    std::string filePath = "InputFileExamples/SynthesisSmallLibrary/deviceTypes.xcat"; //findStringOptionValue(DEVICETYPES_FILE_OPTION_);
    deviceTypesFile.setPath(filePath);
    Core::DeviceTypeRegister * deviceTypeRegister_ = deviceTypesFile.parse();
    Core::DeviceTypeRegister &  deviceTypeRegister_x = *deviceTypeRegister_;
    
    arrayLibraryParser.setDeviceTypeRegister(deviceTypeRegister_x);
    StructRec::ArrayLibrary* parsedArrayLibrary = arrayLibraryParser.parse();

    std::cout<< "StructRec::ArrayLibrary*: parsed done!" << std::endl;
    // std::cout << "file path" << parsedArrayLibrary->getFilePath() << std::endl;
    std::cout << "level: " << parsedArrayLibrary->getLevel()<< std::endl;


}

// TEST #6:
TEST (StructRec, ArrayLibraryItemParser)
{
    std::string relativeFilePath = "Items/Mosfet/MosfetDiodeArray.xml";
    std::string absoluteFilePath = "StructRec/xml/Array/Items/Mosfet/MosfetDiodeArray.xml";

    StructRec::ArrayLibraryItemParser arrayLibraryItemParser;
    arrayLibraryItemParser.setPath(absoluteFilePath);


    // arrayLibraryItemParser.setDeviceTypeRegister(getDeviceTypeRegister());

    Core::DeviceTypesFile deviceTypesFile; 
    deviceTypesFile.setPath("InputFileExamples/SynthesisSmallLibrary/deviceTypes.xcat");
    Core::DeviceTypeRegister * deviceTypeRegister_ = deviceTypesFile.parse();
    arrayLibraryItemParser.setDeviceTypeRegister(*deviceTypeRegister_);


    StructRec::ArrayLibraryItem* arrayLibraryItem = arrayLibraryItemParser.parse();
    arrayLibraryItem->setFilePath(relativeFilePath);

    std::cout<< "StructRec::ArrayLibraryItemParser: parsed done!" << std::endl;

}

/**
 * @brief This test corresponds to the following function: `AutomaticSizing::CircuitInformation * Synthesis::parseCircuitInformation() const`
 * TEST ID: TEST #7
 */
TEST (AutomaticSizing, CircuitInformation)
{
    AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
    // Synthesis::CircuitSpecificationFile specificationFile = getLocalOptions().getCircuitSpecificationsFile();

    Synthesis::CircuitSpecificationFile specificationFile;
    specificationFile.setPath("InputFileExamples/Synthesis/CircuitSpecifications.xml");
    specificationFile.setUseHSpiceLibrary(false);
    specificationFile.parse(*circuitInformation);



    // always use SHM
    // AutomaticSizing::TechnologieFileSHM technologieFile = getLocalOptions().getTechnologieFileSHM();
    AutomaticSizing::TechnologieFileSHM technologieFile;
    technologieFile.setPath("InputFileExamples/Synthesis/TechnologieFile.xml");
    technologieFile.parse(*circuitInformation);

    std::cout << "Created [CircuitInformation] sucessfully!" << std::endl;
}


TEST (Synthesis, LoadPart)
{   
    AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
    // Synthesis::CircuitSpecificationFile specificationFile = getLocalOptions().getCircuitSpecificationsFile();

    Synthesis::CircuitSpecificationFile specificationFile;
    specificationFile.setPath("InputFileExamples/Synthesis/CircuitSpecifications.xml");
    specificationFile.setUseHSpiceLibrary(false);
    specificationFile.parse(*circuitInformation);

    // always use SHM
    // AutomaticSizing::TechnologieFileSHM technologieFile = getLocalOptions().getTechnologieFileSHM();
    AutomaticSizing::TechnologieFileSHM technologieFile;
    technologieFile.setPath("InputFileExamples/Synthesis/TechnologieFile.xml");
    technologieFile.parse(*circuitInformation);



    //logDebug("Creating device level");
    const Synthesis::DeviceLevel * deviceLevel = new Synthesis::DeviceLevel();
    //logDebug("Creating Structural level");
    const Synthesis::StructuralLevel * structuralLevel =  new Synthesis::StructuralLevel(*deviceLevel, *circuitInformation);
    // std::cout << loadParts_->toStr() << std::endl;

    Synthesis::LoadParts * loadParts_ = new Synthesis::LoadParts(*structuralLevel);
    for(auto & loadPart : loadParts_->createLoadPartsPmosTwoTransistorCurrentBiasesDifferentSources())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printTerminals(oss);
        std::cout <<  oss.str();
    }



    for(auto & loadPart : loadParts_->createTwoTransistorsLoadPartsLoadPartsPmosVoltageBiases())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }

    for(auto & loadPart : loadParts_->createFourTransistorsLoadPartsLoadPartsPmosVoltageBiases())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }


    for(auto & loadPart : loadParts_->createLoadPartsPmosTwoTransistorCurrentBiasesDifferentSources())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }

    for(auto & loadPart : loadParts_->createLoadPartsPmosFourTransistorCurrentBiases())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }


    for(auto & loadPart : loadParts_->createLoadPartsPmosCurrentBiases())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }


    for(auto & loadPart : loadParts_->createLoadPartsPmosVoltageBiases())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }


    
    for(auto & loadPart : loadParts_->createLoadPartsPmosMixed())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }



    for(auto & loadPart : loadParts_->createLoadPartsPmosFourTransistorMixed())
    {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << loadPart->getCircuitIdentifier().toStr() << ": " << std::endl;
        std::ostringstream oss;
        loadPart->printBasicInfo(oss);
        std::cout <<  oss.str();
    }

    // g.2
    // std::vector<const Core::Circuit *> createLoadPartsPmosTwoTransistorCurrentBiasesDifferentSources() const; (1)
    // std::vector<const Core::Circuit *> createLoadPartsNmosTwoTransistorCurrentBiasesDifferentSources() const; (1)


    
    // std::vector<const Core::Circuit *> createTwoTransistorsLoadPartsLoadPartsPmosVoltageBiases() const;
    // std::vector<const Core::Circuit *> createTwoTransistorsLoadPartsLoadPartsNmosVoltageBiases() const;
    // std::vector<const Core::Circuit *> createFourTransistorsLoadPartsLoadPartsPmosVoltageBiases() const;
    // std::vector<const Core::Circuit *> createFourTransistorsLoadPartsLoadPartsNmosVoltageBiases() const;


    // std::vector<const Core::Circuit *> createLoadPartsPmosFourTransistorCurrentBiases() const;
    // std::vector<const Core::Circuit *> createLoadPartsNmosFourTransistorCurrentBiases() const;
    // std::vector<const Core::Circuit *> createLoadPartsPmosCurrentBiases() const;
    // std::vector<const Core::Circuit *> createLoadPartsNmosCurrentBiases() const;
    // std::vector<const Core::Circuit *> createLoadPartsPmosVoltageBiases() const;
    // std::vector<const Core::Circuit *> createLoadPartsNmosVoltageBiases() const;
    // std::vector<const Core::Circuit *> createLoadPartsPmosMixed() const;
    // std::vector<const Core::Circuit *> createLoadPartsNmosMixed() const;
    // std::vector<const Core::Circuit *> createLoadPartsPmosFourTransistorMixed() const;
    // std::vector<const Core::Circuit *> createLoadPartsNmosFourTransistorMixed() const;

    std::cout << "Done!";
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}