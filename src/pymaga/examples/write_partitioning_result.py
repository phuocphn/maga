import pymaga
import os
import glob

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
    except:
        print("error when passing circuit: ", fname)
        pass

if __name__ == "__main__":
    INPUT_NETLIST_DIR = "outputs/TopologyGen/FullyDifferentialOpAmps/"
    pymaga_io = pymaga.IOCore()

    for fname in glob.glob( os.path.join(INPUT_NETLIST_DIR, "*")):
        partition(fname)
