#ifndef MGC_RECOGNIZER_H
#define MGC_RECOGNIZER_H

#include "deps.h"

#include <QDir>

#include "mgc_element.h"
#include "AddNewMaskHelper.h"

typedef Mat Mask_t;
typedef QHash<QString, Mat> MaskSet_t;
typedef QHash<QString, MaskSet_t> MaskSets_t;

enum class DrawMode {ELEMENT_MAT, MASK, RECOGNIZED_TEXT};
enum class MisrecMode {IGNORE, SIMPLE_REQUESTING};

class MGC_Recognizer : public MGC_Element
{
public:
    QString m_recMask = "";
    QString m_recMaskLastValue = "";
    double m_resDiff;
    QHash<QString, int> m_resDiffs;
    QList<QPoint> m_foundPoints;

    DrawMode m_drawMode = DrawMode::ELEMENT_MAT;

    explicit MGC_Recognizer(QString id, MGC_Element *parent, const QJsonObject &settings);

    virtual void show() override;

    int compare2Masks(Mask_t &mat1, Mask_t &mat2) const;
    bool recognizeMask();
    bool recognizeMaskInSet();
    bool recognizeMaskInSet(QString &maskName);
    bool recognizeMaskInSets(QString &maskSetName);

    int findPoints(int matchAmount = 1, QString &maskName = QStringLiteral(""));

    void addMaskFromElementMat();
    void addMaskToMaskSetFromElementMat(QString &maskName);
    void addAndSaveMask(QString &folder);

//private:
public:
    int m_tolerance;
    Mask_t m_mask;
    MaskSet_t m_maskSet;
    MaskSets_t m_maskSets;

    MaskSet_t loadMaskSet(QString path) const;
    QString m_mask_path;
    QString m_maskSet_path;
    QString m_maskSets_path;

    MisrecMode m_misrecMode = MisrecMode::IGNORE;
    int m_nameCounter = 0;
signals:

public slots:

};

#endif // MGC_RECOGNIZER_H
