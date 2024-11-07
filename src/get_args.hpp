#pragma once

#include <stdexcept>
#include <string_view>
// #include <unordered_map>

// #include "any.hpp"
#include "param_info.hpp"
#include "util.hpp"
#include "value.hpp"
#include "registry.hpp"

namespace cuke::internal
{

struct conversion
{
  // TODO: check members.
  cuke::value_array::const_iterator begin;
  std::size_t idx;
  std::string_view file;
  std::size_t line;
  std::string_view key;
  // TODO: add values count

  // TODO: POC: hardcoded value 2 here -> add value count
  // operator std::pair<int, int>() const { return conversion_map[key](begin,
  // 2); }
  operator std::pair<int, int>() const
  {
    return cuke::registry().get_expression(key).callback(begin, 2);
  }
  template <typename T>
  operator T() const
  {
    return cuke::registry().get_expression(key).callback(begin, idx + 1);
  }
};

inline conversion get_arg(cuke::value_array::const_iterator begin,
                          std::size_t values_count,
                          const std::vector<param_info>& parameter,
                          std::size_t idx, std::string_view file,
                          std::size_t line)
{
  std::size_t zero_based_idx = idx - 1;
  if (zero_based_idx < values_count)
  {
    return conversion{begin, zero_based_idx + parameter[zero_based_idx].offset,
                      file, line, parameter[zero_based_idx].key};
  }
  else
  {
    throw std::runtime_error(
        std::format("{}:{}: Index out of range", file, line));
  }
}

class string_or_vector
{
 public:
  string_or_vector(const std::vector<std::string>& data) : m_data(data) {}

  operator std::string() const
  {
    std::ostringstream oss;
    for (const auto& str : m_data)
    {
      oss << str << ' ';
    }
    std::string result = oss.str();
    if (!result.empty())
    {
      result.pop_back();
    }
    return result;
  }

  operator std::vector<std::string>() const { return m_data; }

 private:
  const std::vector<std::string> m_data;
};

}  // namespace cuke::internal

/**
 * @def CUKE_ARG(index)
 * @brief Access variables from a step in the step body. ``CUKE_ARG`` starts at
 * index 1 on the first value from the left side.
 *
 * C++ auto type deduction does not work here. The underlying function is
 * overloaded by return type.
 *
 * @param index Variable index to access
 * @return The value from the index in the given step
 */

#define CUKE_ARG(index)                                                      \
  cuke::internal::get_arg(__cuke__values__.begin(), __cuke__values__.size(), \
                          __cuke__parameter_info__, index, __FILE__, __LINE__)

/**
 * @def CUKE_DOC_STRING()
 * @brief Access a doc string in a step. Use std::string as type here, e.g.:
 *
 * ``std::string doc = CUKE_DOC_STRING();`` or
 * ``std::string_view doc = CUKE_DOC_STRING();``
 *
 * C++ auto type deduction does not work here. It's essentially the same as
 * CUKE_ARG(..), with index == last.
 *
 */
#define CUKE_DOC_STRING() \
  cuke::internal::string_or_vector(__cuke__doc__string__)

/**
 * @def CUKE_TABLE()
 * @brief Access a table in a step. Use cuke::table (or const cuke::table&) as
 * type, e.g. ``const cuke::table& my_table = CUKE_TABLE();``
 *
 *
 * C++ auto type deduction does not work here. It's essentially the same as
 * CUKE_ARG(..), with index == last.
 *
 */
#define CUKE_TABLE() __cuke__table__
