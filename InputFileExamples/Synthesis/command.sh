#!/bin/sh

/home/pham/code/acst/build/bin/acst.sh --log-level-console DEBUG --analysis synthesis --xml-structrec-library-file ../../StructRec/xml/AnalogLibrary.xml  --device-types-file deviceTypes.xcat --xml-technologie-file TechnologieFile.xml --xml-circuit-information-file CircuitSpecifications.xml --transistor-model SHM --scaling 1mum --HSPICE-netlist-dir HspiceNetlist

