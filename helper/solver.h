#ifndef SOLVER_H
#define SOLVER_H

#include <QDebug>
#include <QProcess>
#include <QObject>

class Solver : public QObject
{
    Q_OBJECT
public:
    explicit Solver(QObject *parent = 0);

signals:

public slots:
};

#endif // SOLVER_H
