/*
 * Copyright 2020 Laurence Alexander Hurst
 *
 * This file is part of C++ for Finance.
 *
 * C++ for Finance is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * C++ for Finance is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with C++ for Finance.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef INPUTREADER_TSVINPUTREADER_HPP
#define INPUTREADER_TSVINPUTREADER_HPP

#include <istream>
#include <memory>

#include "InputReader.hpp"

// Tab-seperated-values input reader
class TSVInputReader : public InputReader {
protected:
    virtual bool ProcessChar(const char in_char) override;
public:
    // Delegating constructor for InputReader base class
    TSVInputReader(std::shared_ptr<std::istream> input_stream)
    : InputReader(input_stream)
    {}
};

#endif //INPUTREADER_TSVINPUTREADER_HPP
