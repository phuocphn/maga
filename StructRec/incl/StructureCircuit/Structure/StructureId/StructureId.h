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

#ifndef STRUCTREC_INCL_STRUCTURECIRCUIT_STRUCTUREID_H_
#define STRUCTREC_INCL_STRUCTURECIRCUIT_STRUCTUREID_H_

#include "StructRec/incl/StructureCircuit/Structure/StructureId/StructureName.h"
#include "Core/incl/Common/Object.h"

namespace StructRec {

	class StructureId : public Core::Object
    {
    public:
        StructureId();
        StructureId(std::string name, int id);
        ~StructureId();


        void setName(const StructureName&  name);
        void setId(const int id);

        bool isInitialized() const;
        StructureName getName() const;
        int getId() const;

        bool operator!=(const StructureId& other) const;
        bool operator==(const StructureId & other) const;
        bool operator<(const StructureId & other) const;

        std::string toStr() const;

    private:
        bool hasName() const;
        bool hasId() const;

        bool hasEqualName(const StructureId & other) const;
        bool hasEqualId(const StructureId & other) const;

    private:
        static const int ID_NOT_INITIALIZED_;

        StructureName name_;
        int id_;

    };



}



#endif /* STRUCTREC_INCL_STRUCTURECIRCUIT_STRUCTUREID_H_ */