/**
 * @file: OptionsBase.h
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

#ifndef CONTROL_OPTIONSBASE_H_
#define CONTROL_OPTIONSBASE_H_

#include <string>
#include <boost/program_options.hpp>

namespace Control {

    /**
     * @brief This class handles argument passing by command line using boost c++ library 
     * tutorial: https://www.boost.org/doc/libs/1_62_0/doc/html/program_options/tutorial.html
     * 
     */
    class OptionsBase
    {
    protected:
        /// typedef keyword in C++ is used for aliasing existing data types, user-defined data types, and pointers to a more meaningful name. 
        /// Typedefs allow you to give descriptive names to standard data types, which can also help you self-document your code.
        /// Mostly typedefs are used for aliasing, only if the predefined name is too long or complex to write again and again. 
        /// The unnecessary use of typedef is generally not a good practice
        /// typedef <current_name> <new_name>
        typedef boost::program_options::options_description BoostOptionsDescription;
        typedef boost::program_options::variables_map BoostVariablesMap;

        OptionsBase(const std::string& description);
        ~OptionsBase();

    public:
        std::string getStringDescription() const;
        void parse(int argc, char ** argv);

        virtual bool isCircuitReadInOption() const = 0;

    protected:
        /// "add" option functions
        void addBoolOption(const std::string & optionName, const std::string & optionDescription);
        void addStringOption(const std::string & optionName, const std::string & optionDescription);
        void addStringOption(const std::string & optionName, const std::string & optionDescription, const std::string & defaultValue);

        /// "check" option functions
        bool hasOptionValue(const std::string & optionName) const;

        /// "retrival" option functions
        std::string findStringOptionValue(const std::string & optionName) const;
        bool findBoolOptionValue(const std::string & optionName) const; // New function here

    private:
        OptionsBase(const OptionsBase & other); // don't implement
        void operator=(const OptionsBase & other); // don't implement

        bool parsedFlag_;
        BoostOptionsDescription optionsDescription_;
        BoostVariablesMap variablesMap_;
    };
}

#endif
