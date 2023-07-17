#ifndef FIXED_QUEUE_H_
#define FIXED_QUEUE_H_

#include <stdexcept>
#include <iostream>

template <typename T, int len>
class FixedQueue
{
public:
    FixedQueue() {};

    const T& operator[](int rhs) const;

    void push(const T &val);

    constexpr int getLen() const;

    int getFilled() const;

private:
    T data[len];
    int nextToFill = 0;
    int filled = 0;

};

namespace utils
{
    template <typename T, int len>
    void printQueue(const FixedQueue<T, len> &q, char separator = ' ')
    {
        for (int i = 0; i < q.getFilled(); ++i)
        {
            std::cout << q[i] << separator;
        }
        std::cout << "\n";
    }
}

#endif