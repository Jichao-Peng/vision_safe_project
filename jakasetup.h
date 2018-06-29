#ifndef JAKASETUP_H
#define JAKASETUP_H

#include <QDialog>
#include <QString>

namespace Ui {
class JakaSetup;
}

class JakaSetup : public QDialog
{
    Q_OBJECT

public:
    explicit JakaSetup(QWidget *parent = 0);
    ~JakaSetup();

    char ROIShape = 3;
    char ROIMode = 3;
    QString IP;
    int Port;

private slots:
    void on_Rect_clicked();

    void on_Circ_clicked();

    void on_Free_clicked();

    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::JakaSetup *ui;
};

#endif // JAKASETUP_H
