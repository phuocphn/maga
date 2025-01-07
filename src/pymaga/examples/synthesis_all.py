import pymaga
import os

from loguru import logger


def synthesis_level2():
    if True:
        circuitInformation = pymaga.CircuitInformation()
        circuitParameter = pymaga.CircuitParameter()

        # set diffential amplifiers
        circuitParameter.setFullyDifferential(True)
        circuitInformation.setCircuitParameter(circuitParameter)
    else:
        circuitInformation = pymaga.CircuitInformation().init()

    # Initialize HL1
    deviceLevel = pymaga.DeviceLevel()

    differentialpairs = pymaga.DifferentialPair(deviceLevel)
    # print(differentialpairs)

    currentbiases = pymaga.CurrentBiases(deviceLevel)
    # print(currentbiases)

    voltagebiases = pymaga.VoltageBiases(deviceLevel)
    # print(voltagebiases)

    structuralLevel = pymaga.StructuralLevel(deviceLevel, circuitInformation)
    analoginverters = pymaga.AnalogInverters(structuralLevel)
    print(analoginverters)


def synthesis_level3():
    if True:
        circuitInformation = pymaga.CircuitInformation()
        circuitParameter = pymaga.CircuitParameter()

        # set diffential amplifiers
        circuitParameter.setFullyDifferential(True)
        circuitInformation.setCircuitParameter(circuitParameter)
    else:
        circuitInformation = pymaga.CircuitInformation().init()

    # initilize HL1
    deviceLevel = pymaga.DeviceLevel()

    # initilize HL2
    structuralLevel = pymaga.StructuralLevel(deviceLevel, circuitInformation)

    transconductances = pymaga.Transconductances(structuralLevel, circuitInformation)
    transconductances.initializeAllTransconductances(structuralLevel)
    # print(transconductances)

    """
    **Stage Bias**
    - Function: A stage bias supplies a transconductance with defined current. 
    - There are two types exist, but in the current implementation, we only have stage biases with current output.
    """
    stagebiases = pymaga.StageBiases(structuralLevel)
    # print(stagebiases)

    loadparts = pymaga.LoadParts(structuralLevel)
    # print(loadparts)

    amplificationStagesSubBlockLevel = pymaga.AmplificationStagesSubBlockLevel(
        structuralLevel, circuitInformation
    )
    loads = pymaga.Loads(amplificationStagesSubBlockLevel)
    print(loads)


def synthesis_level4():
    if True:
        circuitInformation = pymaga.CircuitInformation()
        circuitParameter = pymaga.CircuitParameter()

        # set diffential amplifiers
        circuitParameter.setFullyDifferential(True)
        # circuitParameter.setComplementary(True)
        circuitInformation.setCircuitParameter(circuitParameter)
    else:
        circuitInformation = pymaga.CircuitInformation().init()

    # initilize HL1
    deviceLevel = pymaga.DeviceLevel()

    # initilize HL2
    structuralLevel = pymaga.StructuralLevel(deviceLevel, circuitInformation)

    # initialize HL3
    amplificationStagesSubBlockLevel = pymaga.AmplificationStagesSubBlockLevel(
        structuralLevel, circuitInformation
    )

    # initialize HL4
    nonInvertingStages = pymaga.NonInvertingStages(
        amplificationStagesSubBlockLevel, circuitInformation
    )
    # print(nonInvertingStages)

    invertingStages = pymaga.InvertingStages(structuralLevel)
    print(invertingStages)


def synthesis_level5():
    if True:
        circuitInformation = pymaga.CircuitInformation()
        circuitParameter = pymaga.CircuitParameter()

        # set diffential amplifiers
        circuitParameter.setFullyDifferential(True)
        # circuitParameter.setComplementary(True)
        circuitInformation.setCircuitParameter(circuitParameter)
    else:
        circuitInformation = pymaga.CircuitInformation().init()

    # initilize HL1
    deviceLevel = pymaga.DeviceLevel()

    # initilize HL2
    structuralLevel = pymaga.StructuralLevel(deviceLevel, circuitInformation)

    # initialize HL3
    amplificationStagesSubBlockLevel = pymaga.AmplificationStagesSubBlockLevel(
        structuralLevel, circuitInformation
    )

    # initialize HL4
    amplificationStageLevel = pymaga.AmplificationStageLevel(
        amplificationStagesSubBlockLevel, structuralLevel, circuitInformation
    )

    # initalize HL5
    currentBiases = pymaga.CurrentBiases(deviceLevel)
    voltageBiases = pymaga.VoltageBiases(deviceLevel)
    cap = pymaga.Capacitor()
    opAmps = pymaga.OpAmps(amplificationStageLevel, currentBiases, voltageBiases, cap)
    print(opAmps)


if __name__ == "__main__":
    logger.info(f"current dir: {os.getcwd()}")
    synthesis_level5()
