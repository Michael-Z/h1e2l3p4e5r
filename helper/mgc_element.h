#ifndef MGC_ELEMENT_H
#define MGC_ELEMENT_H

#include "deps.h"

#include <opencv2/opencv.hpp>
using namespace cv;

#include <QLabel>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>

#include <QObject>

class MGC_Element : public QObject
{
    Q_OBJECT    

public:
    QString m_id;
    QJsonObject m_settings;    
    QRect m_elementArea = QRect(0, 0, 0, 0);
    Mat m_elementMat;
    QLabel *m_elementWidget;
    bool m_isVisible = true;
    bool m_isAvailable = true;
    bool m_isCovered = false;
    int m_isCoveredCounter = 0;

    bool m_doDrawElementMat = false;

    MGC_Element *m_parent;
    QHash<QString, MGC_Element*> m_children;

    explicit MGC_Element(QString id, MGC_Element *parent = 0, const QJsonObject &settings = QJsonObject());
    virtual ~MGC_Element();

    virtual void processNextScreen();
    virtual bool processOnDown();
    virtual void processOnUp();

    virtual void render();
    virtual void show();
    virtual void hide();

    virtual bool refreshElementMat();    

    void showElementMat() const;
    void showMat(const Mat &mat, String winName = "win") const;

    void setBorder(QString width, QString style, QString color);

    static bool saveMatToFile(QString fileName, Mat &mat);

    static QPixmap convertMat2Pixmap(Mat &mat);
    static Rect convertQRect2Rect(QRect &area);
    static QRect convertQJsonArray2QRect(QJsonArray &arr);

signals:

public slots:

protected:    

private:

};

#endif // MGC_ELEMENT_H
