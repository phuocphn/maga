// #include "Control/incl/Analysis/AnalysisFactory.h"
#include "Control/incl/FactoryMap/AnalysesFactoryMap.h"
#include "Control/incl/Analysis/AnalysisFactory.h"


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

#include <iostream>

int main()
{
    std::string analysisType = "synthesis";
    Control::AnalysesFactoryMap* analysisFactoryMap = Control::AnalysesFactoryMap::getInstance();
    Control::AnalysisFactory* analysisFactory = analysisFactoryMap->find(Control::FactoryMapKey(analysisType));
    
    std::cout << "Test completed!";
    return 0;    
}