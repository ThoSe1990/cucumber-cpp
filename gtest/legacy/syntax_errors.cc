#include <gtest/gtest.h>

extern "C" {
    #include "cwt/cucumber.h"   
}

class syntax_errors : public ::testing::Test 
{
protected:
  void SetUp() override
  {
    open_cucumber();
    step_function("a test step with a {string}", test_step);
    step_function("any {int} value", test_step);
  }

  void TearDown() override 
  {
    close_cucumber();
  }
  
  static void test_step(int arg_count, cuke_value* args)
  {

  } 
};

TEST_F(syntax_errors, unknown_step)
{

const char* script = R"*(
  Feature: some feature ... 
    Scenario: some scenario ...
      Given a test step with a "string is valid"
      But this is an unknown step
)*";

  EXPECT_EQ(CUKE_FAILED, run_cuke(script, ""));
}


TEST_F(syntax_errors, invalid_table_1)
{
const char* script = R"*(
  Feature: some feature ... 

  Scenario Outline: My cubes  
    Given any <integer> value
    
    Examples:
      | integer   |
      | 1 | 2 | 3 |
)*";

  EXPECT_EQ(CUKE_COMPILE_ERROR, run_cuke(script, ""));
}


TEST_F(syntax_errors, invalid_table_2)
{
const char* script = R"*(
  Feature: some feature ... 

  Scenario Outline: My cubes  
    Given any <integer> value
    
    Examples:
      | integer   |
      1 | 2 | 3 |
)*";

  EXPECT_EQ(CUKE_COMPILE_ERROR, run_cuke(script, ""));
}



TEST_F(syntax_errors, invalid_tag_1)
{
const char* script = R"*(
  Feature: some feature ... 

  @this_is_not_allowed
  Scenario Outline: My cubes  
    Given any <integer> value
    
    Examples:
      | integer   |
      | 1 | 
)*";

  EXPECT_EQ(CUKE_COMPILE_ERROR, run_cuke(script, ""));
}

TEST_F(syntax_errors, invalid_tag_2)
{
const char* script = R"*(
  Feature: some feature ... 
  
  a tag here is ok because before the first
  scenario its the feature description 
  Scenario: My cubes  
    Given any 99 value
    
  no_at
  Scenario: My cubes  
    Given any 99 value
    
)*";

  EXPECT_EQ(CUKE_COMPILE_ERROR, run_cuke(script, ""));
}


// TODO probably in vm when NIL values are overwritten 
TEST_F(syntax_errors, wrong_types)
{
const char* script = R"*(
  Feature: some feature ... 

  Scenario Outline: My cubes  
    Given any <integer> value
    
    Examples:
      | integer   |
      | "some string value"    |
      | 123   |
)*";

  EXPECT_EQ(CUKE_FAILED, run_cuke(script, ""));
}

TEST_F(syntax_errors, examples_after_scenario)
{
const char* script = R"*(
  Feature: some feature ... 

  Scenario: My cubes  
    Given any <integer> value
    
    Examples:
      | integer   |
      | 1 |
      | 2 |
)*";

  EXPECT_EQ(CUKE_COMPILE_ERROR, run_cuke(script, ""));
}


TEST_F(syntax_errors, undefined_variable)
{
const char* script = R"*(
  Feature: some feature ... 

  Scenario Outline: My cubes  
    Given any <value> value
    
    Examples:
      | another   |
      | 1 |
      | 2 |
)*";

  EXPECT_EQ(CUKE_FAILED, run_cuke(script, ""));
}