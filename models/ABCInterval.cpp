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

template<typename T> ABCInterval<T>::ABCInterval(T firstBound, T secondBound, Distribution distribution) :
    m_distribution(distribution)
{
    setBounds(firstBound, secondBound);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> void ABCInterval<T>::setValue(T value)
{
    m_b1 = value;
    m_b2 = value;
}

template<typename T> void ABCInterval<T>::setBounds(T firstBound, T secondBound)
{
    m_b1 = firstBound;
    m_b2 = secondBound;
}

template<typename T> void ABCInterval<T>::setFirstBound(T bound)
{
    m_b1 = bound;
}

template<typename T> void ABCInterval<T>::setSecondBound(T bound)
{
    m_b2 = bound;
}

template<typename T> void ABCInterval<T>::setMinimum(T min)
{
    if (m_b2 < m_b1)
    {
        m_b2 = min;
    }
    else
    {
        m_b1 = min;
    }
}

template<typename T> void ABCInterval<T>::setMaximum(T max)
{
    if (m_b1 > m_b2)
    {
        m_b1 = max;
    }
    else
    {
        m_b2 = max;
    }
}

template<typename T>
void ABCInterval<T>::setDistribution(ABCInterval<T>::Distribution distribution)
{
    m_distribution = distribution;
}

template<typename T>
void ABCInterval<T>::setDistribution(int distribution)
{
    if (distribution >= 0 && distribution <= LastDistribution)
    {
        m_distribution = (Distribution)distribution;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> bool ABCInterval<T>::operator ==(const ABCInterval<T> &other) const
{
    return (minimum() == other.minimum() && maximum() == other.maximum() && m_distribution == other.m_distribution);
    /*
    bool res = (minimum() == other.minimum() && maximum() == other.maximum());
    if (! isFixedValue())
    {
        res = (res && m_distribution == other.m_distribution);
    }
    return res;
    */
}

template<typename T> bool ABCInterval<T>::operator !=(const ABCInterval<T> &other) const
{
    return ! operator ==(other);
}

template<typename T> bool ABCInterval<T>::operator ==(const T &val) const
{
    return maximum() == val && minimum() == val;
}

template<typename T> bool ABCInterval<T>::operator !=(const T &val) const
{
    return ! operator ==(val);
}

template<typename T> bool ABCInterval<T>::operator <(const T &val) const
{
    return maximum() < val || minimum() < val;
}

template<typename T> bool ABCInterval<T>::operator >(const T &val) const
{
    return maximum() > val || minimum() > val;
}

template<typename T> bool ABCInterval<T>::operator <=(const T &val) const
{
    return maximum() <= val || minimum() <= val;
}

template<typename T> bool ABCInterval<T>::operator >=(const T &val) const
{
    return maximum() >= val || minimum() >= val;
}

template class ABCInterval<int>;
template class ABCInterval<double>;
