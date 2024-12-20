import pymaga
import os

from loguru import logger


def buid_fbb():
    # circuitInformation = pymaga.CircuitInformation().init()
    circuitInformation = pymaga.CircuitInformation()
    circuitParameter = pymaga.CircuitParameter()

    # set diffential amplifiers
    circuitParameter.setFullyDifferential(True)

    circuitInformation.setCircuitParameter(circuitParameter)

    # create structrec lib
    structRec_inst = pymaga.StructRec()
    structureLib = structRec_inst.createStructureLibrary(
        "StructRec/xml/AnalogLibrary.xml",
        "examples/SynthesisSmallLibrary/deviceTypes.xcat",
    )

    fbb = pymaga.FunctionalBlockLibrary(circuitInformation)
    fbb.setStructRecLibrary(library=structureLib)
    print(fbb)


if __name__ == "__main__":
    logger.info(f"current dir: {os.getcwd()}")
    buid_fbb()
