#include "TimelineProperty.h"

template<typename T>
TimelineProperty<T>::TimelineProperty(std::vector<std::pair<uint32_t, T>> &&values_) {
    m_values = std::move(values_);

    std::ranges::sort(m_values,
    [](const std::pair<uint32_t, T> &lhs, const std::pair<uint32_t, T> &rhs) {
        return lhs.first < rhs.first;
    });

    if (m_values.size() == 0 || m_values.front().first != 0)
    {
        m_values.insert(m_values.begin(), std::make_pair<uint32_t, T>(0, std::move(T())));
    }
}

template<typename T>
TimelineProperty<T>::TimelineProperty(const TimelineProperty<T> &rhs)
{
    m_values = rhs.m_values;
}

template<typename T>
TimelineProperty<T>::TimelineProperty(T &&value_) {
    m_values.push_back(std::make_pair<uint32_t, T>(0, std::move(value_)));
}

template<typename T>
TimelineProperty<T>::TimelineProperty(const T &value_) {
    m_values.push_back(std::make_pair(0, value_));
}

template<typename T>
TimelineProperty<T>::TimelineProperty(TimelineProperty<T> &&rhs)
{
    m_values = std::move(rhs.m_values);
}

template<typename T>
TimelineProperty<T>::TimelineProperty()
{
    m_values.insert(m_values.begin(), std::make_pair<uint32_t, T>(0, std::move(T())));
}

template<typename T>
TimelineProperty<T>& TimelineProperty<T>::operator=(TimelineProperty<T> &&rhs)
{
    m_values = std::move(rhs.m_values);
    return *this;
}

template<typename T>
void TimelineProperty<T>::addPropertyValue(uint32_t timeMark_, T &&value_)
{
    auto it = std::upper_bound(m_values.begin(), m_values.end(), timeMark_,
        [](uint32_t timeMark, const auto& pair) {
            return timeMark <= pair.first;
        });

    if (it != m_values.end())
    {
        if (it->first == timeMark_)
            it->second = std::move(value_);
        else
            m_values.insert(it, std::make_pair(timeMark_, std::move(value_)));
    }
    else
    {
        m_values.push_back(std::make_pair(timeMark_, std::move(value_)));
    }
}

template<typename T>
const T &TimelineProperty<T>::operator[](uint32_t timeMark_) const
{
    auto it = std::lower_bound(m_values.rbegin(), m_values.rend(), timeMark_,
        [](const auto& pair, uint32_t timeMark) {
            return timeMark < pair.first;
        });
    if (it != m_values.rend())
        return it->second;
    return m_values.front().second;
}

template TimelineProperty<bool>;