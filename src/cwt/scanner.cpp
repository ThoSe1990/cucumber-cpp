#include "scanner.hpp"
#include "identifiers/german.hpp"
namespace cwt::details
{

static bool is_alpha(char c)
{
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_digit(char c) { return c >= '0' && c <= '9'; }

scanner::scanner(std::string_view source) : m_source(source)
{
  find_language();
}
void scanner::find_language()
{
  for (;;)
  {
    char c = peek();
    switch (c)
    {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        ++m_line;
        advance();
        break;
      case '#':
      {
        bool can_change_language = true;
        advance();
        while (!is_at_end() && !end_of_line())
        {
          skip_whitespaces();
          if (can_change_language &&
              m_source.substr(m_pos).starts_with(m_lan_keyword))
          {
            m_pos += m_lan_keyword.size();
            skip_whitespaces();
            const std::size_t start = m_pos;
            while (is_alpha(peek())) advance();
            set_language(m_source.substr(start, m_pos - start));
          }
          else
          {
            can_change_language = false;
            advance();
          }
        }
        break;
      }
      default:
        return;
    }
  }
}

void scanner::set_language(std::string_view country)
{
  if (country == "de")
  {
    m_identifiers = std::make_unique<german>();
  }
}

char scanner::advance()
{
  ++m_pos;
  return m_source[m_pos - 1];
}

token scanner::make_token(token_type type) const
{
  return token{type, m_source.substr(m_start, m_pos - m_start), m_line};
}
token scanner::make_token(token_type type, std::size_t start,
                          std::size_t end) const
{
  return token{type, m_source.substr(start, end - start), m_line};
}
token scanner::error_token(std::string_view msg) const
{
  return token{token_type::error, msg, m_line};
}

char scanner::peek() const
{
  if (!is_at_end()) [[likely]]
  {
    return m_source[m_pos];
  }
  else [[unlikely]]
  {
    return '\0';
  }
}
char scanner::peek_next() const
{
  if (chars_left() > 0)
  {
    return m_source[m_pos + 1];
  }
  else
  {
    return '\0';
  }
}
bool scanner::is_at_end() const { return m_pos >= m_source.size(); }
bool scanner::tripple_quotes() const
{
  if (chars_left() < 2)
  {
    return false;
  }
  else
  {
    return m_source[m_pos] == '"' && m_source[m_pos + 1] == '"' &&
           m_source[m_pos + 2] == '"';
  }
}
std::size_t scanner::chars_left() const
{
  if (is_at_end())
  {
    return 0;
  }
  return m_source.size() - 1 - m_pos;
}
bool scanner::end_of_line() const
{
  if (peek() == '\n')
  {
    return true;
  }
  else if (peek() == '\r' && peek_next() == '\n')
  {
    return true;
  }
  else
  {
    return false;
  }
}
void scanner::skip_whitespaces()
{
  for (;;)
  {
    char c = peek();
    switch (c)
    {
      case ' ':
      case '\t':
        advance();
        break;
      default:
        return;
    }
  }
}
void scanner::skip()
{
  for (;;)
  {
    char c = peek();
    switch (c)
    {
      case ' ':
      case '\t':
        advance();
        break;
      case '#':
        while (!is_at_end() && !end_of_line())
        {
          advance();
        }
      default:
        return;
    }
  }
}

token scanner::number()
{
  bool is_double = false;
  while (is_digit(peek()))
  {
    advance();
  }
  if (peek() == '.')
  {
    is_double = true;
    advance();
  }
  if (is_digit(peek_next()))
  {
    while (is_digit(peek()))
    {
      advance();
    }
  }
  return is_double ? make_token(token_type::double_value)
                   : make_token(token_type::long_value);
}

token scanner::tag()
{
  for (;;)
  {
    if (is_at_end() || peek() == ' ' || end_of_line())
    {
      break;
    }
    advance();
  }
  return make_token(token_type::tag);
}
token scanner::variable()
{
  while (peek() != '>')
  {
    if (is_at_end() || end_of_line())
    {
      return error_token("Expect '>' after variable.");
    }
    advance();
  }
  advance();
  return make_token(token_type::variable);
}
token scanner::string()
{
  while (peek() != '"' && !is_at_end())
  {
    if (peek() == '\n')
    {
      return error_token("Unexpected linebreak in string value.");
    }
    advance();
  }

  if (is_at_end())
  {
    return error_token("Unterminated string.");
  }
  advance();
  return make_token(token_type::string_value);
}
token scanner::doc_string()
{
  const std::size_t start = m_pos;
  std::size_t end = m_pos;
  while (!tripple_quotes())
  {
    if (end_of_line())
    {
      end = m_pos;
      if (peek() == '\r') end--;
      m_line++;
    }
    advance();

    if (is_at_end())
    {
      return error_token("Unterminated doc string.");
    }
  }

  skip();
  advance();
  advance();
  advance();
  return make_token(token_type::doc_string, start, end);
}

token scanner::word()
{
  while (!is_at_end() && !end_of_line())
  {
    if (peek() == ' ' || peek() == '|')
    {
      break;
    }
    advance();
  }
  return make_token(token_type::word);
}

token scanner::scan_token()
{
  skip();
  m_start = m_pos;

  if (is_at_end())
  {
    return make_token(token_type::eof);
  }
  char c = advance();

  if (is_digit(c))
  {
    return number();
  }
  switch (c)
  {
    case '|':
      return make_token(token_type::vertical);
    case '-':
      return make_token(token_type::minus);
    case '*':
      return make_token(token_type::step);
    case '@':
      return tag();
    case '<':
      return variable();
    case '"':
    {
      if (peek() == '"' && peek_next() == '"')
      {
        advance();
        advance();
        advance();
        return doc_string();
      }
      else
      {
        return string();
      }
    }
    case '\r':
    {
      if (peek() == '\n')
      {
        advance();
        m_line++;
        return make_token(token_type::linebreak);
      }
      else
      {
        return make_token(token_type::error);
      }
    }
    case '\n':
    {
      m_line++;
      return make_token(token_type::linebreak);
    }
  }

  token_type identifier = m_identifiers->get_token(m_source.substr(m_pos - 1));
  if (identifier != token_type::none)
  {
    return make_token(identifier);
  }

  return word();
}

}  // namespace cwt::details
