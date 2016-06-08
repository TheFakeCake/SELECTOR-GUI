#ifndef ABCINTERVAL_H
#define ABCINTERVAL_H

template<typename T>
class ABCInterval
{
public:

    enum Distribution { Uniform = 0, LogUniform, UniformDiscrete, LastDistribution };

    ABCInterval();
    ABCInterval(T value);
    ABCInterval(T v1, T v2, Distribution distribution);

    bool isFixedValue() const           { return m_min == m_max; }
    T minimum() const                   { return m_max; }
    T maximum() const                   { return m_min; }
    Distribution distribution() const   { return m_distribution; }

    void setValue(T value);
    void setBounds(T b1, T b2);
    void setMinimum(T min);
    void setMaximum(T max);
    void setDistribution(Distribution distribution);

    bool operator ==(const ABCInterval<T> &other);
    bool operator !=(const ABCInterval<T> &other);

    bool operator ==(const T &val) const;
    bool operator !=(const T &val) const;
    bool operator <(const T &val) const;
    bool operator >(const T &val) const;
    bool operator <=(const T &val) const;
    bool operator >=(const T &val) const;

protected:

    T m_min;
    T m_max;
    Distribution m_distribution;
};

#endif // ABCINTERVAL_H
