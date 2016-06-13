#include "ABCInterval.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> ABCInterval<T>::ABCInterval() :
    m_distribution(Uniform)
{
}

template<typename T> ABCInterval<T>::ABCInterval(T value) :
    m_distribution(Uniform)
{
    setValue(value);
}

template<typename T> ABCInterval<T>::ABCInterval(T v1, T v2, Distribution distribution) :
    m_distribution(distribution)
{
    setBounds(v1, v2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> void ABCInterval<T>::setValue(T value)
{
    m_min = value;
    m_max = value;
}

template<typename T> void ABCInterval<T>::setBounds(T b1, T b2)
{
    if (b1 < b2)
    {
        m_min = b1;
        m_max = b2;
    }
    else
    {
        m_min = b2;
        m_max = b1;
    }
}

template<typename T> void ABCInterval<T>::setMinimum(T min)
{
    if (min > m_max)
    {
        setValue(min);
    }
    else
    {
        m_min = min;
    }
}

template<typename T> void ABCInterval<T>::setMaximum(T max)
{
    if (max < m_min)
    {
        setValue(max);
    }
    else
    {
        m_max = max;
    }
}

template<typename T>
void ABCInterval<T>::setDistribution(ABCInterval<T>::Distribution distribution)
{
    m_distribution = distribution;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> bool ABCInterval<T>::operator ==(const ABCInterval<T> &other) const
{
    bool res = (m_min == other.m_min && m_max == other.m_max);
    if (! isFixedValue())
    {
        res = (res && m_distribution == other.m_distribution);
    }
    return res;
}

template<typename T> bool ABCInterval<T>::operator !=(const ABCInterval<T> &other) const
{
    return ! operator ==(other);
}

template<typename T> bool ABCInterval<T>::operator ==(const T &val) const
{
    return m_max == val && m_min == val;
}

template<typename T> bool ABCInterval<T>::operator !=(const T &val) const
{
    return ! operator ==(val);
}

template<typename T> bool ABCInterval<T>::operator <(const T &val) const
{
    return m_max < val || m_min < val;
}

template<typename T> bool ABCInterval<T>::operator >(const T &val) const
{
    return m_max > val || m_min > val;
}

template<typename T> bool ABCInterval<T>::operator <=(const T &val) const
{
    return m_max <= val || m_min <= val;
}

template<typename T> bool ABCInterval<T>::operator >=(const T &val) const
{
    return m_max >= val || m_min >= val;
}

template class ABCInterval<int>;
template class ABCInterval<double>;
