/************************************************************************
* file name         : profiler_aux.h
* ----------------- :
* creation time     : 2016/06/11
* author            : Victor Zarubkin
* email             : v.s.zarubkin@gmail.com
* ----------------- :
* description       : This file contains auxiliary profiler macros and funcitons.
* ----------------- :
* change log        : * 2016/06/11 Victor Zarubkin: Moved sources from profiler.h
*                   :
*                   : *
* ----------------- :
* license           : Lightweight profiler library for c++
*                   : Copyright(C) 2016  Sergey Yagovtsev, Victor Zarubkin
*                   :
*                   :
*                   : Licensed under the Apache License, Version 2.0 (the "License");
*                   : you may not use this file except in compliance with the License.
*                   : You may obtain a copy of the License at
*                   :
*                   : http://www.apache.org/licenses/LICENSE-2.0
*                   :
*                   : Unless required by applicable law or agreed to in writing, software
*                   : distributed under the License is distributed on an "AS IS" BASIS,
*                   : WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*                   : See the License for the specific language governing permissions and
*                   : limitations under the License.
*                   :
*                   :
*                   : GNU General Public License Usage
*                   : Alternatively, this file may be used under the terms of the GNU
*                   : General Public License as published by the Free Software Foundation,
*                   : either version 3 of the License, or (at your option) any later version.
*                   :
*                   : This program is distributed in the hope that it will be useful,
*                   : but WITHOUT ANY WARRANTY; without even the implied warranty of
*                   : MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*                   : GNU General Public License for more details.
*                   :
*                   : You should have received a copy of the GNU General Public License
*                   : along with this program.If not, see <http://www.gnu.org/licenses/>.
************************************************************************/
#ifndef EASY_PROFILER_AUX_H
#define EASY_PROFILER_AUX_H

#include <stdint.h>

#include <easy/easy_compiler_support.h>
#include <easy/profiler_colors.h>

//////////////////////////////////////////////////////////////////////////

namespace profiler {

    enum EasyBlockStatus : uint8_t {
        OFF = 0, ///< The block is OFF
        ON = 1, ///< The block is ON (but if it's parent block is off recursively then this block will be off too)
        FORCE_ON = ON | 2, ///< The block is ALWAYS ON (even if it's parent has turned off all children)
        OFF_RECURSIVE = 4, ///< The block is OFF and all of it's children by call-stack are also OFF.
        ON_WITHOUT_CHILDREN = ON | OFF_RECURSIVE, ///< The block is ON but all of it's children are OFF.
        FORCE_ON_WITHOUT_CHILDREN = FORCE_ON | OFF_RECURSIVE, ///< The block is ALWAYS ON but all of it's children are OFF.
    };

    struct passthrough_hash EASY_FINAL {
        template <class T> inline size_t operator () (T _value) const {
            return static_cast<size_t>(_value);
        }
    };

}

//////////////////////////////////////////////////////////////////////////

#include <type_traits>
#include <string>

# define EASY_STRINGIFY(a) #a
# define EASY_STRINGIFICATION(a) EASY_STRINGIFY(a)
# define EASY_TOKEN_JOIN(x, y) x ## y
# define EASY_TOKEN_CONCATENATE(x, y) EASY_TOKEN_JOIN(x, y)
# define EASY_UNIQUE_BLOCK(x) EASY_TOKEN_CONCATENATE(unique_profiler_mark_name_, x)
# define EASY_UNIQUE_DESC(x) EASY_TOKEN_CONCATENATE(unique_profiler_descriptor_, x)

#ifdef BUILD_WITH_EASY_PROFILER

namespace profiler {

    template <const bool> struct NameSwitch;

    class ForceConstStr EASY_FINAL {
        friend NameSwitch<true>;
        friend NameSwitch<false>;

        const char* c_str;

        ForceConstStr() = delete;
        ForceConstStr(const ForceConstStr&) = delete;
        ForceConstStr(ForceConstStr&&) = delete;
        ForceConstStr& operator = (const ForceConstStr&) = delete;
        ForceConstStr& operator = (ForceConstStr&&) = delete;

    public:

        ForceConstStr(const char* _str) : c_str(_str) {}
        ForceConstStr(const ::std::string& _str) : c_str(_str.c_str()) {}
    };

    template <const bool IS_REF> struct NameSwitch EASY_FINAL {
        static const char* runtime_name(const char* name) { return name; }
        static const char* runtime_name(const ::std::string& name) { return name.c_str(); }
        static const char* runtime_name(const ForceConstStr&) { return ""; }

        template <class T>
        static const char* compiletime_name(const T&, const char* autoGeneratedName) { return autoGeneratedName; }
        static const char* compiletime_name(const char*, const char* autoGeneratedName) { return autoGeneratedName; }
        static const char* compiletime_name(const ForceConstStr& name, const char*) { return name.c_str; }
    };

    template <> struct NameSwitch<true> EASY_FINAL {
        static const char* runtime_name(const char*) { return ""; }
        static const char* runtime_name(const ::std::string& name) { return name.c_str(); }
        static const char* runtime_name(const ForceConstStr&) { return ""; }

        template <class T>
        static const char* compiletime_name(const T&, const char* autoGeneratedName) { return autoGeneratedName; }
        static const char* compiletime_name(const char* name, const char*) { return name; }
        static const char* compiletime_name(const ForceConstStr& name, const char*) { return name.c_str; }
    };

    //***********************************************

    inline color_t extract_color() {
        return ::profiler::colors::Default;
    }

    template <class ... TArgs>
    inline color_t extract_color(::profiler::EasyBlockStatus, TArgs...) {
        return ::profiler::colors::Default;
    }

    template <class ... TArgs>
    inline color_t extract_color(color_t _color, TArgs...) {
        return _color;
    }

    template <class T, class ... TArgs>
    inline color_t extract_color(T, color_t _color, TArgs...) {
        return _color;
    }

    template <class ... TArgs>
    inline color_t extract_color(TArgs...) {
        static_assert(sizeof...(TArgs) < 2, "No profiler::color_t in arguments list for EASY_BLOCK(name, ...)!");
        return ::profiler::colors::Default;
    }

    //***********************************************

    inline EasyBlockStatus extract_enable_flag() {
        return ::profiler::ON;
    }

    template <class T, class ... TArgs>
    inline EasyBlockStatus extract_enable_flag(T, ::profiler::EasyBlockStatus _flag, TArgs...) {
        return _flag;
    }

    template <class ... TArgs>
    inline EasyBlockStatus extract_enable_flag(::profiler::EasyBlockStatus _flag, TArgs...) {
        return _flag;
    }

    template <class ... TArgs>
    inline EasyBlockStatus extract_enable_flag(TArgs...) {
        static_assert(sizeof...(TArgs) < 2, "No EasyBlockStatus in arguments list for EASY_BLOCK(name, ...)!");
        return ::profiler::ON;
    }

    //***********************************************

} // END of namespace profiler.

# define EASY_UNIQUE_LINE_ID __FILE__ ":" EASY_STRINGIFICATION(__LINE__)
# define EASY_COMPILETIME_NAME(name) ::profiler::NameSwitch<::std::is_reference<decltype(name)>::value>::compiletime_name(name, EASY_UNIQUE_LINE_ID)
# define EASY_RUNTIME_NAME(name) ::profiler::NameSwitch<::std::is_reference<decltype(name)>::value>::runtime_name(name)
# define EASY_CONST_NAME(name) ::profiler::ForceConstStr(name)

#else

# define EASY_CONST_NAME(name) 

#endif // BUILD_WITH_EASY_PROFILER

//////////////////////////////////////////////////////////////////////////

#endif // EASY_PROFILER_AUX_H
