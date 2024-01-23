#include "feature.hpp"
#include "scenario.hpp"

namespace cwt::details
{

feature::feature(cuke_compiler* enclosing)
    : m_enclosing(enclosing), compiler(*enclosing)
{
  m_parser->consume(token_type::feature, "Expect FeatureLine");
  
  [[maybe_unused]] std::size_t idx = create_name(location());
  m_parser->advance();
  m_parser->advance_to(token_type::scenario, token_type::scenario_outline,
                       token_type::tag, token_type::background,
                       token_type::eof);
}
feature::~feature()
{
  finish_chunk();
  m_enclosing->emit_constant(std::make_unique<chunk>(get_chunk()));
  m_enclosing->emit_constant(op_code::define_var, get_chunk().name());
  m_enclosing->emit_bytes(op_code::get_var,
                          m_enclosing->get_chunk().last_constant());
  m_enclosing->emit_bytes(op_code::call, 0);
}
void feature::compile()
{
  if (m_parser->match(token_type::scenario))
  {
    scenario s(this);
    s.compile();
  }
  else
  {
    m_parser->error_at(m_parser->current(), "Expect ScenarioLine");
  }
}

}  // namespace cwt::details