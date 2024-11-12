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

#ifndef HSPICE_INPUTFILE_INSTANCELINE_H_
#define HSPICE_INPUTFILE_INSTANCELINE_H_

#include "HSpice/incl/InputFile/InstanceLine/Instance.h"
#include "HSpice/incl/InputFile/InstanceLine/InstanceTerminalOrdering.h"
#include "Core/incl/Circuit/Net/NetId/NetId.h"
#include <string>

namespace HSpice {
    namespace InputFile {

        class InstanceLine
        {
        private:
            typedef std::vector<Core::NetId> NetIdVector;

        public:
            InstanceLine();

            static bool isInstance(char identifier);
            void setLine(const std::string& line);
            void parse();

            Instance getInstance() const;
            std::vector<Core::NetId> getNetIds() const;

        private:
            static const char INSTANCE_ID_LOWER_;
            static const char INSTANCE_ID_UPPER_;

            void parseInstanceName(std::istringstream& iss);
            void parseInstanceTerminalsAndName(std::istringstream& iss);
            void parseInstanceTerminalsAndNameCdl(std::istringstream& iss);

            std::string getLine() const;

        private:
            std::string line_;
            Instance instance_;
            NetIdVector netIds_;
        };

    }
}

#endif