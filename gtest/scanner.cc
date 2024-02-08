#include <gtest/gtest.h>

#include "../src/cwt/scanner.hpp"

using namespace cwt::details;

TEST(scanner, init_obj) { scanner s("some script"); }

TEST(token, token_1)
{
  token t = scanner("<some variable>").scan_token();
  EXPECT_EQ(t.type, token_type::variable);
  EXPECT_EQ(t.line, 1);
  EXPECT_EQ(t.value.size(), std::string("<some variable>").size());
  EXPECT_STREQ(t.value.data(), "<some variable>");
}
TEST(token, token_2)
{
  token t = scanner("\"\"\"\nhello doc string\n   \"\"\"").scan_token();
  EXPECT_EQ(t.type, token_type::doc_string);
  EXPECT_EQ(t.line, 2);
  EXPECT_EQ(t.value.size(), std::string_view("\"\"\"\nhello doc string\n   \"\"\"").size());
  EXPECT_EQ(t.value, std::string_view("\"\"\"\nhello doc string\n   \"\"\""));
}

TEST(scanner, scan_vertical) { token t = scanner("|").scan_token(); }
TEST(scanner, scan_minus)
{
  EXPECT_EQ(scanner("-").scan_token().type, token_type::minus);
}
TEST(scanner, asterisk)
{
  EXPECT_EQ(scanner("*").scan_token().type, token_type::step);
}
TEST(scanner, left_paren)
{
  EXPECT_EQ(scanner("(").scan_token().type, token_type::left_paren);
}
TEST(scanner, right_paren)
{
  EXPECT_EQ(scanner(")").scan_token().type, token_type::right_paren);
}
TEST(scanner, and_token)
{
  EXPECT_EQ(scanner("and").scan_token().type, token_type::_and);
}
TEST(scanner, or_token)
{
  EXPECT_EQ(scanner("or").scan_token().type, token_type::_or);
}
TEST(scanner, not_token)
{
  EXPECT_EQ(scanner("not").scan_token().type, token_type::_not);
}
TEST(scanner, xor_token)
{
  EXPECT_EQ(scanner("xor").scan_token().type, token_type::_xor);
}

TEST(scanner, tag_paren_token)
{
  scanner s("@tag1()");
  EXPECT_EQ(s.scan_token().type, token_type::tag);
  EXPECT_EQ(s.scan_token().type, token_type::left_paren);
  EXPECT_EQ(s.scan_token().type, token_type::right_paren);
}
TEST(scanner, word_paren_token)
{
  scanner s("hello world and ()");
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::_and);
  EXPECT_EQ(s.scan_token().type, token_type::left_paren);
  EXPECT_EQ(s.scan_token().type, token_type::right_paren);
}

TEST(scanner, tag_1)
{
  EXPECT_EQ(scanner("@my_tag").scan_token().type, token_type::tag);
}
TEST(scanner, tag_2)
{
  token t = scanner("@tag1with2numbers").scan_token();
  EXPECT_EQ(t.type, token_type::tag);
  EXPECT_EQ(t.value, "@tag1with2numbers");
}
TEST(scanner, variable)
{
  EXPECT_EQ(scanner("<some variable>").scan_token().type, token_type::variable);
}
TEST(scanner, parameter_int)
{
  EXPECT_EQ(scanner("{int}").scan_token().type, token_type::parameter_int);
}
TEST(scanner, parameter_float)
{
  EXPECT_EQ(scanner("{float}").scan_token().type, token_type::parameter_float);
}
TEST(scanner, parameter_word)
{
  EXPECT_EQ(scanner("{word}").scan_token().type, token_type::parameter_word);
}
TEST(scanner, parameter_string)
{
  EXPECT_EQ(scanner("{string}").scan_token().type, token_type::parameter_string);
}
TEST(scanner, parameter_double)
{
  EXPECT_EQ(scanner("{double}").scan_token().type, token_type::parameter_double);
}
TEST(scanner, parameter_byte)
{
  EXPECT_EQ(scanner("{byte}").scan_token().type, token_type::parameter_byte);
}
TEST(scanner, parameter_short)
{
  EXPECT_EQ(scanner("{short}").scan_token().type, token_type::parameter_short);
}
TEST(scanner, parameter_long)
{
  EXPECT_EQ(scanner("{long}").scan_token().type, token_type::parameter_long);
}
TEST(scanner, variable_missing_closing)
{
  EXPECT_EQ(scanner("<some variable").scan_token().type, token_type::error);
}
TEST(scanner, string)
{
  EXPECT_EQ(scanner("\"some string\"").scan_token().type,
            token_type::string_value);
}
TEST(scanner, string_missing_closing)
{
  EXPECT_EQ(scanner("\"some string").scan_token().type, token_type::error);
}
TEST(scanner, doc_string)
{
  EXPECT_EQ(scanner("\"\"\"\nhello doc string\n\"\"\"").scan_token().type,
            token_type::doc_string);
}
TEST(scanner, unterminated_doc_string_1)
{
  EXPECT_EQ(scanner("\"\"\"\nhello doc string\n").scan_token().type,
            token_type::error);
}
TEST(scanner, unterminated_doc_string_2)
{
  EXPECT_EQ(scanner("\"\"\"\nhello doc string\n\"\"").scan_token().type,
            token_type::error);
}
TEST(scanner, unterminated_doc_string_3)
{
  EXPECT_EQ(scanner("\"\"\"\nhello doc string\n\"").scan_token().type,
            token_type::error);
}
TEST(scanner, long_value)
{
  EXPECT_EQ(scanner("99").scan_token().type, token_type::long_value);
}
TEST(scanner, double_value_1)
{
  EXPECT_EQ(scanner("99.").scan_token().type, token_type::double_value);
}
TEST(scanner, double_value_2)
{
  EXPECT_EQ(scanner("99.9").scan_token().type, token_type::double_value);
}
TEST(scanner, double_value_3)
{
  EXPECT_EQ(scanner("99.99").scan_token().type, token_type::double_value);
}
TEST(scanner, arbitrary_text_1)
{
  EXPECT_EQ(scanner("foo").scan_token().type, token_type::word);
}
TEST(scanner, arbitrary_text_2)
{
  scanner s("some text");
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::word);
}
TEST(scanner, statement_1)
{
  scanner s("Step with <var> and 99");

  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::variable);
  EXPECT_EQ(s.scan_token().type, token_type::_and);
  EXPECT_EQ(s.scan_token().type, token_type::long_value);
}

TEST(scanner, statement_2)
{
  scanner s("Step with \"some string value\" and 99.99");

  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::string_value);
  EXPECT_EQ(s.scan_token().type, token_type::_and);
  EXPECT_EQ(s.scan_token().type, token_type::double_value);
}
TEST(scanner, table_header)
{
  scanner s("| some | variables | here |");

  EXPECT_EQ(s.scan_token().type, token_type::vertical);
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
  EXPECT_EQ(s.scan_token().type, token_type::word);
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
}
TEST(scanner, verticals_and_text)
{
  scanner s("| 99 | \"some string\" | 123.123 |");
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
  EXPECT_EQ(s.scan_token().type, token_type::long_value);
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
  EXPECT_EQ(s.scan_token().type, token_type::string_value);
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
  EXPECT_EQ(s.scan_token().type, token_type::double_value);
  EXPECT_EQ(s.scan_token().type, token_type::vertical);
}