#pragma once

#include <vector>
#include <string>
#include <stdexcept>
#include <string_view>

#include "../parser.hpp"

namespace cwt::details::compiler
{

struct tag_token
{
  token_type token;
  std::string value;
};

class tag_expression
{
 public:
  tag_expression(std::string_view expression);
  const tag_token& operator[](std::size_t idx) const;

  std::size_t size() const noexcept;

 private:
  [[nodiscard]] tag_token make_token() const noexcept;
  void compile();
  void expression();
  void tag();
  void and_or_xor();
  void left_association();
  void grouping();
  void close_grouping();
  void push_remaining_operators();

 private:
  std::vector<tag_token> m_out;
  std::vector<tag_token> m_operators;
  parser m_parser;
};

}  // namespace cwt::details::compiler
