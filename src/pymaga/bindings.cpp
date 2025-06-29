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

#include "Synthesis/incl/Library/HierarchyLevel1/DeviceLevel.h"
#include "Synthesis/incl/Library/HierarchyLevel1/Capacitor.h"
#include "Synthesis/incl/Library/HierarchyLevel2/StructuralLevel.h"
#include "Synthesis/incl/Library/HierarchyLevel2/DifferentialPair.h"
#include "Synthesis/incl/Library/HierarchyLevel2/CurrentBiases.h"
#include "Synthesis/incl/Library/HierarchyLevel2/VoltageBiases.h"
#include "Synthesis/incl/Library/HierarchyLevel2/AnalogInverters.h"
#include "Synthesis/incl/Library/HierarchyLevel3/AmplificationStagesSubBlockLevel.h"
#include "Synthesis/incl/Library/HierarchyLevel3/Transconductances.h"
#include "Synthesis/incl/Library/HierarchyLevel3/Loads.h"
#include "Synthesis/incl/Library/HierarchyLevel3/LoadParts.h"
#include "Synthesis/incl/Library/HierarchyLevel3/StageBiases.h"
#include "Synthesis/incl/Library/HierarchyLevel4/AmplificationStageLevel.h"
#include "Synthesis/incl/Library/HierarchyLevel4/InvertingStages.h"
#include "Synthesis/incl/Library/HierarchyLevel4/NonInvertingStages.h"
#include "Synthesis/incl/Library/HierarchyLevel5/OpAmps.h"
#include "Core/incl/Circuit/Circuit.h"
#include "Core/incl/Circuit/CircuitId/CircuitIds.h"
#include "Core/incl/Circuit/Net/NetId/NetName.h"
#include "Core/incl/Circuit/Instance/InstanceTerminal.h"
#include "src/pymaga/AcstUtility.h"


#include "Core/incl/Circuit/Instance/Instance.h"
#include "Core/incl/Circuit/Terminal/TerminalId/TerminalName.h"

#include "Synthesis/incl/TopoGen.h"

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

    py::class_<Synthesis::DeviceLevel>(m, "DeviceLevel")
        .def(py::init<>());
    
    py::class_<Synthesis::Capacitor>(m, "Capacitor")
        .def(py::init<>());

    py::class_<Synthesis::StructuralLevel>(m, "StructuralLevel")
        .def(py::init<const Synthesis::DeviceLevel&, const AutomaticSizing::CircuitInformation &>());  

    // level 2
    py::class_<Synthesis::DifferentialPair>(m, "DifferentialPair")
        .def(py::init<const Synthesis::DeviceLevel&>())
        .def("__repr__", [](Synthesis::DifferentialPair &self) {
            return self.toStr();
        });
    py::class_<Synthesis::CurrentBiases>(m, "CurrentBiases")
        .def(py::init<const Synthesis::DeviceLevel&>())
        .def("__repr__", [](Synthesis::CurrentBiases &self) {
            return self.toStr();
        });
    py::class_<Synthesis::VoltageBiases>(m, "VoltageBiases")
        .def(py::init<const Synthesis::DeviceLevel&>())
        .def("__repr__", [](Synthesis::VoltageBiases &self) {
            return self.toStr();
        });
    py::class_<Synthesis::AnalogInverters>(m, "AnalogInverters")
        .def(py::init<Synthesis::StructuralLevel&>())
        .def("__repr__", [](Synthesis::AnalogInverters &self) {
            return self.toStr();
        });


    // level 3
    py::class_<Synthesis::AmplificationStagesSubBlockLevel>(m, "AmplificationStagesSubBlockLevel")
        .def(py::init<const Synthesis::StructuralLevel&, const AutomaticSizing::CircuitInformation &>());  

    py::class_<Synthesis::Transconductances>(m, "Transconductances")
        .def(py::init<const Synthesis::StructuralLevel&, const AutomaticSizing::CircuitInformation &>())
        .def("__repr__", [](Synthesis::Transconductances &self) {
            return self.toStr();
        })
        .def("initializeAllTransconductances", [](Synthesis::Transconductances &self, const Synthesis::StructuralLevel & structuralLevel) {
            return self.initializeAllTransconductances(structuralLevel);
        });

    py::class_<Synthesis::LoadParts>(m, "LoadParts")
        .def(py::init<const Synthesis::StructuralLevel&>())
        .def("__repr__", [](Synthesis::LoadParts &self) {
            return self.toStr();
        });

    py::class_<Synthesis::StageBiases>(m, "StageBiases")
        .def(py::init<const Synthesis::StructuralLevel&>())
        .def("__repr__", [](Synthesis::StageBiases &self) {
            return self.toStr();
        });

    py::class_<Synthesis::Loads>(m, "Loads")
        .def(py::init<Synthesis::AmplificationStagesSubBlockLevel&>())
        .def("__repr__", [](Synthesis::Loads &self) {
            return self.toStr();
        })
        .def("createSimpleMixedLoadPmos", [](Synthesis::Loads &self) {
            return self.createSimpleMixedLoadPmos();
        })
        .def("createSimpleMixedLoadNmos", [](Synthesis::Loads &self) {
            return self.createSimpleMixedLoadNmos();
        })
        .def("createSimpleTwoLoadPartsFoldedGCCMixedLoadPmos", [](Synthesis::Loads &self) {
            return self.createSimpleTwoLoadPartsFoldedGCCMixedLoadPmos();
        })        
        .def("createSimpleTwoLoadPartsFoldedGCCMixedLoadNmos", [](Synthesis::Loads &self) {
            return self.createSimpleTwoLoadPartsFoldedGCCMixedLoadNmos();
        })            
        .def("createLoadsTwoLoadPartsCascodeGCCMixedPmos", [](Synthesis::Loads &self) {
            return self.createLoadsTwoLoadPartsCascodeGCCMixedPmos();
        }) 
        .def("createLoadsTwoLoadPartsCascodeGCCMixedNmos", [](Synthesis::Loads &self) {
            return self.createLoadsTwoLoadPartsCascodeGCCMixedNmos();
        }) 
        .def("createLoadsTwoLoadPartsMixedCurrentBiasesPmos", [](Synthesis::Loads &self) {
            return self.createLoadsTwoLoadPartsMixedCurrentBiasesPmos();
        }) 
        .def("createLoadsTwoLoadPartsMixedCurrentBiasesNmos", [](Synthesis::Loads &self) {
            return self.createLoadsTwoLoadPartsMixedCurrentBiasesNmos();
        }) 
        .def("createLoadsPmosForFullyDifferentialNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsPmosForFullyDifferentialNonInvertingStage();
        }) 
        .def("createLoadsNmosForFullyDifferentialNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsNmosForFullyDifferentialNonInvertingStage();
        }) 
        .def("createLoadsForComplementaryNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsForComplementaryNonInvertingStage();
        }) 
        .def("createLoadsPmosTwoForSymmetricalOpAmpNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsPmosTwoForSymmetricalOpAmpNonInvertingStage();
        }) 
        .def("createLoadsPmosFourForSymmetricalOpAmpNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsPmosFourForSymmetricalOpAmpNonInvertingStage();
        }) 
        .def("createLoadsNmosTwoForSymmetricalOpAmpNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsNmosTwoForSymmetricalOpAmpNonInvertingStage();
        }) 
        .def("createLoadsNmosFourForSymmetricalOpAmpNonInvertingStage", [](Synthesis::Loads &self) {
            return self.createLoadsNmosFourForSymmetricalOpAmpNonInvertingStage();
        }) 
        ;


    // level 4
    py::class_<Synthesis::AmplificationStageLevel>(m, "AmplificationStageLevel")
        .def(py::init<const Synthesis::AmplificationStagesSubBlockLevel & ,
                        const Synthesis::StructuralLevel & , const AutomaticSizing::CircuitInformation & >())
        .def("getNonInvertingStages", [](Synthesis::AmplificationStageLevel &self) {
            return self.getNonInvertingStages();
        })
        .def("__repr__", [](Synthesis::AmplificationStageLevel &self) {
            return self.toStr();
        });

    py::class_<Synthesis::NonInvertingStages>(m, "NonInvertingStages")
        .def(py::init<const Synthesis::AmplificationStagesSubBlockLevel &, const AutomaticSizing::CircuitInformation &>())
        .def("__repr__", [](Synthesis::NonInvertingStages &self) {
            return self.toStr();
        })
        .def("createSimpleNonInvertingStages", [](Synthesis::NonInvertingStages &self, int caseNumber) {
            return self.createSimpleNonInvertingStages(caseNumber);
        }, py::arg("case_number"), py::return_value_policy::reference_internal);

    py::class_<Synthesis::InvertingStages>(m, "InvertingStages")
        .def(py::init<const Synthesis::StructuralLevel &>())
        .def("__repr__", [](Synthesis::InvertingStages &self) {
            return self.toStr();
        });

    // level 5
    py::class_<Synthesis::OpAmps>(m, "OpAmps")
        .def(py::init<const Synthesis::AmplificationStageLevel &, const Synthesis::CurrentBiases &, 
            const Synthesis::VoltageBiases & , const Synthesis::Capacitor & >(),
            
            
            py::keep_alive<1, 2>(),  // Keep AmplificationStageLevel alive
            py::keep_alive<1, 3>(),  // Keep CurrentBiases alive
            py::keep_alive<1, 4>(),  // Keep VoltageBiases alive
            py::keep_alive<1, 5>())  // Keep Capacitor alive
        // .def(py::init<std::shared_ptr<Synthesis::AmplificationStageLevel>, 
        //           std::shared_ptr<Synthesis::CurrentBiases>, 
        //           std::shared_ptr<Synthesis::VoltageBiases>, 
        //           std::shared_ptr<Synthesis::Capacitor>>());

        // .def("getAmplificationStageLevel", [](Synthesis::OpAmps &self) {
        //     std::cout << "getin";
        //     return self.getAmplificationStageLevel();
        // }, py::return_value_policy::take_ownership)

        .def("getAmplificationStageLevel", [](Synthesis::OpAmps &self) {
            return py::cast(self.getAmplificationStageLevel());
        }, py::return_value_policy::reference_internal)
        .def("createInstance", [](Synthesis::OpAmps &self, const Core::Circuit & circuit, std::string instanceName) {
            // const Core::InstanceName objInstanceName = Core::InstanceName(instanceName);
            return py::cast(self.createInstance(circuit, Synthesis::OpAmps::FIRSTSTAGE_)); 
            //py::cast(self.getAmplificationStageLevel());
        }, py::return_value_policy::reference_internal)

        .def("createSimpleOpAmp", [](Synthesis::OpAmps &self, int & index, Core::Instance & firstStage, 
									Core::Instance * secondStage) {
            return py::cast(self.createSimpleOpAmp(index, firstStage, nullptr));
        }, py::return_value_policy::reference_internal)
        .def("createSimpleOneStageOpAmps", [](Synthesis::OpAmps &self, int caseNumber, int & index) {
            return py::cast(self.createSimpleOneStageOpAmps(caseNumber, index));
        }, 
        py::arg("stage1_casenum"),
        py::arg("start_index"),
        py::return_value_policy::reference_internal);



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

    

    // Simple Definition
    py::class_<Core::Net>(m, "Net")
        .def(py::init<>()); 
    py::class_<Core::InstanceId>(m, "InstanceId")
        .def(py::init<>()); 
    py::class_<Core::InstanceTerminal>(m, "InstanceTerminal")
        .def(py::init<>())
        .def("__repr__", [](Core::InstanceTerminal &self) {
            return "undefined";
        })
        .def("getNet", [](Core::InstanceTerminal &self)
        {
            // return self.getNet();
            const auto& net = self.getNet();

            // TODO: can not return the Net object itself
            return net.toStr();
        })
        .def("getIdentifier", [](Core::InstanceTerminal &self) {
            return self.getIdentifier().toStr();
        });


    py::class_<Core::TerminalId>(m, "TerminalId")
        .def(py::init<>()); 
    py::class_<Core::Terminal>(m, "Terminal")
        .def(py::init<>())
        .def("getNet", [](Core::Terminal &self)
        {
            // return self.getNet();
            const auto& net = self.getNet();

            // TODO: can not return the Net object itself
            return net.toStr();
        })
        .def("__repr__", [](Core::Terminal &self) {
            return self.toStr();
        })
        .def("getIdentifier", [](Core::Terminal &self) {
            return self.getIdentifier().toStr();
        })
        ;
    py::class_<Core::Circuit::TerminalMap>(m, "TerminalMap")
        .def(py::init<>()); 
    py::class_<Core::Instance::InstanceTerminalMap>(m, "InstanceTerminalMap")
        .def(py::init<>()); 
        

    
    py::class_<Core::Instance>(m, "Instance")
        .def(py::init<>())
        .def("getInstanceTerminalMap", [](Core::Instance &self) {
            py::dict d;
            const auto& instanceTerminalMap = self.getInstanceTerminalMap();
            for (const auto& item : instanceTerminalMap) {
                d[py::cast(item.first)] = py::cast(item.second);
            }
            return d;
        })
        .def("__repr__", [](Core::Instance &self) {
            return self.toStr();
        })
        .def("hasMaster", [](Core::Instance &self) {
            return self.hasMaster();
        })
        .def("getMaster", [](Core::Instance &self) {
            // py::cast does not remove or modify instances in C++.
            // It only wraps the returned object for Python.
            // If self.getMaster().clone() returns a Circuit with no instances,
            // the issue is in the clone() or getMaster() implementation, not py::cast.
            return py::cast(self.getMaster());
        })
        ; 


    py::class_<Core::Circuit, std::shared_ptr<Core::Circuit>>(m, "Circuit")
        .def(py::init<>())
        .def("getTerminalMap", [](Core::Circuit &self) {
            py::dict d;
            const auto& terminalMap = self.getTerminalMap();
            for (const auto& item : terminalMap) {
                d[py::cast(item.first)] = py::cast(item.second);
            }
            return d;
        })
        .def("getInstanceMap", [](Core::Circuit &self) {
            py::dict d;
            const auto& instancelMap = self.getInstanceMap();
            for (const auto& item : instancelMap) {
                d[py::cast(item.first)] = py::cast(item.second);
            }
            return d;
        })

        .def("__repr__", [](Core::Circuit &self) {
            return self.getCircuitIdentifier().toStr();
        })
        .def("printBasicInfo", [](Core::Circuit &self) {
            std::ostringstream buf;

            self.printBasicInfo(buf);
            return buf.str();

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


    py::class_<Synthesis::TopoGen>(m, "TopologyLibraryGeneration")
            .def(py::init<>())
            .def("createFlatCircuit", 
                [](const Core::Circuit & circuit) {
                    Synthesis::TopoGen* analysis = new Synthesis::TopoGen();
                    return analysis->createFlatCircuit(circuit);
                },
                "create flat circuit from a hierarchial circuit", py::return_value_policy::take_ownership)
            .def("createAllSimpleOpAmps", 
                    [](Synthesis::TopoGen& self) {
                        self.createAllSimpleOpAmps();
            });


    #ifdef VERSION_INFO
	m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
	#else
	m.attr("__version__") = "dev";
	#endif
}