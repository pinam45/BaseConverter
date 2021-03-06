/*****************************************************************************************
 *                                                                                       *
 * MIT License                                                                           *
 *                                                                                       *
 * Copyright (c) 2017 Maxime Pinard                                                      *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy          *
 * of this software and associated documentation files (the "Software"), to deal         *
 * in the Software without restriction, including without limitation the rights          *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell             *
 * copies of the Software, and to permit persons to whom the Software is                 *
 * furnished to do so, subject to the following conditions:                              *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all        *
 * copies or substantial portions of the Software.                                       *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR            *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,              *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE           *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,         *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE         *
 * SOFTWARE.                                                                             *
 *                                                                                       *
 *****************************************************************************************/

#include <ulltostr.hpp>

#include <algorithm>
#include <cstring>

bool ulltostr(unsigned long long int value, unsigned int base, char* str, size_t sz) {
	if(value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return true;
	}

	if(base < 2) {
		str[0] = '\0';
		return false;
	}

	size_t current = 0;
	while(value > 0) {
		unsigned long long x = value % base;
		if(x < 10)
			str[current] = static_cast<char>('0' + x);
		else
			str[current] = static_cast<char>('A' + (x - 10));

		value /= base;
		if(current == sz) {
			str[current] = '\0';
			return false;
		}
		++current;
	}

	str[current] = '\0';
	std::reverse(str, &str[std::strlen(str)]);
	return true;
}
