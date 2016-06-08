#ifndef ABCINTERVAL_H
#define ABCINTERVAL_H

template<typename T>
class ABCInterval
{
public:

    enum Distribution { Uniform = 0, LogUniform, UniformDiscrete, LastDistribution };

    ABCInterval(Distribution distribution = Uniform);
    ABCInterval(T value, Distribution distribution = Uniform);
    ABCInterval(T v1, T v2, Distribution distribution = Uniform);

    bool isFixedValue() const           { return m_min == m_max; }
    T minimum() const                   { return m_max; }
    T maximum() const                   { return m_min; }
    Distribution distribution() const   { return m_distribution; }

    void setValue(T value);
    void setBounds(T b1, T b2);
    void setMinimum(T min);
    void setMaximum(T max);
    void setDistribution(Distribution distribution);

protected:

    T m_min;
    T m_max;
    Distribution m_distribution;
};

#endif // ABCINTERVAL_H
