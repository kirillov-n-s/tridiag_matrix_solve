#pragma once

#include <vector>
#include <random>
#include <algorithm>
#include <functional>

#include "format.h"

//class / func decl (forward)
namespace num
{
    template<std::floating_point T>
    class vector;

    template<std::floating_point T>
    bool operator==(const vector<T> &lhs, const vector<T> &rhs);
    template<std::floating_point T>
    bool operator!=(const vector<T> &lhs, const vector<T> &rhs);

    template<std::floating_point T>
    vector<T> operator-(const vector<T> &vec);
    template<std::floating_point T>
    vector<T> operator+(const vector<T> &lhs, const vector<T> &rhs);
    template<std::floating_point T>
    vector<T> operator-(const vector<T> &lhs, const vector<T> &rhs);

    template<std::floating_point T>
    vector<T> operator*(const vector<T> &vec, const T &scalar);
    template<std::floating_point T>
    vector<T> operator*(const T &scalar, const vector<T> &vec);
    template<std::floating_point T>
    T operator*(const vector<T> &lhs, const vector<T> &rhs);

    template<std::floating_point T>
    std::ostream &operator<<(std::ostream &out, const vector<T> &vec);
    template<std::floating_point T>
    std::istream &operator>>(std::istream &in, vector<T> &vec);
}

//class def
namespace num
{
    template<std::floating_point T>
    class vector
    {
        std::vector<T> _values;

    public:
        int indexing = 1;

        vector(std::size_t size = 1, const T &value = 0, int indexing = 1);
        vector(std::size_t size, const T &min, const T &max, int indexing = 1);

        vector(const vector &other);
        vector &operator=(const vector &other);

        T &operator[](std::size_t pos);
        const T &operator[](std::size_t pos) const;

        std::size_t size() const;
        T len() const;
        T norm() const;

        friend bool operator==<T>(const vector &lhs, const vector &rhs);
        friend bool operator!=<T>(const vector &lhs, const vector &rhs);

        friend vector operator-<T>(const vector &vec);
        friend vector operator+<T>(const vector &lhs, const vector &rhs);
        friend vector operator-<T>(const vector &lhs, const vector &rhs);

        friend vector operator*<T>(const vector &vec, const T &scalar);
        friend T operator*<T>(const vector &lhs, const vector &rhs);

        friend std::ostream &operator<<<T>(std::ostream &out, const vector &vec);
        friend std::istream &operator>><T>(std::istream &in, vector &vec);
    };
}

//func def
namespace num
{
    template<std::floating_point T>
    vector<T>::vector(std::size_t size, const T &value, int indexing)
        : _values(size), indexing(indexing)
    {
        std::ranges::fill(_values, value);
    }

    template<std::floating_point T>
    vector<T>::vector(std::size_t size, const T &min, const T &max, int indexing)
        : _values(size), indexing(indexing)
    {
        std::mt19937 rng((std::random_device()()));
        std::uniform_real_distribution<T> distrib(min, max);
        std::ranges::generate(_values,
                              [&rng, &distrib]() { return distrib(rng); });
    }

    template<std::floating_point T>
    vector<T>::vector(const vector<T> &other)
            : _values(other._values), indexing(other.indexing) {}

    template<std::floating_point T>
    vector<T> &vector<T>::operator=(const vector<T> &other)
    {
        _values = other._values;
        return *this;
    }

    template<std::floating_point T>
    T &vector<T>::operator[](std::size_t pos) 
    {
        return _values[pos - indexing];
    }

    template<std::floating_point T>
    const T &vector<T>::operator[](std::size_t pos) const
    {
        return _values[pos - indexing];
    }

    template<std::floating_point T>
    std::size_t vector<T>::size() const
    {
        return _values.size();
    }

    template<std::floating_point T>
    T vector<T>::len() const
    {
        return std::sqrt(*this * *this);
    }

    template<std::floating_point T>
    T vector<T>::norm() const
    {
        return std::abs(*std::ranges::max_element(_values,
                                                  [](const auto& lhs, const auto& rhs) { return std::abs(lhs) < std::abs(rhs); }));
    }

    template<std::floating_point T>
    bool operator==(const vector<T> &lhs, const vector<T> &rhs)
    {
        return std::ranges::equal(lhs._values, rhs._values);
    }

    template<std::floating_point T>
    bool operator!=(const vector<T> &lhs, const vector<T> &rhs)
    {
        return !(lhs == rhs);
    }

    template<std::floating_point T>
    vector<T> operator-(const vector<T> &vec)
    {
        vector<T> res(vec.size());
        std::ranges::transform(vec._values, res._values.begin(), std::negate());
        return res;
    }

    template<std::floating_point T>
    vector<T> operator+(const vector<T> &lhs, const vector<T> &rhs)
    {
        vector<T> res(lhs.size());
        std::ranges::transform(lhs._values, rhs._values, res._values.begin(), std::plus());
        return res;
    }

    template<std::floating_point T>
    vector<T> operator-(const vector<T> &lhs, const vector<T> &rhs)
    {
        vector<T> res(lhs.size());
        std::ranges::transform(lhs._values, rhs._values, res._values.begin(), std::minus());
        return res;
    }

    template<std::floating_point T>
    vector<T> operator*(const vector<T> &vec, const T &scalar)
    {
        vector<T> res(vec.size());
        std::ranges::transform(vec._values, res._values.begin(),
                               [&scalar](const auto& val) { return val * scalar; });
        return res;
    }

    template<std::floating_point T>
    vector<T> operator*(const T &scalar, const vector<T> &vec)
    {
        return vec * scalar;
    }

    template<std::floating_point T>
    T operator*(const vector<T> &lhs, const vector<T> &rhs)
    {
        T res = 0;
        int n = lhs.size();
        for (int i = 0; i < n; i++)
            res += lhs._values[i] * rhs._values[i];
        return res;
    }

    template<std::floating_point T>
    std::ostream &operator<<(std::ostream &out, const vector<T> &vec)
    {
        int places = format<T>(out);
        std::ranges::for_each(vec._values, [&out, &places](const auto& val) { out << std::setw(places) << val; });
        return out << std::endl;
    }

    template<std::floating_point T>
    std::istream &operator>>(std::istream &in, vector<T> &vec)
    {
        std::ranges::for_each(vec._values, [&in](auto& val) { in >> val; });
        return in;
    }
}
