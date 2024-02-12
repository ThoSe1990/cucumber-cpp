#include <gtest/gtest.h>

#include "../src/compiler/scenario_outline.hpp"

#define PRINT_STACK 1
#include "../src/debug.hpp"

using namespace cwt::details;

namespace compiler_scenario_outline_w_tags
{
const char* script = R"*(
  Feature: Hello World
  
  @tag1 @tag2
  Scenario Outline: A Scenario
  Given A Step with <first var> and <second var>
  
  Examples:
  | first var | second var |
  | 1         | 2          |
  | 3         | 4          |
)*";
}  // namespace compiler_scenario_outline_w_tags

TEST(compiler_scenario_outline_w_tags, scenarios_code_1)
{
  compiler::cucumber cuke(compiler_scenario_outline_w_tags::script);
  compiler::feature f(&cuke);
  f.compile();
  disassemble_chunk(f.get_chunk(), "scenario outline");
  EXPECT_EQ(f.get_chunk().size(), 88);
}

TEST(compiler_scenario_outline_w_tags, scenarios_code_2)
{
  compiler::cucumber cuke(compiler_scenario_outline_w_tags::script);
  compiler::feature f(&cuke);
  f.set_options(options{.tags=compiler::tag_expression("@tag1 and @tag2")});
  f.compile();
  EXPECT_EQ(f.get_chunk().size(), 88);
}
TEST(compiler_scenario_outline_w_tags, scenarios_code_3)
{
  compiler::cucumber cuke(compiler_scenario_outline_w_tags::script);
  compiler::feature f(&cuke);
  f.set_options(options{.tags=compiler::tag_expression("@tag1")});
  f.compile();
  EXPECT_EQ(f.get_chunk().size(), 88);
}
TEST(compiler_scenario_outline_w_tags, scenarios_code_4)
{
  compiler::cucumber cuke(compiler_scenario_outline_w_tags::script);
  compiler::feature f(&cuke);
  f.set_options(options{.tags=compiler::tag_expression("@tag3")});
  f.compile();
  disassemble_chunk(f.get_chunk(), "scenario outline");
  EXPECT_EQ(f.get_chunk().size(), 10);
}
