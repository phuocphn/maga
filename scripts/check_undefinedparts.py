import xml.etree.ElementTree as ET
import glob

import os


def check_undefinedparts(delete=False):
    num_errors = 0
    for PATH in glob.glob(
        "outputs/TopologyGen/SingleOutputOpAmps/partitioning.result/*.xml"
    ):
        # PATH = "outputs/TopologyGen_08_03_2025/FullyDifferentialOpAmps/partitioning.result/three_stage_fully_differential_op_amp_1_2_1.xml"
        tree = ET.parse(PATH)
        root = tree.getroot()
        partitioning_result = root[1]

        undefinedParts = partitioning_result.findall("undefinedParts")
        if len(undefinedParts) == 1:
            # print("Found undefined parts in the partitioning result:")
            print(PATH, "found: ", undefinedParts[0].text)
            if len(undefinedParts[0]) > 0:
                print("error: ", PATH)
                if delete:
                    os.remove(PATH)
                    os.remove(
                        PATH.replace("partitioning.result/", "").replace(".xml", ".ckt")
                    )
                    os.remove(
                        PATH.replace(
                            "partitioning.result/", "structural_recognition.result/"
                        )
                    )

                num_errors += 1
        else:
            print("No/or more undefined parts found in the partitioning result")
        # print(partitioning_result.tag)
    print(f"{num_errors} errors found")


check_undefinedparts()
