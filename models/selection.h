#ifndef SELECTION_H
#define SELECTION_H

#include <QObject>
#include <QString>
#include <QStringList>

class Selection : public QObject
{
    Q_OBJECT

public:

    enum Model {
        SOS = 0,
        FDS,
        DPS,
        FirstModel = SOS,
        LastModel = DPS
    };

    enum Heterogeneity {
        Uniform = 0,
        Latitudinal,
        Longitudinal,
        FirstHeterogeneity = Uniform,
        LastHeterogeneity = Longitudinal
    };

    static QString modelName(Model model);
    static QString modelFullName(Model model);
    static QString heterogeneityName(Heterogeneity heterogeneity);

    Selection(Model model = Model::FirstModel, Heterogeneity = Heterogeneity::FirstHeterogeneity);

    Model model() const;
    QString modelName() const;
    QString modelFullName() const;
    Heterogeneity heterogeneity() const;
    QString heterogeneityName() const;
    double uniformCoefficient() const;
    double northEastCoefficient() const;
    double southWestCoefficient() const;

public slots:
    void setModel(Model model);
    void setModel(int model);
    void setHeterogeneity(Heterogeneity heterogeneity);
    void setHeterogeneity(int heterogeneity);
    void setUniformCoefficient(double coeff);
    void setNorthEastCoefficient(double coeff);
    void setSouthWestCoefficient(double coeff);

signals:
    void modelChanged(int newModel);
    void heterogeneityChanged(int newHeterogeneity);
    void uniformCoefficientChanged(double newCoeff);
    void northEastCoefficientChanged(double newCoeff);
    void southWestCoefficientChanged(double newCoeff);

protected:

    static const QStringList m_modelNames;
    static const QStringList m_modelFullNames;
    static const QStringList m_heterogeneityNames;

    Model m_model;
    Heterogeneity m_heterogeneity;
    double m_uniformCoefficient;
    double m_northEastCoefficient;
    double m_southWestCoefficient;
};

#endif // SELECTION_H