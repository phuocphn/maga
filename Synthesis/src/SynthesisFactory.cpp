/**
 * @file: SynthesisFactory.cpp
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
#include "Synthesis/incl/SynthesisFactory.h"
#include "Synthesis/incl/Synthesis.h"
#include "Core/incl/Common/BacktraceAssert.h"

namespace Synthesis {

    SynthesisFactory::SynthesisFactory():
            Control::AnalysisFactory("synthesis","analog circuit synthesis"),
            localOptions_("Allowed options for analog circuit synthesis")
    {
    }

    SynthesisFactory::~SynthesisFactory()
    {
    }

    std::string SynthesisFactory::getLocalOptionsStringDescription() const
    {
        return localOptions_.getStringDescription();
    }

    Control::OptionsBase & SynthesisFactory::getLocalOptions()
    {
        return localOptions_;
    }

    SynthesisFactory* SynthesisFactory::getInstance()
    {
        static boost::scoped_ptr<SynthesisFactory> theInst(new SynthesisFactory());
        return theInst.get();
    }

    Control::AbstractAnalysis* SynthesisFactory::create() const
    {
        Synthesis* analysis = new Synthesis();
        assert(analysis);
        return analysis;
    }

}


