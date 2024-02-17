#pragma once

#include <vector>
#include <string>
#include <utility>

#include "feature.hpp"

namespace cwt::details::compiler
{

class examples
{
 public:
  examples(feature* f, const cuke::value_array& tags);
  void header();
  void body(std::size_t scenario_idx);

 private:
  std::size_t make_variable();
  void process_table_row();
  void create_call(std::size_t scenario_idx);

 private:
  feature* m_feature;
  std::vector<std::size_t> m_variables;
  cuke::value_array m_tags;
  
  struct row
  {
    token begin;
    token end;
  };
  row m_header;
  row m_current_row;

};

}  // namespace cwt::details::compiler
