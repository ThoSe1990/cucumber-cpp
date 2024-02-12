#include <gtest/gtest.h>

#include "../src/cwt/compiler/tags.hpp"

using namespace cwt::details;

TEST(tag_expression, tags_1_tag)
{
  compiler::tag_expression tc("@tag");
  ASSERT_EQ(tc.size(), 1);
  EXPECT_EQ(tc[0].value, "@tag");
}
TEST(tag_expression, tags_1_not_tag)
{
  compiler::tag_expression tc("not @tag");
  ASSERT_EQ(tc.size(), 2);
  EXPECT_EQ(tc[0].value, "@tag");
  EXPECT_EQ(tc[1].token, token_type::_not);
}
TEST(tag_expression, tags_2_not_tag)
{
  compiler::tag_expression tc("not (@tag1 or @tag2)");
  ASSERT_EQ(tc.size(), 4);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].token, token_type::_or);
  EXPECT_EQ(tc[3].token, token_type::_not);
}
TEST(tag_expression, tags_xor_tag)
{
  compiler::tag_expression tc("@tag1 xor @tag2");
  ASSERT_EQ(tc.size(), 3);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].token, token_type::_xor);
}
TEST(tag_expression, tags_1_tag_w_paren)
{
  compiler::tag_expression tc("(@tag)");
  ASSERT_EQ(tc.size(), 1);
  EXPECT_EQ(tc[0].value, "@tag");
}
TEST(tag_expression, tags_2_tags_and)
{
  compiler::tag_expression tc("@tag1 and @tag2");
  ASSERT_EQ(tc.size(), 3);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].token, token_type::_and);
}
TEST(tag_expression, tags_3_tags_brackets)
{
  compiler::tag_expression tc("(@tag1 and @tag2) or @tag3");
  ASSERT_EQ(tc.size(), 5);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].token, token_type::_and);
  EXPECT_EQ(tc[3].value, "@tag3");
  EXPECT_EQ(tc[4].token, token_type::_or);
}
TEST(tag_expression, tags_3_tags_brackets__2)
{
  compiler::tag_expression tc("@tag1 and (@tag2 or @tag3)");
  ASSERT_EQ(tc.size(), 5);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].value, "@tag3");
  EXPECT_EQ(tc[3].token, token_type::_or);
  EXPECT_EQ(tc[4].token, token_type::_and);
}

TEST(tag_expression, tags_4_tags_1_brackets)
{
  compiler::tag_expression tc("@tag1 or @tag2 and (@tag3 or @tag4)");
  ASSERT_EQ(tc.size(), 7);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].token, token_type::_or);
  EXPECT_EQ(tc[3].value, "@tag3");
  EXPECT_EQ(tc[4].value, "@tag4");
  EXPECT_EQ(tc[5].token, token_type::_or);
  EXPECT_EQ(tc[6].token, token_type::_and);
}

TEST(tag_expression, tags_3_tags_1not_2_brackets)
{
  compiler::tag_expression tc("not @tag1 or @tag2 and (@tag3 or @tag4)");
  ASSERT_EQ(tc.size(), 8);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].token, token_type::_not);
  EXPECT_EQ(tc[2].value, "@tag2");
  EXPECT_EQ(tc[3].token, token_type::_or);
  EXPECT_EQ(tc[4].value, "@tag3");
  EXPECT_EQ(tc[5].value, "@tag4");
  EXPECT_EQ(tc[6].token, token_type::_or);
  EXPECT_EQ(tc[7].token, token_type::_and);
}

TEST(tag_expression, tags_3_tags_4not_2_brackets)
{
  compiler::tag_expression tc(
      "not @tag1 or not @tag2 and (not @tag3 or not @tag4)");
  ASSERT_EQ(tc.size(), 11);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].token, token_type::_not);
  EXPECT_EQ(tc[2].value, "@tag2");
  EXPECT_EQ(tc[3].token, token_type::_not);
  EXPECT_EQ(tc[4].token, token_type::_or);
  EXPECT_EQ(tc[5].value, "@tag3");
  EXPECT_EQ(tc[6].token, token_type::_not);
  EXPECT_EQ(tc[7].value, "@tag4");
  EXPECT_EQ(tc[8].token, token_type::_not);
  EXPECT_EQ(tc[9].token, token_type::_or);
  EXPECT_EQ(tc[10].token, token_type::_and);
}

TEST(tag_expression, tags_3_tags_2_brackets)
{
  compiler::tag_expression tc("@tag1 or ((@tag2 and @tag3) or @tag4)");
  ASSERT_EQ(tc.size(), 7);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].value, "@tag3");
  EXPECT_EQ(tc[3].token, token_type::_and);
  EXPECT_EQ(tc[4].value, "@tag4");
  EXPECT_EQ(tc[5].token, token_type::_or);
  EXPECT_EQ(tc[6].token, token_type::_or);
}

TEST(tag_expression, tags_3_tags_3_brackets)
{
  compiler::tag_expression tc("(@tag1 or ((@tag2 and @tag3) or @tag4)");
  ASSERT_EQ(tc.size(), 7);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].value, "@tag3");
  EXPECT_EQ(tc[3].token, token_type::_and);
  EXPECT_EQ(tc[4].value, "@tag4");
  EXPECT_EQ(tc[5].token, token_type::_or);
  EXPECT_EQ(tc[6].token, token_type::_or);
}

TEST(tag_expression, tags_3_tags_x_brackets)
{
  compiler::tag_expression tc(
      "@tag1 or (((@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  ASSERT_EQ(tc.size(), 15);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].value, "@tag3");
  EXPECT_EQ(tc[3].token, token_type::_and);
  EXPECT_EQ(tc[4].value, "@tag4");
  EXPECT_EQ(tc[5].value, "@tag5");
  EXPECT_EQ(tc[6].token, token_type::_and);
  EXPECT_EQ(tc[7].token, token_type::_or);
  EXPECT_EQ(tc[8].value, "@tag7");
  EXPECT_EQ(tc[9].token, token_type::_or);
  EXPECT_EQ(tc[10].value, "@tag8");
  EXPECT_EQ(tc[11].value, "@tag9");
  EXPECT_EQ(tc[12].token, token_type::_and);
  EXPECT_EQ(tc[13].token, token_type::_and);
  EXPECT_EQ(tc[14].token, token_type::_or);
}

TEST(tag_expression, tags_3_tags_x_brackets_2)
{
  compiler::tag_expression tc(
      "@tag1 or (((@tag2 and @tag3) or (@tag4 and @tag5) and @tag7) and (@tag8 "
      "and @tag9))");
  ASSERT_EQ(tc.size(), 15);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].value, "@tag3");
  EXPECT_EQ(tc[3].token, token_type::_and);
  EXPECT_EQ(tc[4].value, "@tag4");
  EXPECT_EQ(tc[5].value, "@tag5");
  EXPECT_EQ(tc[6].token, token_type::_and);
  EXPECT_EQ(tc[7].token, token_type::_or);
  EXPECT_EQ(tc[8].value, "@tag7");
  EXPECT_EQ(tc[9].token, token_type::_and);
  EXPECT_EQ(tc[10].value, "@tag8");
  EXPECT_EQ(tc[11].value, "@tag9");
  EXPECT_EQ(tc[12].token, token_type::_and);
  EXPECT_EQ(tc[13].token, token_type::_and);
  EXPECT_EQ(tc[14].token, token_type::_or);
}

TEST(tag_expression, some_more_spaces)
{
  compiler::tag_expression tc("@tag1   and (   @tag2    or@tag3)");
  ASSERT_EQ(tc.size(), 5);
  EXPECT_EQ(tc[0].value, "@tag1");
  EXPECT_EQ(tc[1].value, "@tag2");
  EXPECT_EQ(tc[2].value, "@tag3");
  EXPECT_EQ(tc[3].token, token_type::_or);
  EXPECT_EQ(tc[4].token, token_type::_and);
}

TEST(tag_expression, syntax_error_1)
{
  compiler::tag_expression tc("some bad syntax");
  ASSERT_EQ(tc.size(), 0);
}
TEST(tag_expression, syntax_error_2)
{
  compiler::tag_expression tc("@tag1 and syntax error");
  ASSERT_EQ(tc.size(), 0);
}

TEST(tag_evaluation, one_tag_true)
{
  value_array tags{std::string("@hello_world")};
  compiler::tag_expression tc("@hello_world");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, one_tag_false)
{
  value_array tags{std::string("@hello_world")};
  compiler::tag_expression tc("@something_else");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, two_tags_and_true)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc("@tag1 and @tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, two_tags_and_false)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc("@tag1 and @not_given");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, _4_tags_true_1)
{
  value_array tags{std::string("@tag4")};
  compiler::tag_expression tc("(@tag1 or ((@tag2 and @tag3) or @tag4)");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, _4_tags_true_2)
{
  value_array tags{std::string("@tag2"), std::string("@tag3")};
  compiler::tag_expression tc("(@tag1 or ((@tag2 and @tag3) or @tag4)");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, _4_tags_false)
{
  value_array tags{std::string("@tag5"), std::string("@not_there"),
                   std::string("@anything")};
  compiler::tag_expression tc("(@tag1 or ((@tag2 and @tag3) or @tag4)");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, empty_condition_not_is_true_1)
{
  value_array tags;
  compiler::tag_expression tc("not @tag1");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_true_1)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc(
      "@tag1 or (( (@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_true_2)
{
  value_array tags{std::string("@tag1"), std::string("@tag2"),
                   std::string("@tag7"), std::string("@tag8"),
                   std::string("@tag9")};
  compiler::tag_expression tc(
      "@tag1 or (((@tag2 and @tag3) or (@tag4 and@tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_true_3)
{
  value_array tags{std::string("@tag1"), std::string("@tag2"),
                   std::string("@tag7"), std::string("@tag8"),
                   std::string("@tag9")};
  compiler::tag_expression tc(
      "@tag1 or (((@tag2 and @tag3) or (@tag4 and@tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_true_w_xor_4)
{
  value_array tags{std::string("@tag4"), std::string("@tag5"),
                   std::string("@tag8"), std::string("@tag9")};
  compiler::tag_expression tc(
      "@tag1 xor (((@tag2 and @tag3) or (@tag4 and@tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_false_w_xor)
{
  value_array tags{std::string("@tag1"), std::string("@tag4"),
                   std::string("@tag5"), std::string("@tag8"),
                   std::string("@tag9")};
  compiler::tag_expression tc(
      "@tag1 xor (((@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_false_1)
{
  value_array tags{std::string("@tag2"), std::string("@tag5"),
                   std::string("@tag8"), std::string("@tag9")};
  compiler::tag_expression tc(
      "@tag1 or (((@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, a_big_condition_false_2)
{
  value_array tags;
  compiler::tag_expression tc(
      "@tag1 or (((@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, xor_true)
{
  value_array tags{std::string("@tag2")};
  compiler::tag_expression tc("@tag1 xor @tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, syntax_error_1)
{
  value_array tags{std::string("@tag2")};
  compiler::tag_expression tc("@tag1 helloworld @tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, syntax_error_2)
{
  value_array tags{std::string("@tag2")};
  compiler::tag_expression tc("tag1 @tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, syntax_error_3)
{
  value_array tags{std::string("@tag")};
  compiler::tag_expression tc("$tag");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, empty_expression_1)
{
  value_array tags{std::string("@tag")};
  compiler::tag_expression tc;
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, empty_expression_2)
{
  value_array tags{std::string("@tag")};
  compiler::tag_expression tc("");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, default_ctor_and_set_1)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc(
      "@tag1 or (( (@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, default_ctor_and_set_2)
{
  value_array tags{std::string("@tag1"), std::string("@tag4"),
                   std::string("@tag5"), std::string("@tag8"),
                   std::string("@tag9")};
  compiler::tag_expression tc(
      "@tag1 xor (((@tag2 and @tag3) or (@tag4 and @tag5) or @tag7) and (@tag8 "
      "and @tag9))");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, not_1_and_not_2)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc("not @tag1 and not @tag2");
  EXPECT_FALSE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, tag1_and_tag2_1)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc("@tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, tag1_and_tag2_2)
{
  value_array tags{std::string("@tag1"), std::string("@tag2")};
  compiler::tag_expression tc("@tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}
TEST(tag_evaluation, tag2_and_tag3)
{
  value_array tags{std::string("@tag2"), std::string("@tag3")};
  compiler::tag_expression tc("@tag3 and @tag2");
  EXPECT_TRUE(tc.evaluate(tags.size(), tags.rbegin()));
}