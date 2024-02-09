#pragma once

#include <span>
#include <vector>
#include <string>
#include <filesystem>

namespace cwt::details
{

struct feature_file
{
  std::string path;
  std::vector<unsigned long> lines;
};

struct options
{
  bool quiet{false};
  std::vector<feature_file> files;
  std::string tag_expression{""};
};

class terminal_arguments
{
 public:
  terminal_arguments(int argc, const char* argv[]);
  const options& get_options() const noexcept;

 private:
  void process();
  void process_path(std::string_view sv);
  void process_option(std::span<const char*>::iterator it);
  void find_feature_in_dir(const std::filesystem::path& dir);

 private:
  std::span<const char*> m_args;
  options m_options;
};

}  // namespace cwt::details
