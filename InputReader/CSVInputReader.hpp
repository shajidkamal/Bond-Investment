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

#ifndef INPUTREADER_CSVINPUTREADER_HPP
#define INPUTREADER_CSVINPUTREADER_HPP

#include "InputReader.hpp"

// comma-seperated-values input stream reader
class CSVInputReader : public InputReader {
private:
    /*
     * Flag to hold if the reader instance is currently parsing characters
     * within a quoted string.
     */
    bool inside_double_quote_;
    /*
     * Flag to hold if the previous character was a double quote within a
     * quote string (as it may be either the end of the quoted string or the
     * start of a pair indicating a literal double quote.
     */
    bool last_char_double_quote_;
protected:
    virtual bool ProcessChar(const char in_char) override;
public:
    // Constructor - delegates handling the stream to InputReader base class
    CSVInputReader(std::shared_ptr<std::istream> input_stream)
    : InputReader(input_stream), inside_double_quote_{false},
      last_char_double_quote_{false}
    {}
};

#endif //INPUTREADER_CSVINPUTREADER_HPP
