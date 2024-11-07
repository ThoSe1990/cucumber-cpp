#pragma once

#include <algorithm>
#include <stdexcept>

#include "step.hpp"
#include "hooks.hpp"
#include "expression.hpp"

namespace cuke
{
namespace internal
{
template <typename Hook>
static void run_hook(const std::vector<Hook>& hooks)
{
  std::for_each(hooks.begin(), hooks.end(), [](const auto& h) { h.call(); });
}
template <typename Hook>
static void run_hook(const std::vector<Hook>& hooks,
                     const std::vector<std::string>& tags)
{
  std::for_each(hooks.begin(), hooks.end(),
                [&tags](const auto& h)
                {
                  if (h.valid_tag(tags))
                  {
                    h.call();
                  }
                });
}
class registry
{
 public:
  void sort_steps_by_type()
  {
    sort_steps([](const internal::step& lhs, const internal::step& rhs)
               { return lhs.step_type() < rhs.step_type(); });
  }

  void clear() noexcept
  {
    m_steps.clear();
    m_hooks.before.clear();
    m_hooks.after.clear();
    m_hooks.before_step.clear();
    m_hooks.after_step.clear();
    m_hooks.before_all.clear();
    m_hooks.after_all.clear();
    m_custom_conversions.clear();
  }

  // TODO: refactor this if POC works
  void push_custom_conversion(const expression& conversion)
  {
    if (std::find_if(m_custom_conversions.begin(), m_custom_conversions.end(),
                     [&conversion](const expression& current) {
                       return conversion.key == current.key;
                     }) != m_custom_conversions.end())
    {
      // TODO: is this possible at compile time?
      throw std::runtime_error(
          std::format("Custom type '{}' already exists", conversion.key));
    }
    m_custom_conversions.push_back(conversion);
  }
  // TODO: refactor this if POC works
  [[nodiscard]] const expression& get_custom_conversion(
      std::string_view key) const
  {
    auto it = std::find_if(
        m_custom_conversions.begin(), m_custom_conversions.end(),
        [&key](const expression& current) { return key == current.key; });
    if (it != m_custom_conversions.end())
    {
      // TODO: is this possible at compile time?
      return *it;
    }
    throw std::runtime_error(
        std::format("Custom type '{}' already exists", key));
  }

  void push_step(const internal::step& s) noexcept { m_steps.push_back(s); }
  [[nodiscard]] const std::vector<internal::step>& steps() const noexcept
  {
    return m_steps;
  }

  void push_hook_before(const internal::hook& h) noexcept
  {
    m_hooks.before.push_back(h);
  }

  [[nodiscard]] const std::vector<internal::hook>& hooks_before() const noexcept
  {
    return m_hooks.before;
  }

  void push_hook_after(const internal::hook& h) noexcept
  {
    m_hooks.after.push_back(h);
  }
  [[nodiscard]] const std::vector<internal::hook>& hooks_after() const noexcept
  {
    return m_hooks.after;
  }
  void push_hook_after_all(const internal::hook& h) noexcept
  {
    m_hooks.after_all.push_back(h);
  }
  [[nodiscard]] const std::vector<internal::hook>& hooks_after_all()
      const noexcept
  {
    return m_hooks.after_all;
  }
  void push_hook_before_all(const internal::hook& h) noexcept
  {
    m_hooks.before_all.push_back(h);
  }
  [[nodiscard]] const std::vector<internal::hook>& hooks_before_all()
      const noexcept
  {
    return m_hooks.before_all;
  }
  void push_hook_before_step(const internal::hook& h) noexcept
  {
    m_hooks.before_step.push_back(h);
  }
  [[nodiscard]] const std::vector<internal::hook>& hooks_before_step()
      const noexcept
  {
    return m_hooks.before_step;
  }

  void push_hook_after_step(const internal::hook& h) noexcept
  {
    m_hooks.after_step.push_back(h);
  }
  [[nodiscard]] const std::vector<internal::hook>& hooks_after_step()
      const noexcept
  {
    return m_hooks.after_step;
  }

  void run_hook_before(const std::vector<std::string>& tags) const noexcept
  {
    run_hook(m_hooks.before, tags);
  }
  void run_hook_after(const std::vector<std::string>& tags) const noexcept
  {
    run_hook(m_hooks.after, tags);
  }
  void run_hook_before_step() const noexcept { run_hook(m_hooks.before_step); }
  void run_hook_after_step() const noexcept { run_hook(m_hooks.after_step); }
  void run_hook_before_all() const noexcept { run_hook(m_hooks.before_all); }
  void run_hook_after_all() const noexcept { run_hook(m_hooks.after_all); }

 private:
  template <typename Compare>
  void sort_steps(Compare compare)
  {
    std::sort(m_steps.begin(), m_steps.end(), compare);
  }

  std::vector<internal::step> m_steps;
  struct
  {
    std::vector<internal::hook> before;
    std::vector<internal::hook> after;
    std::vector<internal::hook> before_all;
    std::vector<internal::hook> after_all;
    std::vector<internal::hook> before_step;
    std::vector<internal::hook> after_step;
  } m_hooks;
  std::vector<expression> m_custom_conversions;
};

}  // namespace internal

[[nodiscard]] internal::registry& registry();

}  // namespace cuke
