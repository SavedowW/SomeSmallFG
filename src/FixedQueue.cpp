#include "FixedQueue.h"
#include "ActionResolver.h"


template <typename T, int len>
const T& FixedQueue<T, len>::operator[](int rhs) const
{
    if (rhs >= filled)
        throw std::runtime_error("Trying to access non-existing element");

    int lastFilled = nextToFill-1;
    if (lastFilled < 0)
        lastFilled = len - 1;

    int id = nextToFill - 1 - rhs;
    if (id < 0)
        id = len + id;

    return data[id];
}

template <typename T, int len>
void FixedQueue<T, len>::push(const T &val)
{
    data[nextToFill] = val;
    nextToFill = (nextToFill + 1) % len;
    filled = std::min(filled + 1, len);
}

template <typename T, int len>
constexpr int FixedQueue<T, len>::getLen() const
{
    return len;
}

template <typename T, int len>
int FixedQueue<T, len>::getFilled() const
{
    return filled;
}

template FixedQueue<InputState, 30>;