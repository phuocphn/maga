#!/bin/sh

../../build/bin/acst.sh --log-level-console DEBUG  --circuit-netlist cascodedSymmetricalCMOSOTA.hspice --output-file cascodedSymmetricalCMOSOTA.xml --analysis partitioning --xml-structrec-library-file ../../StructRec/xml/AnalogLibrary.xml --hspice-mapping-file HSpiceMapping.xcat --hspice-supplynet-file supplyNets.xcat --device-types-file deviceTypes.xcat
