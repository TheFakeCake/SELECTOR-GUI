#include "ABCInterval.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> ABCInterval<T>::ABCInterval(Distribution distribution) :
    m_distribution(distribution)
{
}

template<typename T> ABCInterval<T>::ABCInterval(T value, Distribution distribution) :
    m_distribution(distribution)
{
    set(value);
}

template<typename T> ABCInterval<T>::ABCInterval(T v1, T v2, Distribution distribution) :
    m_distribution(distribution)
{
    set(v1, v2);
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
        set(min);
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
        set(max);
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
