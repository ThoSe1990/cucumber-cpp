#pragma once
#include <iostream>
#include "value.hpp"

// https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxCAAzBqkAA6oCoRODB7evnrJqY4CQSHhLFEx8baY9vkMQgRMxASZPn5cFVXptfUEhWGR0XEJCnUNTdmtQ109xaUDAJS2qF7EyOwc5rHByN5YANQmsW5OQ8SYrPvYJhoAguub25h7BwBumA4k55c3ZhsMW167%2BzcBAAnolMAB9AjEJiEBQfa6fT5bJgKBQ7YioADukMRAHYrNcdjtEl4IrQ8MgQJ8iRjsQQIGgGEMdkN0CAQC83sRAcECB8zAA2HboJh1UgsghskCpABeEII6Kx4tZ7Nl8sV2PoRgICFmOxAOxY4JFdQgJqYevFRtpEFp80N4Np4K1wB1tqxzsYrt11L2%2BMRhL9ABFfYzmbzzELUGDoURuQBWKzxoMQFXSvByyE7NC0PVhvmBkz%2BwM0zAEJYMB3mkyJ61YgBUdc1Xp11hzNZDsQJVyJRZDgcSxDwT1F7FDAmZac5cZ5gn5QqN1a7vrTaqzTf23aJq4z6qbnu1CE3eM73aRBlRO34qD2COLPeJpPJlN9ToV0eioveiY7qclqt3LM7WzCcCwfIsLFvB9S3LYhKzfCBF1FJhxVpK1HQ9F0dVmTcoN7XF%2BxuAchxHAgxxLCUpWnd4DgjWJsCrZCINaMxSFiUhfSJLjuJ43jeOkeNSAFDiKL4sTxNxUgAA5SAATj7Y8KJ3TMFX3LCEEeIMdliRSbgI3TPl5Q0YQYCA9TxLcr1QG8qAMwM0zQLwFUBQErxrCwNA7dzPOTR43FctNXFoXTt3/FBFmcg5XNsxMfJDRMuA7PyArCoKQso9lHMi/yDjc2KvMTMwkpc3LAoYdBguXeywqy5LcpiixEuTbziqi0rUvKyrLIciK6rcPLGoKwbfJK/qyoq9KeqcvqBqa%2BKLCKka2rGjqJqqh8puy6L3MW%2Ba4pm8aupXGretGgbdvcuaDtWo7qqlWqzoai7CtanKVqlNL1s49EywrHZPK%2B/Trg4eZaE4eNeD8DgtFIVBOH8yxrBZRZlgedYeFIAhNBB%2BYAGsQHjeMADpYnjWTcQFeMNHjLhYlkqnWLBjhJEh7HYc4XgFBABIsehkHSDgWAYEQcKWESOhonISg0DFiWYmRIwuDMDRWhoWgyOILmIAiNmImCepgU4DG9eYYhgQAeQibRXl5jGZbYQRzYYWhDb50gsAiLxgDcMRaC57heCwFhDGAcQ3fwE4HGHTB/ZhzBVFeJzVhh3lKjZ8kImhM2PCwI3eChPAWDz%2BYqAMYAFAANTwTBMXNsEoYx/hBBEMR2CkGRBEUFR1Dd3RWgMIwUGsax9DwCIucgeZo2qf2AFpWX2INTERyxlaJWfzdiXhUBeYghywCezLaG30hccrRhaUhAmCXoSn6VpcjSAQL5yFIn4YKY%2BhicZKhPgROhGJ4Zoeg7B/xqMMboN9pj31sBAl%2B4wIGfzvt/eYCgUYrAkKDcGrM3Zww4DsVQUkBSzwFJIbMA9gA7CVkTDQRMuA7AgLgQgJA9jfC4LMfO2NZh4xAJILgdCKZSXiBTLgApZKSCEfoTgLNSBF1iFJImskNAaCIcrAUUlZKUxpsJKGMM8Gc25pjLhAthYQCQBFEkBApYQEwPgOMeA2Sq1kC3cQ7cm7yCUGoNmfdSCYmhIkYuUiOAQ1ILo7enBzZOUsTsVAVACFEJIWQhWlDqG0PoRADwst6DEFYbEdhnC%2BbcKCTIouAp%2BEaFkrJWmuJZJ5I0LESQEjQls30bYQxvMtBFPxo0ompCNDfEkOIoh1NlHCSZlvZpuCOZGMKVgjgZgcF6OmR0nGpBd6pGcJIIAA%3D

namespace cuke
{

class table
{
 public:
  table() = default;
  table(const table&) = default;
  table(table&&) = default;
  table& operator=(const table&) = default;
  table& operator=(table&&) = default;
  table(value_array data, std::size_t col_count);

  class row
  {
   public:
    row(const value_array& data, std::size_t col_count);
    row(value_array::const_iterator current, std::size_t col_count);

    [[nodiscard]] const cuke::value& operator[](std::size_t idx) const;

   private:
    value_array::const_iterator m_current;
    std::size_t m_col_count;
  };

  class const_iterator
  {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const row;
    using difference_type = std::ptrdiff_t;
    using pointer = const row*;
    using reference = const row&;

    explicit const_iterator(value_array::const_iterator it, std::size_t col_count)
        : m_current(it), m_col_count(col_count)
    {
    }
    row operator*() const { return row(m_current, m_col_count); }
    const_iterator& operator++()
    {
      m_current += m_col_count;
      return *this;
    }
    bool operator!=(const const_iterator& rhs) const
    {
      return rhs.m_current != m_current;
    }

   private:
    value_array::const_iterator m_current;
    std::size_t m_col_count;
  };

  class iterator_provider
  {
   public:
    iterator_provider(cuke::value_array::const_iterator begin, cuke::value_array::const_iterator end,  std::size_t col_count)
        : m_begin(begin), m_end(end), m_col_count(col_count)
    {
    }
    const_iterator end() const { return const_iterator(m_end, m_col_count); }
    const_iterator begin() const { return const_iterator(m_begin, m_col_count); }

   private:
    value_array::const_iterator m_begin;
    value_array::const_iterator m_end;
    std::size_t m_col_count;
  };

  [[nodiscard]] std::size_t row_count() const noexcept;
  [[nodiscard]] std::size_t col_count() const noexcept;
  [[nodiscard]] std::size_t cells_count() const noexcept;

  [[nodiscard]] row operator[](std::size_t idx) const;

  [[nodiscard]] iterator_provider raw() const
  {
    return iterator_provider(m_data.begin(), m_data.end(), m_col_count);
  }

 private:
  value_array m_data;
  std::size_t m_col_count{0};
};

}  // namespace cuke
