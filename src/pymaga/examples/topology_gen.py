import pymaga
import os

from loguru import logger


def topogen():
    topogen = pymaga.TopologyLibraryGeneration()
    topogen.createAllSimpleOpAmps()

if __name__ == "__main__":
    logger.info(f"current dir: {os.getcwd()}")
    topogen()
