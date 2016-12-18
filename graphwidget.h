#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <QVector>
#include <QSharedMemory>

typedef int DataType;
const int repaintInterval = 10;

class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = 0);
    bool load();
    int frequency;
    int dataPos;
    QVector<DataType> data;
    inline DataType getPrevVal(int index);
    int periodSyncCount;

    void addValue(DataType v);

protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void timerEvent(QTimerEvent *);
private:
    QPixmap paintBuffer;
    void repaintBuffer();
    DataType maxVal;
    DataType minVal;
    int startOffset;
    int periodSyncCounter;
};


DataType GraphWidget::getPrevVal(int index)
{
    return data[(index>dataPos)?(data.size()+dataPos-index):(dataPos-index)];
}

#endif // GRAPHWIDGET_H
