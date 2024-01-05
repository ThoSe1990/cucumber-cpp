#pragma once

#include <stdexcept>

#include "chunk.hpp"
#include "value.hpp"

namespace cwt::details
{

std::size_t chunk::size() const noexcept { return m_code.size(); }
std::size_t chunk::constants_count() const noexcept
{
  return m_constants.size();
}
std::size_t chunk::last_constant() const noexcept
{
  return m_constants.size() - 1;
}

const uint32_t& chunk::back() const noexcept { return m_code.back(); }
value& chunk::constant(const std::size_t index)
{
  if (index < m_constants.size()) [[likely]]
  {
    return m_constants[index];
  }
  else [[unlikely]]
  {
    throw std::out_of_range("Chunk: Constants out of range");
  }
}

value& chunk::constants_back() noexcept { return m_constants.back(); }

void chunk::push_byte(op_code byte, const std::size_t line)
{
  push_byte(static_cast<uint32_t>(byte), line);
}
void chunk::push_byte(uint32_t byte, const std::size_t line)
{
  m_code.push_back(byte);
  m_lines.push_back(line);
}

chunk::const_iterator chunk::cbegin() const
{
  return chunk::const_iterator(m_code.cbegin());
}
chunk::const_iterator chunk::cend() const
{
  return chunk::const_iterator(m_code.cend());
}
op_code chunk::instruction(std::size_t index) const
{
  return static_cast<op_code>(m_code[index]);
}
uint32_t chunk::at(std::size_t index) const { return m_code[index]; }
uint32_t chunk::operator[](std::size_t index) const
{
  if (index < m_code.size()) [[likely]]
  {
    return m_code[index];
  }
  else [[unlikely]]
  {
    throw std::out_of_range("Chunk Operator[]: Byte out of range");
  }
}

const uint32_t& chunk::const_iterator::operator*() const { return *m_current; }
uint32_t chunk::const_iterator::next()
{
  uint32_t next = *m_current;
  ++m_current;
  return next;
}
op_code chunk::const_iterator::next_as_instruction()
{
  return static_cast<op_code>(next());
}
chunk::const_iterator& chunk::const_iterator::operator++()
{
  ++m_current;
  return *this;
}
bool chunk::const_iterator::operator==(const chunk::const_iterator& rhs) const
{
  return m_current == rhs.m_current;
}
bool chunk::const_iterator::operator!=(const chunk::const_iterator& rhs) const
{
  return m_current != rhs.m_current;
}

}  // namespace cwt::details
