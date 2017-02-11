#ifndef AddNewMaskHelper_H
#define AddNewMaskHelper_H

#include "deps.h"

#include <QString>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>

#include "mgc_recognizer.h"
#include <QAbstractButton>

#include <QtMultimedia/QSound>

class MGC_Recognizer;

class AddNewMaskHelper : public QDialog
{
public:
    QString m_path;
    Mat m_templateMaskMat;
    QPoint m_parentPos;
    QString m_newMaskName;
    bool m_hasMaskAdded = false;

    AddNewMaskHelper(const MGC_Recognizer *elem);

    QVBoxLayout *verticalLayout;
    QLabel *templateMask;
    QLineEdit *newMaskNameLineEdit;
    QDialogButtonBox *buttonBox;
public slots:
    void onClick(QAbstractButton *button);
};

#endif // AddNewMaskHelper_H
