import pymaga
import os

from loguru import logger


def print_partitioning_result(circuit: pymaga.Circuit) -> pymaga.PartitioningResult:
    print("[B] Paritioning Result".center(100, "*"))
    circuitAnalysis = pymaga.CircuitAnalysis()
    partitioningResult = circuitAnalysis.getPartitioningResult(circuit)
    print(partitioningResult)
    print("[E] Paritioning Result".center(100, "*"))


def print_structurec(circuit: pymaga.Circuit) -> pymaga.StructRecResult:
    print("[B] Structure Result".center(100, "*"))
    structRec_inst = pymaga.StructRec()
    structureLib: pymaga.StructRec = structRec_inst.createStructureLibrary(
        "StructRec/xml/AnalogLibrary.xml",
        "examples/StructureRecognition/deviceTypes.xcat",
    )
    structureResult = structureLib.recognize(circuit)
    print(structureResult)
    print("[E] Structure Result".center(100, "*"))


if __name__ == "__main__":
    pymaga_io = pymaga.IOCore()

    # circuitInformation = pymaga.CircuitInformation()
    # circuitParameter = pymaga.CircuitParameter()
    # circuitParameter.setFullyDifferential(True)
    # circuitInformation.setCircuitParameter(circuitParameter)

    simple_input = """
** Generated for: hspiceD
** Generated on: Apr 12 09:34:33 2019
** Design library name: CascodeSymmetricalCMOSOTA
** Design cell name: cascodeSymmetricalCMOSOTA
** Design view name: schematic
.GLOBAL vdd! gnd!

.TEMP 25.0
.OPTION
+    ARTIST=2
+    INGOLD=2
+    PARHIER=LOCAL
+    PSF=2

** Library name: CascodeSymmetricalCMOSOTA
** Cell name: cascodeSymmetricalCMOSOTA
** View name: schematic
c1 outFirstStage out
c2 outSecondStage out
m1 outVoltageBiasXXpXX1 outSourceVoltageBiasXXnXX1 gnd! gnd! nmos
m2 outVoltageBiasXXpXX2 outSourceVoltageBiasXXnXX1 gnd! gnd! nmos
m3 FirstStageYout1 FirstStageYout1 gnd! gnd! nmos
m4 outFirstStage FirstStageYout1 gnd! gnd! nmos
m5 FirstStageYsourceTransconductance outVoltageBiasXXpXX2 vdd! vdd! pmos
m6 FirstStageYout1 in1 FirstStageYsourceTransconductance FirstStageYsourceTransconductance pmos
m7 outFirstStage in2 FirstStageYsourceTransconductance FirstStageYsourceTransconductance pmos
c3 out gnd!
m8 outSecondStage ibias outSourceVoltageBiasXXnXX1 outSourceVoltageBiasXXnXX1 nmos
m9 outSourceVoltageBiasXXnXX1 outSourceVoltageBiasXXnXX1 gnd! gnd! nmos
m10 outSecondStage outVoltageBiasXXpXX1 SecondStageYinnerTransconductance SecondStageYinnerTransconductance pmos
m11 SecondStageYinnerTransconductance outFirstStage vdd! vdd! pmos
m12 out outSecondStage gnd! gnd! nmos
m13 out outVoltageBiasXXpXX2 vdd! vdd! pmos
m14 ibias ibias VoltageBiasXXnXX1Yinner VoltageBiasXXnXX1Yinner nmos
m15 VoltageBiasXXnXX1Yinner outSourceVoltageBiasXXnXX1 gnd! gnd! nmos
m16 outVoltageBiasXXpXX1 outVoltageBiasXXpXX1 vdd! vdd! pmos
m17 outVoltageBiasXXpXX2 outVoltageBiasXXpXX2 vdd! vdd! pmos
.END
"""

    figure_2a_fucobo = """
** Generated for: hspiceD
** Generated on: Apr 12 09:34:33 2019
** Design library name: CascodeSymmetricalCMOSOTA
** Design cell name: cascodeSymmetricalCMOSOTA
** Design view name: schematic
.GLOBAL vdd! gnd!

.TEMP 25.0
.OPTION
+    ARTIST=2
+    INGOLD=2
+    PARHIER=LOCAL
+    PSF=2

** Library name: CascodeSymmetricalCMOSOTA
** Cell name: cascodeSymmetricalCMOSOTA
** View name: schematic
mP14 N009 Bias vdd! vdd! pmos
mP15 Bias Bias vdd! vdd! pmos
mP7 N005 Bias vdd! vdd! pmos
mP16 N007 Bias vdd! vdd! pmos
mP1 N012 in2 N005 N005 pmos
mP2 N013 in1 N005 N005 pmos
mP17 N004 N004 vdd! vdd! pmos
mP5 N002 Bias vdd! vdd! pmos
mP6 N001 Bias vdd! vdd! pmos
mP3 out2 N004 N002 N002 pmos
mP4 ou1 N004 N001 N001 pmos
mP12 N003 Bias vdd! vdd! pmos
mP13 N006 Bias vdd! vdd! pmos
mP9 N008 Vref N003 N003 pmos
mP8 N010 out2 N003 N003 pmos
mP11 N010 ou1 N006 N006 pmos
mP10 N008 Vref N006 N006 pmos
mN9 N009 N011 gnd! gnd! nmos
mN8 N004 N011 gnd! gnd! nmos
mN1 ou1 N007 N013 N013 nmos
mN2 out2 N007 N012 N012 nmos
mN3 N013 N008 gnd! gnd! nmos
mN4 N012 N008 gnd! gnd! nmos
mN5 N008 N008 gnd! gnd! nmos
mN6 N010 N010 gnd! gnd! nmos
mN7 N007 N007 gnd! gnd! nmos
.END
"""
    with open("/tmp/circuit_input.ckt", "w") as f:
        f.write(figure_2a_fucobo)

    circuit = pymaga_io.readInCircuit(
        circuit_filepath="/tmp/circuit_input.ckt",  # "examples/StructureRecognition/input.ckt",
        supplynet_filepath="examples/StructureRecognition/supplyNets.xcat",
        mapping_filepath="examples/StructureRecognition/HSpiceMapping.xcat",
        devicetype_filepath="examples/StructureRecognition/deviceTypes.xcat",
    )
    print(circuit)
    print_partitioning_result(circuit)
    print_structurec(circuit)
