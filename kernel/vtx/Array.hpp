#pragma once

#include <stddef.h>

namespace vtx
{
template <typename T, size_t N> class Array
{
  public:
    Array() = default;
    Array(T initialValue)
    {
        for (size_t i = 0; i < N; i++)
            m_Data[i] = initialValue;
    }

    constexpr size_t Length() const
    {
        return N;
    }
    T &operator[](size_t i)
    {
        return m_Data[i];
    }
    const T &operator[](size_t i) const
    {
        return m_Data[i];
    }

  private:
    T m_Data[N];
};
} // namespace vtx
