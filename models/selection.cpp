#include "selection.h"

#include <QObject>

////////////////////////////////////////////////////////////////////////////////////////////////////
const QStringList Selection::m_modelNames = (QStringList()
                                             << QObject::tr("SOS")
                                             << QObject::tr("FDS")
                                             << QObject::tr("DPS"));

const QStringList Selection::m_modelFullNames = (QStringList()
                                                << QObject::tr("Symmetric overdominant selection")
                                                << QObject::tr("Frequency dependent selection")
                                                << QObject::tr("Dominant positive selection"));

const QStringList Selection::m_heterogeneityNames = (QStringList()
                                                     << QObject::tr("Uniform")
                                                     << QObject::tr("Latitudinal")
                                                     << QObject::tr("Longitudinal"));

////////////////////////////////////////////////////////////////////////////////////////////////////
Selection::Selection(Model model, Heterogeneity heterogeneity) :
    QObject(),
    m_model(model),
    m_heterogeneity(heterogeneity),
    m_uniformCoefficient(0.0),
    m_northEastCoefficient(0.0),
    m_southWestCoefficient(0.0)
{
}

////////////////////////////////////// STATIC METHODS //////////////////////////////////////////////
QString Selection::modelName(Model model)
{
    return m_modelNames.at(model);
}

QString Selection::modelFullName(Model model)
{
    return m_modelFullNames.at(model);
}

QString Selection::heterogeneityName(Heterogeneity heterogeneity)
{
    return m_heterogeneityNames.at(heterogeneity);
}

///////////////////////////////////////// GETTERS /////////////////////////////////////////////////
Selection::Model Selection::model() const
{
    return m_model;
}

QString Selection::modelName() const
{
    return modelName(m_model);
}

QString Selection::modelFullName() const
{
    return modelFullName(m_model);
}

Selection::Heterogeneity Selection::heterogeneity() const
{
    return m_heterogeneity;
}

QString Selection::heterogeneityName() const
{
    return heterogeneityName(m_heterogeneity);
}

ABCInterval<double> Selection::uniformCoefficient() const
{
    return m_uniformCoefficient;
}

ABCInterval<double> Selection::northEastCoefficient() const
{
    return m_northEastCoefficient;
}

ABCInterval<double> Selection::southWestCoefficient() const
{
    return m_southWestCoefficient;
}

///////////////////////////////////////// SETTERS //////////////////////////////////////////////////
void Selection::setModel(Model model)
{
    setModel((int)model);
}

void Selection::setModel(int model)
{
    if (model != m_model)
    {
        if (model < FirstModel)
        {
            m_model = FirstModel;
        }
        else if (model > LastModel)
        {
            m_model = LastModel;
        }
        else
        {
            m_model = (Model)model;
        }
        emit modelChanged(m_model);
    }
}

void Selection::setHeterogeneity(Heterogeneity heterogeneity)
{
    setHeterogeneity((int)heterogeneity);
}

void Selection::setHeterogeneity(int heterogeneity)
{
    if (heterogeneity != m_heterogeneity)
    {
        if (heterogeneity < FirstHeterogeneity)
        {
            m_heterogeneity = FirstHeterogeneity;
        }
        else if (heterogeneity > LastHeterogeneity)
        {
            m_heterogeneity = LastHeterogeneity;
        }
        else
        {
            m_heterogeneity = (Heterogeneity)heterogeneity;
        }
        emit heterogeneityChanged(m_heterogeneity);
    }
}

void Selection::setUniformCoefficient(ABCInterval<double> coeff)
{
    if (coeff != m_uniformCoefficient)
    {
        m_uniformCoefficient = coeff;

        if (m_uniformCoefficient < 0.0)
        {
            m_uniformCoefficient.setMinimum(0.0);
        }
        if (m_uniformCoefficient > 1.0)
        {
            m_uniformCoefficient.setMaximum(1.0);
        }
        emit uniformCoefficientChanged(m_uniformCoefficient);
    }
}

void Selection::setNorthEastCoefficient(ABCInterval<double> coeff)
{
    if (coeff != m_northEastCoefficient)
    {
        m_northEastCoefficient = coeff;

        if (m_northEastCoefficient < 0.0)
        {
            m_northEastCoefficient.setMinimum(0.0);
        }
        if (m_northEastCoefficient > 1.0)
        {
            m_northEastCoefficient.setMaximum(1.0);
        }
        emit northEastCoefficientChanged(m_northEastCoefficient);
    }
}

void Selection::setSouthWestCoefficient(ABCInterval<double> coeff)
{
    if (coeff != m_southWestCoefficient)
    {
        m_southWestCoefficient = coeff;

        if (m_southWestCoefficient < 0.0)
        {
            m_southWestCoefficient.setMinimum(0.0);
        }
        if (m_southWestCoefficient > 1.0)
        {
            m_southWestCoefficient.setMaximum(1.0);
        }
        emit southWestCoefficientChanged(m_southWestCoefficient);
    }
}
