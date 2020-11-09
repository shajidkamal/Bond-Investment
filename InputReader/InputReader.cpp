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

#include <exception>
#include <fstream>
#include <istream>
#include <memory>
#include <string>

#include "InputReader.hpp"
#include "CSVInputReader.hpp"
#include "TSVInputReader.hpp"

InputField InputReader::GetNextField() {
    // Clear the field read for the next input
    current_field_ = "";

    bool finished {false}; // Have we reached the end of the string?

    while(!finished) {
        // Test if stream is safe to use
        if (!input_stream_->good())
            throw std::runtime_error {"Problem with input stream in GetNextField"};

        // Get next character from file
        char in_char{'\0'};
        input_stream_->get(in_char);

        // If hit end of file, won't have read any char
        if (input_stream_->eof())
            finished = true;
        else
            finished = ProcessChar(in_char);
    }

    // Return the field
    return InputField {current_field_};
}

std::unique_ptr<InputReader> InputReaderFactory(std::string filename) {
    const std::string extension {filename.substr(filename.size()-4, 4)};

    // Create an input file stream to the input file
    std::shared_ptr<std::ifstream> in_file {std::make_shared<std::ifstream>(filename)};

    // Check it opened okay - fail early if there's a problem.
    if (!in_file->good()){
        throw std::runtime_error {"Problem opening " + filename + " in InputReaderFactory"};
    }

    if (extension == ".csv") {
        // Construct a CSV reader
        return std::make_unique<CSVInputReader>(in_file);
    } else if (extension == ".tsv") {
        // Construct a TSV reader
        return std::make_unique<TSVInputReader>(in_file);
    } else {
        throw std::invalid_argument {"Unknown file extension in InputReaderFactory: " + extension};
    }
}

