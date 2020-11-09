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

#include "CSVInputReader.hpp"

bool CSVInputReader::ProcessChar(const char in_char) {
    /*
     * We have quite a few different cases to check, so switching rather than
     * a huge block of 'if's
     */
    switch(in_char) {
        // Comma and newlines might end fields (depends on context)
        case ',':
        case '\n':
            /*
             * If the instance was inside a double quote but this follows a
             * single double quote, then that quote ended the double quoted
             * portion.
             */
            if (inside_double_quote_ && last_char_double_quote_) {
                inside_double_quote_ = false;
                last_char_double_quote_ = false;
            }

            /*
             * If this comma or newline is inside a double quote, treat it as
             * a literal ',' or '\n'
             */
            if (inside_double_quote_) {
                current_field_ += in_char;
                break; // Done - processed character
            } else {
                /*
                 * If not inside a double quote, it ends (but is not part of)
                 * the current field.
                 */
                return true;
            }
        /*
         * Double quotes might start or end a quoted string or a pair might
         * be a literal double quote if insude a quoted string.
         */
        case '"':
            // Has this quote been found inside a quoted string?
            if(inside_double_quote_) {
                /*
                 * ...if so, was the previous character also a quote (forming
                 * a pair)?
                 */
                if (last_char_double_quote_) {
                    // A pair is a literal double-quote, so store one...
                    current_field_ += in_char;
                    /*
                     * ...and clear the flag indicating the previous
                     * character was a double quote (having now processed
                     * both of the pair of double quote characters)
                     */
                    last_char_double_quote_ = false;
                    break; // Done - processed character
                } else {
                    /*
                     * ...double quote inside a quoted string but last
                     * character was not a double quote.  Set the flags
                     * indicating that the last character was a double quote
                     * in case this is the first of a pair.
                     */
                    last_char_double_quote_ = true;
                }
            } else {
                /*
                 *...was not inside a double quote and found one.  Now this
                 * instance is.
                 */
                inside_double_quote_ = true;
            }
            /*
             * What ever happened in this case, we have processed the double
             * quote character, so done.
             */
            break; 
        // Any other character has no special meaning
        default:
            current_field_ += in_char; // store it
            /*
             * If the flag showing the previous character was a double quote
             * is set then (since this case only runs if the next character
             * was not one) it was a single double quote, which ended the
             * quoted string (pairs are only valid within quoted strings so
             * the flag only gets set if the instance was within on.
             */
            if (last_char_double_quote_) {
                // No longer in quoted string
                inside_double_quote_ = false;
                /*
                  * Have now processed the previous charcter (double quote),
                  * this character was not one and has already been stored.
                  * Reset flag saying the last character was a double quote.
                  */
                last_char_double_quote_ = false;
            }
    }

    /*
     * The only case where a field ends returns true, above.  All other cases
     * are not the end of a field.
     */
    return false;
}
