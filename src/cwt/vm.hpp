#pragma once

#include <stack>
#include <string_view>

#include "value.hpp"
#include "chunk.hpp"

namespace cwt::details
{

struct call_frame
{
  std::vector<chunk*> chunks;
};


class vm
{
 public:
  void run(std::string_view script);

 private:
 private:
  std::stack<value> m_stack;
};

}  // namespace cwt::details
