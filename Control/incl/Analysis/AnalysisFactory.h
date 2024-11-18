/**
 * @file: AnalysisFactory.h
 * @author: Inga Abel <inga.abel@tum.de>, Maximilian Neuner <maximilian.neuner@tum.de>, Michael Eick <michael.eick@tum.de>
 */


/**************************************************************************************************
 *  Main authors:
 *     Inga Abel <inga.abel@tum.de>, 
 *	   Maximilian Neuner <maximilian.neuner@tum.de>, 
 *     Michael Eick <michael.eick@tum.de>
 *
 * 
 *  Copyright (C) 2021
 *  Chair of Electronic Design Automation
 *  Univ.-Prof. Dr.-Ing. Ulf Schlichtmann
 *  TU Muenchen
 *  Arcisstrasse 21
 *  D-80333 Muenchen
 *  Germany
 *
 *  This file is part of acst, a analog circuit analysis, sizing and synthesis enviroment:
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *************************************************************************************************/


#ifndef CONTROL_ANALYSISFACTORY_H_
#define CONTROL_ANALYSISFACTORY_H_

#include "Control/incl/FactoryMap/FactoryBase.h"
#include "Control/incl/Options/OptionsBase.h"
#include <string>

#include "AbstractAnalysis.h"

namespace Control {

    class AnalysisFactory : public FactoryBase
    {
    protected:
        AnalysisFactory(const std::string& name,const std::string& desc);

    public:
        virtual ~AnalysisFactory();

        virtual std::string getLocalOptionsStringDescription() const = 0;

        /**
         * @brief Get the Local Options object. New classes extend this base `AnalysisFactory` need to define this method,
         * which will return specified options that only available for this class/component.
         * 
         * New classes/components such as [Synthesis|StructRec]Factory.[h/cpp] will contains a "private" variable name localOptions_ (type: [Synthesis|StructRec]LocalOptions)
         * to store local options. We can see file with the same name "LocalOptions.h" in all component folders (all inherited from the base classes: Control::OptionsBase/ Control::HspiceOptions)
         * @return OptionsBase& 
         */
        virtual OptionsBase & getLocalOptions() = 0;
        virtual AbstractAnalysis* create() const = 0;

    };


}
#endif
