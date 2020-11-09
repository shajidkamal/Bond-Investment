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

#ifndef INPUTREADER_INPUTREADER_HPP
#define INPUTREADER_INPUTREADER_HPP

#include <istream>
#include <memory>
#include <string>

#include "InputField.hpp"

// Abstract base class for all input file readers
class InputReader {
private:
    // Will hold the stream being read from
    std::shared_ptr<std::istream> input_stream_;

protected:
    /* Process next character from file.
     * Must be implemented by all deriving classes - this is how they
     * interpret the content of the file (i.e. provide their
     * specialisation).
     *
     * Args:
     *   char - character read from file.
     * Returns:
     *   true if the last character was the end of the current field, false
     *   otherwise.
     */
    virtual bool ProcessChar(const char) = 0;

    /* Protected member to hold the current field that will be returned.
     * Will be returned when ProcessChar returns true - ProcessChar should
     * populate this.
     */
    std::string current_field_;

public:
    /* Constructor.
     *
     * Args: 
     *   std::shared_ptr<std::istream> - shared pointer to the input stream
     *     to read from (so we don't try and duplicate the stream).
     */
    InputReader(std::shared_ptr<std::istream> input_stream)
    : input_stream_{input_stream}, current_field_ {""}
    {}
    virtual ~InputReader() = default;
    /* Method to get next field from the input stream.
     * Returns:
     *   String containing the next field in the stream.
     */
    InputField GetNextField();
};

/*
 * Constructs and returns an appropriate InputReader
 *
 * Args:
 *  filename: path to file to open
 *
 * Returns:
 *  Unique pointer to a constructed InputReader object
 */
std::unique_ptr<InputReader> InputReaderFactory(std::string filename);

#endif //INPUTREADER_INPUTREADER_HPP
