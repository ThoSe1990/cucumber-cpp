#include <format>
#include <string_view>

#include <gtest/gtest.h>

#include "../src/options.hpp"
#include "test_paths.hpp"

TEST(options, init_obj)
{
  cuke::internal::terminal_arguments targs;
  EXPECT_FALSE(targs.get_options().quiet);
  EXPECT_TRUE(targs.get_options().files.empty());
  EXPECT_TRUE(targs.get_options().tags.empty());
}
TEST(options, file_path_doesnt_exist)
{
  const char* argv[] = {"program", "path/doesnt/exist/to/file.feature"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_TRUE(targs.get_options().files.empty());
}
TEST(options, file_path_does_exist)
{
  std::string path =
      std::format("{}/test_files/any.feature", unittests::test_dir());
  const char* argv[] = {"program", path.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_FALSE(targs.get_options().files.empty());
  EXPECT_EQ(targs.get_options().files[0].path, std::string(argv[1]));
}

namespace details
{
[[nodiscard]] bool has_file(
    const std::vector<cuke::internal::feature_file>& container,
    std::string_view file_name)
{
  for (const cuke::internal::feature_file& file : container)
  {
    if (file.path.ends_with(file_name))
    {
      return true;
    }
  }
  return false;
}
}  // namespace details

TEST(options, find_files_in_dir)
{
  std::string path = std::format("{}/test_files", unittests::test_dir());
  const char* argv[] = {"program", path.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_EQ(targs.get_options().files.size(), 3);

  EXPECT_TRUE(details::has_file(targs.get_options().files, "any.feature"));
  EXPECT_TRUE(details::has_file(targs.get_options().files, "example.feature"));
  EXPECT_TRUE(details::has_file(targs.get_options().files, "fail.feature"));
}
namespace details
{
static std::string remove_trailing_char(std::string_view str, std::size_t n)
{
  return std::string(str.substr(0, str.size() - n));
}
}  // namespace details
TEST(options, file_path_does_exist_w_line)
{
  std::string path =
      std::format("{}/test_files/any.feature:3", unittests::test_dir());
  const char* argv[] = {"program", path.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_FALSE(targs.get_options().files.empty());
  EXPECT_EQ(targs.get_options().files[0].path,
            details::remove_trailing_char(argv[1], 2));
  ASSERT_FALSE(targs.get_options().files[0].lines_to_compile.empty());
  EXPECT_EQ(targs.get_options().files[0].lines_to_compile.at(0), 3);
}
TEST(options, file_path_does_exist_w_lines)
{
  std::string path = std::format("{}/test_files/any.feature:3:123:9999",
                                 unittests::test_dir());
  const char* argv[] = {"program", path.c_str()};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_FALSE(targs.get_options().files.empty());
  EXPECT_EQ(targs.get_options().files[0].path,
            details::remove_trailing_char(argv[1], 11));
  ASSERT_EQ(targs.get_options().files[0].lines_to_compile.size(), 3);
  EXPECT_EQ(targs.get_options().files[0].lines_to_compile.at(2), 3);
  EXPECT_EQ(targs.get_options().files[0].lines_to_compile.at(1), 123);
  EXPECT_EQ(targs.get_options().files[0].lines_to_compile.at(0), 9999);
}
TEST(options, tag_expression_1)
{
  const char* argv[] = {"program", "-t", "@tag1 or @tag2"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_FALSE(targs.get_options().tags.empty());
  EXPECT_TRUE(
      targs.get_options().tags.evaluate(std::vector{std::string{"@tag1"}}));
  EXPECT_TRUE(
      targs.get_options().tags.evaluate(std::vector{std::string{"@tag2"}}));
  EXPECT_FALSE(
      targs.get_options().tags.evaluate(std::vector{std::string{"@tag3"}}));
}
TEST(options, tag_expression_2)
{
  const char* argv[] = {"program", "--tags", "@tag1 or @tag2"};
  int argc = sizeof(argv) / sizeof(argv[0]);
  cuke::internal::terminal_arguments targs;
  targs.initialize(argc, argv);
  ASSERT_FALSE(targs.get_options().tags.empty());
  EXPECT_TRUE(
      targs.get_options().tags.evaluate(std::vector{std::string{"@tag1"}}));
  EXPECT_TRUE(
      targs.get_options().tags.evaluate(std::vector{std::string{"@tag2"}}));
  EXPECT_FALSE(
      targs.get_options().tags.evaluate(std::vector{std::string{"@tag3"}}));
}
