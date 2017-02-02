#include "AddNewMaskHelper.h"

AddNewMaskHelper::AddNewMaskHelper(QHash<QString, int> masksDifferencies,
                                   QString path, Mask_t &templateMaskMat, QPoint parentPos)
    : m_path(path), m_templateMaskMat(templateMaskMat), m_parentPos(parentPos)
{    
    this->setWindowTitle("adding new mask");

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    for (QString curMaskName: masksDifferencies.keys()) {
        QLabel *maskLabel = new QLabel(this);
        maskLabel->setTextFormat(Qt::RichText);
        QString labelText = "<img src=\"" + m_path + curMaskName + ".bmp\">";
        labelText += QString(" - ") + curMaskName + " - " + QString::number(masksDifferencies[curMaskName]);
        maskLabel->setText(labelText);
        maskLabel->adjustSize();
        verticalLayout->addWidget(maskLabel);

        QFrame *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        verticalLayout->addWidget(line);
    }

    templateMask = new QLabel(this);
    templateMask->setPixmap(MGC_Element::convertMat2Pixmap(m_templateMaskMat));
    verticalLayout->addWidget(templateMask);

    newMaskNameLineEdit = new QLineEdit(this);
    verticalLayout->addWidget(newMaskNameLineEdit);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setStandardButtons(QDialogButtonBox::Close|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::clicked, this, &AddNewMaskHelper::onClick);
    verticalLayout->addWidget(buttonBox);

//    QPushButton *b = buttonBox->button(QDialogButtonBox::Cancel);
//    b->setAutoDefault(true);
//    b->setDefault(true);

    this->adjustSize();
    this->move(m_parentPos);    
}

void AddNewMaskHelper::onClick(QAbstractButton* button)
{
    m_newMaskName = newMaskNameLineEdit->text();
    if (buttonBox->standardButton(button) == QDialogButtonBox::Close) {
        QApplication::exit(0);
    } else if (buttonBox->standardButton(button) == QDialogButtonBox::Cancel || m_newMaskName == "") {
        m_hasMaskAdded = false;
    } else if(buttonBox->standardButton(button) == QDialogButtonBox::Ok) {
        QString fullPath = m_path + m_newMaskName + ".bmp";
        MGC_Recognizer::saveMatToFile(fullPath, m_templateMaskMat);
        m_hasMaskAdded = true;
    } else {
        throw "WHY?";
    }
    this->close();
}
