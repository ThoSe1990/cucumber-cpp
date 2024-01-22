#pragma once

#include <memory>
#include <string_view>
#include <type_traits>

#include "../token.hpp"
#include "../chunk.hpp"
#include "../parser.hpp"

namespace cwt::details
{
static constexpr std::string create_string(std::string_view sv);
static constexpr std::string create_string(std::string_view begin,
                                           std::string_view end);
static constexpr std::string create_string(const token& begin,
                                           const token& end);

class compiler
{
 public:
  [[nodiscard]] std::string location() const;
  [[nodiscard]] std::size_t create_name(const std::string& location);
  [[nodiscard]] bool error() const noexcept;
  [[nodiscard]] bool no_error() const noexcept;
  [[nodiscard]] chunk take_chunk() noexcept;

  void emit_byte(uint32_t byte);
  void emit_byte(op_code code);
  void emit_bytes(op_code code, uint32_t byte);
  uint32_t emit_jump();
  void patch_jump(uint32_t offset);
  void emit_hook(hook_type type);

  template <typename Arg>
  void emit_constant(op_code code, Arg&& arg)
  {
    emit_bytes(code, m_chunk.make_constant(std::forward<Arg>(arg)));
  }
  template <typename Arg>
  void emit_constant(Arg&& arg)
  {
    emit_bytes(op_code::constant,
               m_chunk.make_constant(std::forward<Arg>(arg)));
  }

 protected:
  compiler(std::string_view source);
  compiler(std::string_view source, std::string_view filename);

  template <typename Other,
            typename = std::enable_if_t<std::is_base_of_v<compiler, Other>>>
  compiler(const Other& other)
      : m_parser(other.m_parser),
        m_filename(other.m_filename),
        m_chunk(location())
  {
  }

 protected:
  std::shared_ptr<parser> m_parser;

 private:
  std::string m_filename;
  chunk m_chunk;
};

}  // namespace cwt::details
