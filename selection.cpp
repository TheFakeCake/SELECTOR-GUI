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
Selection::Selection(Model model, Heterogeneity heterogeneity) : m_model(model), m_heterogeneity(heterogeneity),
    m_uniformCoefficient(0.0), m_northEastCoefficient(0.0), m_southWestCoefficient(0.0)
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

double Selection::uniformCoefficient() const
{
    return m_uniformCoefficient;
}

double Selection::northEastCoefficient() const
{
    return m_northEastCoefficient;
}

double Selection::southWestCoefficient() const
{
    return m_southWestCoefficient;
}

///////////////////////////////////////// SETTERS //////////////////////////////////////////////////
void Selection::setModel(Model model)
{
    m_model = model;
}

void Selection::setHeterogeneity(Heterogeneity heterogeneity)
{
    m_heterogeneity = heterogeneity;
}

void Selection::setUniformCoefficient(double coeff)
{
    if (coeff >= 0.0 && coeff <= 1.0)
    {
        m_uniformCoefficient = coeff;
    }
}

void Selection::setNorthEastCoefficient(double coeff)
{
    if (coeff >= 0.0 && coeff <= 1.0)
    {
        m_northEastCoefficient = coeff;
    }
}

void Selection::setSouthWestCoefficient(double coeff)
{
    if (coeff >= 0.0 && coeff <= 1.0)
    {
        m_southWestCoefficient = coeff;
    }
}
