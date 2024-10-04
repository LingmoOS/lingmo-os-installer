#ifndef QUICKINSATLLERPROCESS_H
#define QUICKINSATLLERPROCESS_H

#include <QObject>

namespace LMInstaller {


class QuickInsatllerProcess : public QObject
{
    Q_OBJECT
public:
    explicit QuickInsatllerProcess(QObject *parent = nullptr);

signals:
    void signalProcessStatus(int exitCode);

public slots:
    void exec();

public:


};
}
#endif // QUICKINSATLLERPROCESS_H
