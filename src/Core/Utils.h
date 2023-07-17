#ifndef UTILS_H_
#define UTILS_H_

namespace utils
{
    template <typename T>
    inline T clamp(const T& val, const T &min, const T &max)
    {
    	if (val < min)
    		return min;
    
    	if (val > max)
    		return max;
    
    	return val;
    }

    template <typename T>
    inline bool sameSign(const T &v1, const T &v2)
    {
    	return (v1 > 0 && v2 > 0 || v1 < 0 && v2 < 0);
    }

}

template <typename T, size_t len>
std::ostream& operator<< (std::ostream& out, const std::array<T, len>& arr)
{
    std::cout << "[";
    for (int i = 0; i < len; ++i)
    {
        std::cout << arr[i];
        if (i != len - 1)
            std::cout << ", ";
    }
    std::cout << "]";
    return out;
}

#endif