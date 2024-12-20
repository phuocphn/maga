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


#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)


namespace py = pybind11;
using namespace pybind11::literals;

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

            }, "Create a StructRec::Library given libFilePath and deviceFilePath");


    py::class_<Synthesis::FunctionalBlockLibrary>(m, "FunctionalBlockLibrary")
        .def(py::init<const AutomaticSizing::CircuitInformation&>(), py::arg("circuit_information"))
        .def("setStructRecLibrary", [](Synthesis::FunctionalBlockLibrary& self, StructRec::Library& library) {
            self.setStructRecLibrary(library);
        }, 
        py::arg("library"),
        "Set the StructRec::Library reference")
        .def("__repr__", [](Synthesis::FunctionalBlockLibrary &self) {
            return self.toStr();
        });

	#ifdef VERSION_INFO
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
	#else
	m.attr("__version__") = "dev";
	#endif
}