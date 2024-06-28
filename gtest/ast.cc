#include <gtest/gtest.h>

#include "../src/ast/parser.hpp"

TEST(ast, init_obj) { cuke::parser p; }

TEST(ast, feature_w_parser)
{
  const char* script = "Feature: First Feature";
  cuke::parser p;
  p.parse_script(script);
  EXPECT_EQ(p.head().feature().keyword(), std::string("Feature:"));
  EXPECT_EQ(p.head().feature().name(), std::string("First Feature"));
}
TEST(ast, feature_error)
{
  const char* script = "this is no feature";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me
  cuke::ast::feature_node feature = cwt::details::parse_feature(lex);
  EXPECT_TRUE(lex.error());
  EXPECT_TRUE(feature.name().empty());
  EXPECT_TRUE(feature.keyword().empty());
}
TEST(ast, feature)
{
  const char* script = "Feature: A Feature";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me
  cuke::ast::feature_node feature = cwt::details::parse_feature(lex);
  EXPECT_EQ(feature.keyword(), std::string("Feature:"));
  EXPECT_EQ(feature.name(), std::string("A Feature"));
  EXPECT_EQ(feature.line(), 1);
}
TEST(ast, feature_w_description)
{
  const char* script = R"*(
  Feature: A Feature
  with some 
  description below
  )*";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me
  cuke::ast::feature_node feature = cwt::details::parse_feature(lex);
  EXPECT_EQ(feature.description().size(), 2);
  EXPECT_EQ(feature.description().at(0), std::string("with some"));
  EXPECT_EQ(feature.description().at(1), std::string("description below"));
  EXPECT_EQ(feature.line(), 2);
}

TEST(ast, feature_w_tags)
{
  const char* script = R"*(
  @tag1 @tag2
  Feature: A Feature
  )*";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me
  cuke::ast::feature_node feature = cwt::details::parse_feature(lex);
  EXPECT_EQ(feature.tags().size(), 2);
  EXPECT_EQ(feature.tags().at(0), std::string("@tag1"));
  EXPECT_EQ(feature.tags().at(1), std::string("@tag2"));
  EXPECT_EQ(feature.line(), 3);
}

TEST(ast, feature_w_scenario)
{
  const char* script = R"*(
  Feature: A feature
  Scenario: A Scenario
  )*";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me
  cuke::ast::feature_node feature = cwt::details::parse_feature(lex);
  ASSERT_EQ(feature.scenarios().size(), 1);
  EXPECT_EQ(feature.scenarios().at(0)->keyword(), std::string("Scenario:"));
  EXPECT_EQ(feature.scenarios().at(0)->name(), std::string("A Scenario"));
}

TEST(ast, scenario_w_steps)
{
  const char* script = R"*(
  Scenario: A Scenario
  Given A step with value 5 
  )*";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me

  auto scenarios = cwt::details::parse_scenarios(lex);
  ASSERT_EQ(scenarios.size(), 1);
}

TEST(ast, parse_step)
{
  const char* script = "Given A step with value 5";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me

  auto steps = cwt::details::parse_steps(lex);
  ASSERT_EQ(steps.size(), 1);
  EXPECT_EQ(steps.at(0).keyword(), std::string("Given"));
  EXPECT_EQ(steps.at(0).name(), std::string("A step with value 5"));
  EXPECT_TRUE(steps.at(0).doc_string().empty());
}
TEST(ast, parse_step_w_doc_string)
{
  const char* script = R"*(
    Given A step with value 5
    """
    this is 
    a multiline 
    doc string
    """ 
  )*";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me

  auto steps = cwt::details::parse_steps(lex);
  ASSERT_FALSE(lex.error());
  ASSERT_EQ(steps.at(0).doc_string().size(), 3);
  EXPECT_EQ(steps.at(0).doc_string().at(0), std::string("this is"));
  EXPECT_EQ(steps.at(0).doc_string().at(1), std::string("a multiline"));
  EXPECT_EQ(steps.at(0).doc_string().at(2), std::string("doc string"));
}
TEST(ast, parse_step_w_doc_string_w_backticks)
{
  const char* script = R"*(
    Given A step with value 5
    ``` 
    this is 
    a multiline 
    doc string
    ``` 
  )*";
  cwt::details::lexer lex(script);
  lex.advance();  // TODO delete me

  auto steps = cwt::details::parse_steps(lex);
  ASSERT_FALSE(lex.error());
  ASSERT_EQ(steps.at(0).doc_string().size(), 3);
  EXPECT_EQ(steps.at(0).doc_string().at(0), std::string("this is"));
  EXPECT_EQ(steps.at(0).doc_string().at(1), std::string("a multiline"));
  EXPECT_EQ(steps.at(0).doc_string().at(2), std::string("doc string"));
}