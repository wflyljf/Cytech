#ifndef WINDOWDOCK_H
#define WINDOWDOCK_H

#include <QFrame>

class WindowDock : public QFrame
{
    Q_OBJECT
public:
    explicit WindowDock(const QString &c, QWidget *parent);

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    void setCustomSizeHint(const QSize &size);

signals:

public slots:
    void changeSizeHints();

protected:
    void paintEvent(QPaintEvent *);
    QString window;
    QSize szHint, minSzHint;

};

#endif // WINDOWDOCK_H
