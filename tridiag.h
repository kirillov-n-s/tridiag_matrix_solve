#pragma once

#include "vector.h"

//class / func decl (forward)
namespace num
{
    template<std::floating_point T>
    class tridiag;

    template<std::floating_point T>
    bool operator==(const tridiag<T> &lhs, const tridiag<T> &rhs);
    template<std::floating_point T>
    bool operator!=(const tridiag<T> &lhs, const tridiag<T> &rhs);

    template<std::floating_point T>
    tridiag<T> operator-(const tridiag<T> &mat);
    template<std::floating_point T>
    tridiag<T> operator+(const tridiag<T> &lhs, const tridiag<T> &rhs);
    template<std::floating_point T>
    tridiag<T> operator-(const tridiag<T> &lhs, const tridiag<T> &rhs);

    template<std::floating_point T>
    tridiag<T> operator*(const tridiag<T> &mat, const T &scalar);
    template<std::floating_point T>
    tridiag<T> operator*(const T &scalar, const tridiag<T> &mat);
    template<std::floating_point T>
    vector<T> operator*(const tridiag<T> &mat, const vector<T> &vec);

    template<std::floating_point T>
    std::ostream &operator<<(std::ostream &out, const tridiag<T> &mat);
    template<std::floating_point T>
    std::istream &operator>>(std::istream &in, tridiag<T> &mat);
}

//class def
namespace num
{
    template<std::floating_point T>
    class tridiag
    {
    public:
        vector<T> a, b, c;

        tridiag(std::size_t size = 1, const T &value = 0);
        tridiag(const vector<T> &a,
                const vector<T> &b,
                const vector<T> &c);
        tridiag(std::size_t size, const T &min, const T &max);
        tridiag(std::size_t size,
                const T &min_a, const T &max_a,
                const T &min_b, const T &max_b,
                const T &min_c, const T &max_c);

        tridiag(const tridiag &other);
        tridiag &operator=(const tridiag &other);

        std::size_t size() const;

        friend bool operator==<T>(const tridiag &lhs, const tridiag &rhs);
        friend bool operator!=<T>(const tridiag &lhs, const tridiag &rhs);

        friend tridiag operator-<T>(const tridiag &mat);
        friend tridiag operator+<T>(const tridiag &lhs, const tridiag &rhs);
        friend tridiag operator-<T>(const tridiag &lhs, const tridiag &rhs);

        friend tridiag operator*<T>(const tridiag<T> &mat, const T &scalar);
        friend vector<T> operator*<T>(const tridiag<T> &mat, const vector<T> &vec);

        friend std::ostream &operator<<<T>(std::ostream &out, const tridiag &mat);
        friend std::istream &operator>><T>(std::istream &in, tridiag &mat);
    };
}

//func def
namespace num
{
    template<std::floating_point T>
    tridiag<T>::tridiag(std::size_t size, const T &value)
        : a(size - 1, value, 2), b(size, value), c(size - 1, value) {}

    template<std::floating_point T>
    tridiag<T>::tridiag(const vector<T> &a,
                        const vector<T> &b,
                        const vector<T> &c)
        : a(a, 2), b(b), c(c) {}

    template<std::floating_point T>
    tridiag<T>::tridiag(std::size_t size, const T &min, const T &max)
        : a(size - 1, min, max, 2),
          b(size, min, max),
          c(size - 1, min, max)
          {}

    template<std::floating_point T>
    tridiag<T>::tridiag(std::size_t size,
                        const T &min_a, const T &max_a,
                        const T &min_b, const T &max_b,
                        const T &min_c, const T &max_c)
        : a(size - 1, min_a, max_a, 2),
          b(size, min_b, max_b),
          c(size - 1, min_c, max_c)
          {}

    template<std::floating_point T>
    tridiag<T>::tridiag(const tridiag &other)
            : a(other.a), b(other.b), c(other.c) {}

    template<std::floating_point T>
    tridiag<T> &tridiag<T>::operator=(const tridiag<T> &other)
    {
        a = other.a;
        b = other.b;
        c = other.c;
        return *this;
    }

    template<std::floating_point T>
    std::size_t tridiag<T>::size() const
    {
        return b.size();
    }

    template<std::floating_point T>
    bool operator==(const tridiag<T> &lhs, const tridiag<T> &rhs)
    {
        return lhs.a == rhs.a && lhs.b == rhs.b && lhs.c == rhs.c;
    }

    template<std::floating_point T>
    bool operator!=(const tridiag<T> &lhs, const tridiag<T> &rhs)
    {
        return !(lhs == rhs);
    }

    template<std::floating_point T>
    tridiag<T> operator-(const tridiag<T> &mat)
    {
        return tridiag<T>(-mat.a, -mat.b, -mat.c);
    }

    template<std::floating_point T>
    tridiag<T> operator+(const tridiag<T> &lhs, const tridiag<T> &rhs)
    {
        return tridiag<T>(lhs.a + rhs.a, lhs.b + rhs.b, lhs.c + rhs.c);
    }

    template<std::floating_point T>
    tridiag<T> operator-(const tridiag<T> &lhs, const tridiag<T> &rhs)
    {
        return tridiag<T>(lhs.a - rhs.a, lhs.b - rhs.b, lhs.c - rhs.c);
    }

    template<std::floating_point T>
    tridiag<T> operator*(const tridiag<T> &mat, const T &scalar)
    {
        return tridiag<T>(mat.a * scalar, mat.b * scalar, mat.c * scalar);
    }

    template<std::floating_point T>
    tridiag<T> operator*(const T &scalar, const tridiag<T> &mat)
    {
        return mat * scalar;
    }

    template<std::floating_point T>
    vector<T> operator*(const tridiag<T> &mat, const vector<T> &vec)
    {
        int n = mat.size();
        vector<T> res(n);
        res[1] = mat.b[1] * vec[1] + mat.c[1] * vec[2];
        for (int i = 2; i < n; i++)
            res[i] = mat.a[i] * vec[i - 1] + mat.b[i] * vec[i] + mat.c[i] * vec[i + 1];
        res[n] = mat.a[n] * vec[n - 1] + mat.b[n] * vec[n];
        return res;
    }

    template<std::floating_point T>
    std::ostream &operator<<(std::ostream &out, const tridiag<T> &mat)
    {
        int n = mat.size();
        int places = format<T>(out);
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (j == i + 1)
                    out << std::setw(places) << mat.c[i];
                else if (j == i)
                    out << std::setw(places) << mat.b[i];
                else if (j == i - 1)
                    out << std::setw(places) << mat.a[i];
                else
                    out << std::setw(places) << 0;
            }
            out << std::endl;
        }
        return out;
    }

    template<std::floating_point T>
    std::istream &operator>>(std::istream &in, tridiag<T> &mat)
    {
        /*int n = mat.size();
        T placeholder;
        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (j == i + 1)
                    in >> mat.c[i];
                else if (j == i)
                    in >> mat.b[i];
                else if (j == i - 1)
                    in >> mat.a[i];
                else
                    in >> placeholder;
            }
        }
        return in;*/

        return in >> mat.a >> mat.b >> mat.c;
    }
}
