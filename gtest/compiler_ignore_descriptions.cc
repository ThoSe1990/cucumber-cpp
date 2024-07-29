#include <gtest/gtest.h>

#include "../src/compiler/cucumber.hpp"

#define PRINT_STACK 1
#include "../src/debug.hpp"

using namespace cuke::internal;

TEST(compiler_ignore_descriptions, feature_description)
{
  const char* script = R"*(
  Feature: Hello World
    Some Scenario Description here ... 
    Some Scenario: Description here ... 
    Some Scenario: Description here ... 
    Some Scenario: Description here ... 
  Scenario: A Scenario
  Given A step
)*";

  compiler::cucumber cuke(script);
  cuke.compile();

  chunk* feature_chunk = cuke.get_chunk().constant(0).as<function>().get();
  disassemble_chunk(cuke.get_chunk(), "script");
  disassemble_chunk(*feature_chunk, "feature");
  EXPECT_TRUE(cuke.no_error());
}
TEST(compiler_ignore_descriptions, scenario_description)
{
  const char* script = R"*(
  Feature: Hello World
    Some Scenario Description here ... 
    Some Scenario: Description here ... 
    Some Scenario: Description here ... 
    Some Scenario: Description here ... 
  Scenario: A Scenario
    Some teext here 
    with keywords too: Given When 
  Given A step
)*";

  compiler::cucumber cuke(script);
  cuke.compile();

  chunk* feature_chunk = cuke.get_chunk().constant(0).as<function>().get();
  disassemble_chunk(cuke.get_chunk(), "script");
  disassemble_chunk(*feature_chunk, "feature");
  EXPECT_TRUE(cuke.no_error());
}
TEST(compiler_ignore_descriptions, scenario_outline_description)
{
  const char* script = R"*(
  Feature: Hello World
    Some Scenario Description here ... 
    Some Scenario Description here ... 
    Some Scenario: Description here ... 
    Some Scenario Description here ... 
  Scenario Outline: A Scenario
    Some teext here 
    with keywords too: Given When and some other stuff...
  Given A step with a <value> here 
)*";

  testing::internal::CaptureStdout();

  compiler::cucumber cuke(script);
  cuke.compile();

  chunk* feature_chunk = cuke.get_chunk().constant(0).as<function>().get();

  EXPECT_TRUE(cuke.error());
  EXPECT_EQ(
      std::string("\x1B[31m[line 11] Error at end: Expect StepLine, Tags or "
                  "Examples\x1B[0m\n\x1B[31m[line 11] Error at end: Expect Examples table\x1B[0m\n"),
      testing::internal::GetCapturedStdout());
}
TEST(compiler_ignore_descriptions, examples_description)
{
  const char* script = R"*(
  Feature: Hello World
    Some Scenario Description here ... 
    Some Scenario Description here ... 
    Some Scenario: Description here ... 
    Some Scenario Description here ... 
  Scenario Outline: A Scenario
    Some teext here 
    with keywords too: Given When 
  Given A step with <value>

  Examples: some examples 
    with some description | in here ... || 
    in multiple lines!
  | value | 
  | 123   |
)*";

  compiler::cucumber cuke(script);
  cuke.compile();

  chunk* feature_chunk = cuke.get_chunk().constant(0).as<function>().get();
  disassemble_chunk(cuke.get_chunk(), "script");
  disassemble_chunk(*feature_chunk, "feature");
  EXPECT_TRUE(cuke.no_error());
}