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

#ifndef CONSOLECONTROL_WARN_HPP
#define CONSOLECONTROL_WARN_HPP


#if defined(_MSC_VER)
#define EXT_HEADERS_OPEN \
__pragma(warning(push))\
__pragma(warning(disable:4365))
#define EXT_HEADERS_CLOSE \
__pragma(warning(pop))
#elif defined(__GNUC__)
#define EXT_HEADERS_OPEN \
_Pragma("GCC diagnostic push")\
_Pragma("GCC diagnostic ignored \"-Wzero-as-null-pointer-constant\"")\
_Pragma("GCC diagnostic ignored \"-Wconversion\"")\
_Pragma("GCC diagnostic ignored \"-Wsuggest-override\"")
#define EXT_HEADERS_CLOSE \
_Pragma("GCC diagnostic pop")
#else
#define EXT_HEADERS_OPEN
#define EXT_HEADERS_CLOSE
#endif


#endif //CONSOLECONTROL_WARN_HPP
