#ifndef ABCINTERVAL_H
#define ABCINTERVAL_H

#include <QObject>

template<typename T>
class ABCInterval
{
public:

    enum Distribution {
        Uniform = 0,
        LogUniform,
        UniformDiscrete,
        LastDistribution = UniformDiscrete
    };

    ABCInterval();
    ABCInterval(T value);
    ABCInterval(T firstBound, T secondBound, Distribution distribution);

    bool isFixedValue() const           { return m_b1 == m_b2; }
    T firstBound() const                { return m_b1; }
    T secondBound() const               { return m_b2; }
    T minimum() const                   { return qMin(m_b1, m_b2); }
    T maximum() const                   { return qMax(m_b1, m_b2); }
    Distribution distribution() const   { return m_distribution; }

    void setValue(T value);
    void setBounds(T firstBound, T secondBound);
    void setFirstBound(T bound);
    void setSecondBound(T bound);
    void setMinimum(T min);
    void setMaximum(T max);
    void setDistribution(Distribution distribution);
    void setDistribution(int distribution);

    bool operator ==(const ABCInterval<T> &other) const;
    bool operator !=(const ABCInterval<T> &other) const;

    bool operator ==(const T &val) const;
    bool operator !=(const T &val) const;
    bool operator <(const T &val) const;
    bool operator >(const T &val) const;
    bool operator <=(const T &val) const;
    bool operator >=(const T &val) const;

protected:

    T m_b1;
    T m_b2;
    Distribution m_distribution;
};

#endif // ABCINTERVAL_H
