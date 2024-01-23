
#include "cucumber.hpp"
#include "feature.hpp"

namespace cwt::details::compiler
{
cucumber::cucumber(std::string_view source) : compiler(source, "")
{
  init();
}
cucumber::cucumber(std::string_view source, std::string_view filename)
    : compiler(source, filename)
{
  init();
}
function cucumber::make_function() noexcept
{
  return std::make_unique<chunk>(get_chunk());
}
void cucumber::init()
{
  m_parser->advance();
  m_parser->skip_linebreaks();
}
void cucumber::compile()
{
  switch (m_parser->current().type)
  {
    case token_type::feature:
    {
      feature f(this);
      f.compile();
    }
    break;
    default:
    {
      m_parser->error_at(m_parser->current(), "Expect FeatureLine");
    }
  }

  finish_chunk();
}

}  // namespace cwt::details
