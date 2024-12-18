#include <iostream>

#include "Core/incl/Common/Backtrace.h"
#include "Control/incl/Control.h"
#include "Log/incl/LogMacros.h"
#include <iostream>
#include <time.h>
#include <thread>
#include <vector>
#include <mutex>

int main(int argc, char *argv[]) {

    try{
    	auto start = std::chrono::high_resolution_clock::now();
        Core::installBacktraceExceptionHandler();
        Control::Control control;

        if (argc == 1)
        { 
            argc = 27;
            char* argv_default[argc];

            argv_default[0] = "acst";
            argv_default[1] = "--log-level-console";
            argv_default[2] = "DEBUG";

            argv_default[3] = "--analysis";
            argv_default[4] = "automaticsizing";


            argv_default[5] = "--circuit-netlist";
            argv_default[6] = "examples/AutomaticSizing/singleOutputOpAmp/complementaryOpAmp.hspice";

            argv_default[7] = "--output-file";
            argv_default[8] = "examples/AutomaticSizing/singleOutputOpAmp/complementaryOpAmp.xml";

            argv_default[9] = "--xml-structrec-library-file";
            argv_default[10] = "StructRec/xml/AnalogLibrary.xml";

            argv_default[11] = "--hspice-mapping-file";
            argv_default[12] = "examples/AutomaticSizing/singleOutputOpAmp/HSpiceMapping.xcat";

            argv_default[13] = "--hspice-supplynet-file";
            argv_default[14] = "examples/AutomaticSizing/singleOutputOpAmp/supplyNets.xcat";

            argv_default[15] = "--device-types-file";
            argv_default[16] = "examples/AutomaticSizing/singleOutputOpAmp/deviceTypes.xcat";


            argv_default[17] = "--xml-technologie-file";
            argv_default[18] = "examples/AutomaticSizing/singleOutputOpAmp/TechnologyFile.xml";


            argv_default[19] = "--xml-circuit-information-file";
            argv_default[20] = "examples/AutomaticSizing/singleOutputOpAmp/CircuitParameterAndSpecifications.xml";

            argv_default[21] = "--transistor-model";
            argv_default[22] = "SHM";

            argv_default[23] = "--scaling";
            argv_default[24] = "1mum";

            argv_default[25] = "--runtime";
            argv_default[26] = "5";

            control.run(argc,argv_default);

        }
        else
        {
            control.run(argc,argv);

        }

    	auto end = std::chrono::high_resolution_clock::now();
    	auto diff = end - start;
    	int timeHours = std::chrono::duration_cast<std::chrono::hours>(diff).count();
    	int timeMinutes = std::chrono::duration_cast<std::chrono::minutes>(diff).count() - timeHours * 60;
    	int timeSeconds = std::chrono::duration_cast<std::chrono::seconds>(diff).count() - timeHours * 3600 - timeMinutes * 60;
    	logDebug("\n Program runtime: " << timeHours << "h " << timeMinutes <<"min " << timeSeconds << "s");
        return 0;
    }
    catch(Core::BacktraceException* ex)
    {
        std::cerr << *ex << std::endl;
        delete ex;
        return -1;
    }


    return 0;
}