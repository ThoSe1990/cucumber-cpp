#include <gtest/gtest.h>

#include "../src/ast/parser.hpp"

TEST(ast, init_obj)
{
  const char* script = R"*(
  Feature: First Feature
  Scenario: First Scenario
  Given A Step with 123
)*";
  cwt::details::parser p(script);
}

TEST(ast_tests, feature)
{
  const char* script = R"*(
  Feature:
)*";
  cwt::details::parser p(script);
  p.parse();
  EXPECT_EQ(p.head().infos.type, cuke::ast::node_type::gherkin_document);
  EXPECT_EQ(p.head().children[0].infos.type, cuke::ast::node_type::feature);
}

TEST(ast_tests, scenario)
{
  const char* script = R"*(
  Feature:
  Scenario: 
)*";
  cwt::details::parser p(script);
  p.parse();
  EXPECT_EQ(p.head().children.size(), 1);
  EXPECT_EQ(p.head().children[0].infos.type, cuke::ast::node_type::feature); 
}
