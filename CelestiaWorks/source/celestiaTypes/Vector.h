#pragma once


#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif


namespace celestia
{
	template<int L, typename T> struct CELESTIA_WORKS Vec;


	template<typename T>
	constexpr Vec<4, T> calc(Vec<4, T> const& a, Vec<4, T> const& b)
	{
		return Vec<4, T>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	template<typename T>
	constexpr Vec<4, T> calcAdd(Vec<4, T> const& a, Vec<4, T> const& b)
	{
		return Vec<4, T>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}


	//vec2
	template<typename T>
	struct CELESTIA_WORKS Vec<2,T>
	{
		T x, y;
		
		// <Constructor>
		//constexpr Vec() = default;
		//constexpr Vec(Vec const& v) = default;

		constexpr Vec()
			:x(0), y(0)
		{}
		constexpr Vec(Vec<2, T> const& value)
			:x(value.x),y (value.y)
		{}
		constexpr explicit Vec(T value)
			: x(value),y(value)
		{}
		constexpr Vec(T _x, T _y)
			: x(_x),y(_y)
		{}

		template<typename U>
		constexpr Vec(U _x, U _y)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y))
		{}

		//  <Artihmetic operators>
		constexpr Vec<2, T>& operator=(Vec const& v) = default;

		template<typename U>
		constexpr Vec<2, T>& operator=(Vec<2, U> const& v)
		{
			this->x = static_cast<T>(v.x);
			this->y = static_cast<T>(v.y);
			return *this;
		}

		template<typename U>
		constexpr Vec<2, T>& operator+=(Vec<2, U> const& v)
		{
			this->x += static_cast<T>(v.x);
			this->y += static_cast<T>(v.y);
			return *this;
		}

		template<typename U>
		constexpr Vec<2, T>& operator-=(Vec<2, U> const& v)
		{
			this->x -= static_cast<T>(v.x);
			this->y -= static_cast<T>(v.y);
			return *this;
		}

		template<typename U>
		constexpr Vec<2, T>& operator*=(Vec<2, U> const& v)
		{
			this->x *= static_cast<T>(v.x);
			this->y *= static_cast<T>(v.y);
			return *this;
		}

		template<typename U>
		constexpr Vec<2, T>& operator/=(Vec<2, U> const& v)
		{
			this->x /= static_cast<T>(v.x);
			this->y /= static_cast<T>(v.y);
			return *this;
		}

	};

	//vec3
	template<typename T>
	struct CELESTIA_WORKS Vec<3, T>
	{
		union
		{
			struct { T x, y, z; };
			struct { T r, g, b; };
		};

		// <Constructor>
		constexpr Vec()
			:x(0), y(0), z(0)
		{}
		constexpr Vec(Vec<3, T> const& value)
			:x(value.x), y(value.y),z(value.z)
		{}
		constexpr explicit Vec(T value)
			: x(value), y(value),z(value.z)
		{}
		constexpr Vec(T _x, T _y, T _z)
			: x(_x), y(_y), z(_z)
		{}

		template<typename U>
		constexpr Vec(U _x, U _y, U _z)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z))
		{}

		//  <Artihmetic operators>
		constexpr Vec<3, T>& operator=(Vec const& v) = default;

		template<typename U>
		constexpr Vec<3, T>& operator=(Vec<3, U> const& v)
		{
			this->x = static_cast<T>(v.x);
			this->y = static_cast<T>(v.y);
			this->z = static_cast<T>(v.z);
			return *this;
		}

		template<typename U>
		constexpr Vec<3, T>& operator+=(Vec<3, U> const& v)
		{
			this->x += static_cast<T>(v.x);
			this->y += static_cast<T>(v.y);
			this->z += static_cast<T>(v.z);
			return *this;
		}

		template<typename U>
		constexpr Vec<3, T>& operator-=(Vec<3, U> const& v)
		{
			this->x -= static_cast<T>(v.x);
			this->y -= static_cast<T>(v.y);
			this->z -= static_cast<T>(v.z);
			return *this;
		}

		template<typename U>
		constexpr Vec<3, T>& operator*=(Vec<3, U> const& v)
		{
			this->x *= static_cast<T>(v.x);
			this->y *= static_cast<T>(v.y);
			this->z *= static_cast<T>(v.z);
			return *this;
		}

		template<typename U>
		constexpr Vec<3, T>& operator/=(Vec<3, U> const& v)
		{
			this->x /= static_cast<T>(v.x);
			this->y /= static_cast<T>(v.y);
			this->z /= static_cast<T>(v.z);
			return *this;
		}


	};

	//vec4
	template<typename T>
	struct CELESTIA_WORKS Vec<4, T>
	{
		union
		{
			struct { T x, y, z, w; };
			struct { T r, g, b, a; };
		};

		constexpr Vec()
			:x(0), y(0), z(0), w(0)
		{}
		// <Constructor>
		constexpr Vec(Vec<4, T> const& value)
			:x(value.x), y(value.y), z(value.z), w(value.w)
		{}
		constexpr explicit Vec(T value)
			: x(value), y(value), z(value.z), w(value.w)
		{}
		constexpr Vec(T _x, T _y, T _z, T _w)
			: x(_x), y(_y), z(_z), w(_w)
		{}

		template<typename U>
		constexpr Vec(U _x, U _y, U _z, U _w)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z)), w(static_cast<T>(_w))
		{}

		//  <Artihmetic operators>
		constexpr Vec<4, T>& operator=(Vec const& v) = default;

		template<typename U>
		constexpr Vec<4, T>& operator=(Vec<4, U> const& v)
		{
			this->x = static_cast<T>(v.x);
			this->y = static_cast<T>(v.y);
			this->z = static_cast<T>(v.z);
			this->w = static_cast<T>(v.w);
			return *this;
		}

		template<typename U>
		constexpr Vec<4, T>& operator+=(Vec<4, U> const& v)
		{
			this->x += static_cast<T>(v.x);
			this->y += static_cast<T>(v.y);
			this->z += static_cast<T>(v.z);
			this->w += static_cast<T>(v.w);
			return *this;
		}
		
		constexpr Vec<4, T>& operator+=(T const& s)
		{
			this->x += s;
			this->y += s;
			this->z += s;
			this->w += s;
			return *this;
		}

		template<typename U>
		constexpr Vec<4, T>& operator-=(Vec<4, U> const& v)
		{
			this->x -= static_cast<T>(v.x);
			this->y -= static_cast<T>(v.y);
			this->z -= static_cast<T>(v.z);
			this->w -= static_cast<T>(v.w);
			return *this;
		}
		
		template<typename U>
		constexpr Vec<4, T>& operator*=(Vec<4, U> const& v)
		{
			this->x *= static_cast<T>(v.x);
			this->y *= static_cast<T>(v.y);
			this->z *= static_cast<T>(v.z);
			this->w *= static_cast<T>(v.w);
			return *this;
		}

		template<typename U>
		constexpr Vec<4, T>& operator*=(U scalar) 
		{
			return (*this = calc<T>(*this, Vec<4, T>(scalar)));
		}

		template<typename U>
		constexpr Vec<4, T>& operator/=(Vec<4, U> const& v)
		{
			this->x /= static_cast<T>(v.x);
			this->y /= static_cast<T>(v.y);
			this->z /= static_cast<T>(v.z);
			this->w /= static_cast<T>(v.w);
			return *this;
		}

		constexpr Vec<4, T> operator*(const T v) const
		{
			//return ((*this) *= value);
			Vec<4, T> result;
			result.x = this->x * v;
			result.y = this->y * v;
			result.z = this->z * v;
			result.w = this->w * v;
			return result;
		}

		

		constexpr Vec<4, T> operator+(const Vec<4,T>& vec) const
		{
			return calcAdd(*this, vec);
		}

	};


	//583 rivi

	typedef Vec<2, float> Vec2;
	typedef Vec<3, float> Vec3;
	typedef Vec<4, float> Vec4;

	typedef Vec<2, int> Vec2i;
	typedef Vec<3, int> Vec3i;
	typedef Vec<4, int> Vec4i;

	typedef Vec<2, unsigned int> Vec2u;
	typedef Vec<3, unsigned int> Vec3u;
	typedef Vec<4, unsigned int> Vec4u;

}

