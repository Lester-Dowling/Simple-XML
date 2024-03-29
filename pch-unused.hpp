#pragma once
#ifdef _WINDOWS
#include <WinSDKVer.h>
#include <SDKDDKVer.h>
// include <windows.h>
#endif // _WINDOWS

#include <cassert>
#include <cstdlib>
#include <cctype>

#include <algorithm>
#include <functional>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <optional>
#include <iterator>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <numeric>
#include <stdexcept>

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/adapted/struct/define_struct.hpp>
#include <boost/fusion/adapted/struct/define_struct_inline.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/define_struct_inline.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_eps.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/spirit/include/qi_symbols.hpp>
#include <boost/system/system_error.hpp>
#include <boost/tokenizer.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/compare.hpp>
#include "config.h"
