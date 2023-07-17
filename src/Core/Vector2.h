#ifndef VECTOR2_H_
#define VECTOR2_H_
#include <cmath>
#include <iostream>
#include "Utils.h"

template <typename T>
struct Vector2
{
	T x, y;
	constexpr inline Vector2(T nx = 0, T ny = 0)
	{
		x = nx;
		y = ny;
	}

	template <typename TR>
	constexpr inline Vector2(const Vector2<TR> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}

	template <typename TR>
	constexpr inline Vector2<T> &operator=(const Vector2<TR> &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	template <typename TR>
	constexpr inline bool operator==(const Vector2<TR> &rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	template<typename TR>
	constexpr inline auto operator+(const Vector2<TR>& rhs) const -> Vector2<decltype(x+rhs.x)>
	{
		return { x + rhs.x, y + rhs.y };
	}

	template<typename TR>
	constexpr inline auto operator-(const Vector2<TR>& rhs) const -> Vector2<decltype(x-rhs.x)>
	{
		return { x - rhs.x, y - rhs.y };
	}

	template<typename TR>
	constexpr inline auto operator*(const TR& num) const -> Vector2<decltype(x*num)>
	{
		return { x * num, y * num };
	}

	template<typename TR>
	constexpr inline auto operator/(const TR& num) const -> Vector2<decltype(x/num)>
	{
		return { x / num, y / num };
	}

    template<typename TR>
	constexpr inline Vector2<T> &operator+=(const Vector2<TR>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
        return *this;
	}

    template<typename TR>
	constexpr inline Vector2<T> operator-=(const Vector2<TR>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
        return *this;
	}

    template<typename TR>
	constexpr inline Vector2<T> operator*=(const TR& num)
	{
		x *= num;
		y *= num;
        return *this;
	}

    template<typename TR>
	constexpr inline Vector2<T> operator/=(const TR& num)
	{
		x /= num;
		y /= num;
        return *this;
	}

	constexpr inline auto getLen() const
	{
		return sqrt(x * x + y * y);
	}

	constexpr inline auto getSqLen() const
	{
		return x * x + y * y;
	}

	constexpr inline Vector2<decltype(x / sqrt(x))> normalised() const
	{
		float l = sqrt(x * x + y * y);
		if (l == 0.0f)
			return *this;

		return {x / l, y / l};
	}

    template<typename TR>
	constexpr inline auto mulComponents(const Vector2<TR>& rhs) const -> Vector2<decltype(x * rhs.x)>
	{
		return { x * rhs.x, y * rhs.y };
	}
};

template <typename T>
std::ostream& operator<< (std::ostream& out, const Vector2<T>& vec)
{
    out << vec.x << " " << vec.y;
    return out;
}

namespace utils
{
	template <typename T>
	inline Vector2<T> clamp(const Vector2<T>& val, const Vector2<T> &min, const Vector2<T> &max)
	{
		return {clamp(val.x, min.x, max.x), clamp(val.y, min.y, max.y)};
	}
}

struct HorizontalOverlapResult
{
	bool selfIsOnLeft = false;
	bool selfIsInside = false;
	bool otherIsInside = false;
	float overlapWidth = 0;
	inline bool doesOverlap() const
	{
		return selfIsOnLeft || selfIsInside || otherIsInside  || overlapWidth;
	}
};

//Only rectangle hitbox
struct Hitbox
{
	float x, y, w, h;
	inline Hitbox(float nx = 0, float ny = 0, float nw = 0, float nh = 0)
	{
		x = nx;
		y = ny;
		w = nw;
		h = nh;
	}
	inline Hitbox operator+(const Vector2<float>& rhs) const
	{
		return { x + rhs.x, y + rhs.y, w, h };
	}
	inline int isCollideWith_x(const Hitbox& hbox) const
	{
		if (x <= hbox.x && x + w > hbox.x)
			return 1;
		if (x + w >= hbox.x + hbox.w && x < hbox.x + hbox.w)
			return 2;
		if (x >= hbox.x && x + w <= hbox.x + hbox.w)
			return 4;
		else
			return 0;
	}
	inline int isCollideWith_y(const Hitbox& hbox) const
	{
		if (y <= hbox.y && y + h > hbox.y)
			return 1;
		if (y + h >= hbox.y + hbox.h && y < hbox.y + hbox.h)
			return 2;
		if (y >= hbox.y && y + h <= hbox.y + hbox.h)
			return 4;
		else
			return 0;
	}
	inline bool isCollideWith(const Hitbox& hbox) const
	{
		return isCollideWith_x(hbox) && isCollideWith_y(hbox);
	}

	//-1 if beyond left bound
	// 1 if beyond right bound
	// 0 if within bounds
	inline int isWithinHorizontalBounds(float leftBound_, float rightBound_) const
	{
		if (x < leftBound_)
			return -1;
		
		if (x + w > rightBound_)
			return 1;

		return 0;
	}
	
	inline float rangeToLeftBound(float leftBound_) const
	{
		return x - leftBound_;
	}

	inline float rangeToRightBound(float rightBound_) const
	{
		return rightBound_ - (x + w);
	}

	inline bool doesExceedOrTouchBoundaries(float leftBound_, float rightBound_) const
	{
		if (x <= leftBound_)
			return true;
		
		if (x + w >= rightBound_)
			return true;

		return false;
	}

	inline HorizontalOverlapResult getHorizontalOverlap(const Hitbox &rhs) const
	{
		HorizontalOverlapResult res;

		if (x > rhs.x && x + w < rhs.x + rhs.w)
		{
			res.selfIsInside = true;
		}
		else if (rhs.x > x && rhs.x + rhs.w < x + w)
		{
			res.otherIsInside = true;
		}
		else if (x <= rhs.x && x + w > rhs.x && x + w <= rhs.x + rhs.w)
		{
			res.selfIsOnLeft = true;
			res.overlapWidth = x + w - rhs.x;
		}
		else if (x > rhs.x && x + w > rhs.x + rhs.w && x < rhs.x + rhs.w)
		{
			res.overlapWidth = rhs.x + rhs.w - x;
		}

		return res;
	}


	inline Vector2<float> getPos() const
	{
		return {x, y};
	}
	inline Vector2<float> getSize() const
	{
		return {w, h};
	}
	inline Vector2<float> getCenter() const
	{
		return {x + w / 2.0f, y + h / 2.0f};
	}
	
};

#endif