/**
 * @file: AbstractAnalysis.h
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

 
#ifndef CONTROL_ABSTRACTANALYSIS_H_
#define CONTROL_ABSTRACTANALYSIS_H_

#include "Control/incl/File/OutputFile.h"
#include "Control/incl/FileFormat/OutputFileFormat.h"
#include "HSpice/incl/InputFile/SupplyNet/SupplyNetIdentifiers.h"
#include "HSpice/incl/DeviceLineMappingFile/DeviceLineMapper.h"
namespace Core {
    class Circuit;
    class DeviceTypeRegister;
}

namespace Control {

    class OptionsBase; // "forward declaration" (https://stackoverflow.com/a/4757628)
    class AbstractResult;
    
    class AbstractAnalysis
    {
    public:
        AbstractAnalysis();
        virtual ~AbstractAnalysis();

        virtual void setLocalOptions(const OptionsBase & localOptions) = 0;
        virtual void initialize() = 0;

        virtual void compute() = 0;
        virtual void write() = 0;
    };
}

#endif
