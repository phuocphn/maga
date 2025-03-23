import xml.etree.ElementTree as ET
import glob
import os
import pymaga
import os
from pathlib import Path

from loguru import logger


def print_partitioning_result(circuit: pymaga.Circuit) -> pymaga.PartitioningResult:
    print("[B] Paritioning Result".center(100, "*"))
    circuitAnalysis = pymaga.CircuitAnalysis()
    partitioningResult = circuitAnalysis.getPartitioningResult(circuit)
    print(partitioningResult)
    print("[E] Paritioning Result".center(100, "*"))
    print("write to file")
    partitioningResult.writeXmlPartitioningResult("/tmp/rec.xml")


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


def write_partitioning_result(circuit: pymaga.Circuit, save_path):
    circuitAnalysis = pymaga.CircuitAnalysis()
    partitioningResult = circuitAnalysis.getPartitioningResult(circuit)
    print(partitioningResult)
    partitioningResult.writeXmlPartitioningResult(save_path)


def partition(
    fname,
    processed_dir="outputs/processed_circuits",
    output_dir="outputs/res.partitioning",
):
    Path(processed_dir).mkdir(parents=True, exist_ok=True)
    Path(output_dir).mkdir(parents=True, exist_ok=True)

    with open(fname, "r") as f:
        netlist_data = "".join(f.readlines()[1:-2])

    netlist_data = netlist_data.replace("sourceNmos", "gnd!")
    netlist_data = netlist_data.replace("sourcePmos", "vdd!")

    updated_netlist = f"""
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
{netlist_data.strip()}
.END
""".strip()
    print(updated_netlist)
    print(os.path.basename(fname))

    new_circuit_fname = os.path.join(
        processed_dir,
        os.path.basename(fname),
    )
    partitioning_fname = os.path.join(
        output_dir,
        os.path.basename(fname).replace(".ckt", ".xml"),
    )
    with open(new_circuit_fname, "w") as f:
        f.write(updated_netlist)

    circuit = pymaga_io.readInCircuit(
        circuit_filepath=new_circuit_fname,  # "examples/StructureRecognition/input.ckt",
        supplynet_filepath="examples/StructureRecognition/supplyNets.xcat",
        mapping_filepath="examples/StructureRecognition/HSpiceMapping.xcat",
        devicetype_filepath="examples/StructureRecognition/deviceTypes.xcat",
    )
    try:
        write_partitioning_result(circuit, partitioning_fname)
        return True
    except:
        print("error when passing circuit: ", fname)
        return False


# --------------


INPUT_NETLIST_DIR = "outputs/TopologyGen/SingleOutputOpAmps/"
ERROR_FILE = os.path.join(INPUT_NETLIST_DIR, "partitioning.errors.txt")
PROCCESSED_DIR = os.path.join(INPUT_NETLIST_DIR, "partitioning.processed_netlist")
OUTPUT_DIR = os.path.join(INPUT_NETLIST_DIR, "partitioning.result")
check_exist = False
pymaga_io = pymaga.IOCore()


num_errors = 0
for PATH in glob.glob(
    "outputs/TopologyGen/SingleOutputOpAmps/partitioning.result/*.xml"
):
    tree = ET.parse(PATH)
    root = tree.getroot()
    partitioning_result = root[1]

    undefinedParts = partitioning_result.findall("undefinedParts")
    if len(undefinedParts) == 1:
        print(PATH, "found: ", undefinedParts[0].text)
        if len(undefinedParts[0]) > 0:
            print("error: ", PATH)
            ret = partition(
                PATH.replace("partitioning.result/", "").replace(".xml", ".ckt"),
                processed_dir=PROCCESSED_DIR,
                output_dir=OUTPUT_DIR,
            )
            num_errors += 1
    else:
        print("No/or more undefined parts found in the partitioning result")
print(f"{num_errors} errors found")
