        globalOptions_.parse(argc,argv);
        initializeLogger();


        if(globalOptions_.hasHelp())
        {
            globalOptions_.printHelp(std::cout);
            return;
        }
        