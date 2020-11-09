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

#ifndef INPUTREADER_INPUTFIELD_HPP
#define INPUTREADER_INPUTFIELD_HPP

#include <string>
#include <ostream>

class InputField {
private:
    // Field value
    std::string field_value_;
public:
    InputField(const std::string & value) : field_value_{value} {}
    operator std::string() const {return field_value_;}
    operator int() const {return std::stoi(field_value_);}
    operator double() const {return std::stod(field_value_);}
};

std::ostream& operator<<(std::ostream& lhs, const InputField & rhs);

#endif //INPUTREADER_INPUTFIELD_HPP
