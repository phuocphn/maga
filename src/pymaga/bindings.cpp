#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

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
#include "AutomaticSizing/incl/CircuitInformation/DesignAttributes.h"

#include "AutomaticSizing/incl/CircuitInformation/CircuitInformation.h"
#include "AutomaticSizing/incl/CircuitInformation/CircuitParameter.h"
#include "Synthesis/incl/CircuitSpecificationFile/CircuitSpecificationFile.h"

#include "Partitioning/incl/Partitioning.h"
#include "Synthesis/incl/Library/FunctionalBlockLibrary.h"

#include "StructRec/incl/Library/Library.h"
#include "StructRec/incl/LibraryFile/LibraryFile.h"
#include "Core/incl/DeviceTypeRegister/DeviceTypeRegister.h"
#include "Core/incl/Common/Object.h"




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

#include "HSpice/incl/InputFile/InputFile.h"
#include "HSpice/incl/InputFile/SupplyNet/SupplyNetFile.h"
#include "Core/incl/DeviceTypeRegister/DeviceTypeRegister.h"
#include "Partitioning/incl/Results/Result.h"
#include "StructRec/incl/Results/Result.h"

#include "src/pymaga/AcstUtility.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;
using namespace pybind11::literals;

class DerivedHspiceOptions {
public:
    DerivedHspiceOptions()  {}
};

class CircuitAnalysis {
    public:
    CircuitAnalysis() {}
};   

PYBIND11_MODULE(pymaga, m) {
    m.doc() = "Python binding for the maga.";
    py::class_<AutomaticSizing::DesignAttributes>(m, "DesignAttributes")
        .def(py::init<>()); 


    py::class_<AutomaticSizing::CircuitInformation>(m, "CircuitInformation")
        .def(py::init<>())
        .def("getDesignAttributes", &AutomaticSizing::CircuitInformation::getDesignAttributes)
        .def("getCircuitSpecification", &AutomaticSizing::CircuitInformation::getCircuitSpecification)
        .def("setDesignAttributes", &AutomaticSizing::CircuitInformation::setDesignAttributes)
        .def("setCircuitSpecification", &AutomaticSizing::CircuitInformation::setCircuitSpecification)
        .def("setCircuitParameter", &AutomaticSizing::CircuitInformation::setCircuitParameter)
        .def("init", [](AutomaticSizing::CircuitInformation) {
                AutomaticSizing::CircuitInformation * circuitInformation = new AutomaticSizing::CircuitInformation;
                AutomaticSizing::CircuitParameter * circuitParameter = new AutomaticSizing::CircuitParameter;
                circuitInformation->setCircuitParameter(*circuitParameter);
                
			    return circuitInformation;
			}, "init circuitInformation.");

    py::class_<AutomaticSizing::CircuitParameter>(m, "CircuitParameter")
        .def(py::init<>())
        .def("setFullyDifferential", [](AutomaticSizing::CircuitParameter& self, bool flag) {
            self.setFullyDifferential(flag);
        })
        .def("setComplementary", [](AutomaticSizing::CircuitParameter& self, bool flag) {
            self.setComplementary(flag);
        }); 

    py::class_<StructRec::Library>(m, "StructRec")
        .def(py::init<>())
        .def("createStructureLibrary", [](StructRec::Library& self, std::string libFilePath, std::string deviceFilePath) {

                StructRec::LibraryFile structRecLibraryFile;
                structRecLibraryFile.setPath(libFilePath);

                // create device register
                Core::DeviceTypesFile deviceTypesFile; 
                deviceTypesFile.setPath(deviceFilePath);
                Core::DeviceTypeRegister * deviceTypeRegister = deviceTypesFile.parse();

                structRecLibraryFile.setDeviceTypeRegister(*deviceTypeRegister);
                StructRec::Library* structRecLibrary = structRecLibraryFile.parse();
                return structRecLibrary;

            }, "Create a StructRec::Library given libFilePath and deviceFilePath")
        .def("recognize", [](StructRec::Library& self, Core::Circuit &circuit) {
                return self.recognize(circuit);
            }, "Run structure recognition");

    py::class_<Synthesis::FunctionalBlockLibrary>(m, "FunctionalBlockLibrary")
        .def(py::init<const AutomaticSizing::CircuitInformation&>(), py::arg("circuit_information"))
        .def("setStructRecLibrary", [](Synthesis::FunctionalBlockLibrary& self, StructRec::Library& library) {
            self.setStructRecLibrary(library);
        }, 
        py::arg("library"),
        "Set the StructRec::Library reference")
        .def("__repr__", [](Synthesis::FunctionalBlockLibrary &self) {
            return self.toStr();
        })
        .def("getStructuralLevel", [](Synthesis::FunctionalBlockLibrary &self) {
            return self.getStructuralLevel();
        }, py::return_value_policy::take_ownership)
        .def("getDeviceLevel", [](Synthesis::FunctionalBlockLibrary &self) {
            return self.getDeviceLevel();
        }, py::return_value_policy::take_ownership);

    
    py::class_<Core::Circuit, std::shared_ptr<Core::Circuit>>(m, "Circuit")
        .def(py::init<>())
        .def("__repr__", [](Core::Circuit &self) {
            return self.toStr();
        });

    py::class_<DerivedHspiceOptions>(m, "IOCore")
        .def(py::init<>())
        .def("readInCircuit", 
            [](DerivedHspiceOptions& self, const std::string& circuitFilePath, 
            const std::string& supplyNetFilePath, const std::string& mappingFilePath, 
            const std::string& deviceFilePath) {
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

                // Core::Circuit& circuit = *hspiceInputFile->readNewCircuit();
                            std::shared_ptr<Core::Circuit> circuit = 
                std::make_shared<Core::Circuit>(*hspiceInputFile->readNewCircuit());
                delete hspiceInputFile;
                return circuit; // Python owns the shared_ptr
            },
            py::arg("circuit_filepath"), 
            py::arg("supplynet_filepath"), 
            py::arg("mapping_filepath"), 
            py::arg("devicetype_filepath"),
            "Read Hspice circuit and convert to a Core::Circuit object.", py::return_value_policy::take_ownership);

    py::class_<Partitioning::Result>(m, "PartitioningResult")
        .def(py::init<>())
        .def("__repr__", [](Partitioning::Result &self) {
            return self.toStr();
        })
        .def("writeXmlPartitioningResult", [](Partitioning::Result &self, const std::string& filePath) {
            Control::OutputFile outputFile1;
            outputFile1.setPath(filePath);
            return self.writeXmlPartitioningResult(outputFile1);
        });
	
    py::class_<StructRec::Result>(m, "StructRecResult")
        .def(py::init<>())
        .def("__repr__", [](StructRec::Result &self) {
            return self.toStr();
        })
        .def("writeXml", [](StructRec::Result &self, const std::string& filePath) {
            Control::OutputFile outputFile1;
            outputFile1.setPath(filePath);
            return self.writeXml(outputFile1);
        });

    py::class_<CircuitAnalysis>(m, "CircuitAnalysis")
        .def(py::init<>())
        .def("getPartitioningResult", 
            [](CircuitAnalysis& self, const Core::Circuit & circuit) {
                return getPartitioningResult(circuit);
            },
            "Read Hspice circuit and return Partition::Result object.", py::return_value_policy::take_ownership);

    
    
    #ifdef VERSION_INFO
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
	#else
	m.attr("__version__") = "dev";
	#endif
}