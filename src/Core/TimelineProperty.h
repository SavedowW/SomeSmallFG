#ifndef TIMELINE_PROPERTY_H_
#define TIMELINE_PROPERTY_H_

#include <vector>
#include <algorithm>
#include <ranges>
#include <utility>

template<typename T>
class TimelineProperty {
public:
    TimelineProperty(std::vector<std::pair<uint32_t, T>> &&values_);
    TimelineProperty(T &&value_);
    TimelineProperty(const T &value_);
    TimelineProperty();

    // std::pair that requires copy constructor is bad civilization
    TimelineProperty(const TimelineProperty<T> &rhs);
    TimelineProperty& operator=(const TimelineProperty<T> &rhs) = delete;
    TimelineProperty(TimelineProperty<T> &&rhs);
    TimelineProperty& operator=(TimelineProperty<T> &&rhs);

    void addPropertyValue(uint32_t timeMark_, T &&value_);

    const T &operator[](uint32_t timeMark_) const;

private:
    std::vector<std::pair<uint32_t, T>> m_values;

};

#endif