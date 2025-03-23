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
            argc = 17;
            char* argv_default[argc];

            argv_default[0] = "acst";
            argv_default[1] = "--log-level-console";
            argv_default[2] = "DEBUG";

            argv_default[3] = "--analysis";
            argv_default[4] = "partitioning";


            argv_default[5] = "--circuit-netlist";
            argv_default[6] = "examples/Partitioning/input.hspice";

            argv_default[7] = "--output-file";
            argv_default[8] = "examples/Partitioning/output.xml";

            argv_default[9] = "--xml-structrec-library-file";
            argv_default[10] = "StructRec/xml/AnalogLibrary.xml";

            argv_default[11] = "--hspice-mapping-file";
            argv_default[12] = "examples/Partitioning/HSpiceMapping.xcat";

            argv_default[13] = "--hspice-supplynet-file";
            argv_default[14] = "examples/Partitioning/supplyNets.xcat";

            argv_default[15] = "--device-types-file";
            argv_default[16] = "examples/Partitioning/deviceTypes.xcat";

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