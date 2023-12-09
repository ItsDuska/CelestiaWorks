#pragma once
#include "celestiaTypes/Vector.h"
#include <cassert>
#include <cstring>

namespace celestia
{
	template<int L, typename T> struct CELESTIA_WORKS Mat;

	template<typename T>
	struct CELESTIA_WORKS Mat<4,T>
	{
		typedef Vec<4, T> colType;
	private:
		colType value[4];
	public:
		colType& operator[](int index)
		{
			assert(index < 4);
			return this->value[index];
		}
		constexpr colType const& operator[](int index) const
		{
			assert(index < 4);
			return this->value[index];
		}

		constexpr Mat()
			: value{
				colType(1, 0, 0, 0),
				colType(0, 1, 0, 0),
				colType(0, 0, 1, 0),
				colType(0, 0, 0, 1)
			}
		{}

		constexpr Mat(Mat<4, T> const& m)
			:value{
				colType(m[0]),
				colType(m[1]),
				colType(m[2]),
				colType(m[3])
			}
		{}

		constexpr explicit Mat(T s)
			:value{
				colType(s, 0.f, 0.f, 0.f),
				colType(0.f, s, 0.f, 0.f),
				colType(0.f, 0.f, s, 0.f),
				colType(0.f, 0.f, 0.f, s)
			}
		{}

		constexpr Mat(
			T const& x0, T const& y0, T const& z0, T const& w0,
			T const& x1, T const& y1, T const& z1, T const& w1,
			T const& x2, T const& y2, T const& z2, T const& w2,
			T const& x3, T const& y3, T const& z3, T const& w3
		)
			: value{
				colType(x0, y0, z0, w0),
				colType(x1, y1, z1, w1),
				colType(x2, y2, z2, w2),
				colType(x3, y3, z3, w3) }
		{}
		template<typename U>
		constexpr Mat<4, T>& operator=(Mat<4, U> const& m)
		{
			memcpy(&this->value, &m.value, 16 * sizeof(T));
		}

		template<typename U>
		constexpr Mat<4, T>& operator+=(U s)
		{
			this->value[0] += s;
			this->value[1] += s;
			this->value[2] += s;
			this->value[3] += s;
			return *this;
		}

		template<typename U>
		constexpr Mat<4, T>& operator+=(Mat<4, U> const& m)
		{
			this->value[0] += m[0];
			this->value[1] += m[1];
			this->value[2] += m[2];
			this->value[3] += m[3];
			return *this;
		}

		template<typename U>
		constexpr Mat<4, T>& operator-=(U s)
		{
			this->value[0] -= s;
			this->value[1] -= s;
			this->value[2] -= s;
			this->value[3] -= s;
			return *this;
		}

		template<typename U>
		constexpr Mat<4, T>& operator-=(Mat<4, U> const& m)
		{
			this->value[0] -= m[0];
			this->value[1] -= m[1];
			this->value[2] -= m[2];
			this->value[3] -= m[3];
			return *this;
		}

		template<typename U>
		constexpr Mat<4, T>& operator*=(U s)
		{
			this->value[0] *= s;
			this->value[1] *= s;
			this->value[2] *= s;
			this->value[3] *= s;
			return *this;
		}

		template<typename U>
		constexpr Mat<4, T>& operator*=(Mat<4, U> const& m)
		{
			return *this = *this *m;
		}

		template<typename U>
		constexpr Mat<4, T>& operator/=(U s)
		{
			this->value[0] /= s;
			this->value[1] /= s;
			this->value[2] /= s;
			this->value[3] /= s;
			return *this;
		}

	};

	typedef Mat<4, float> Mat4;

}