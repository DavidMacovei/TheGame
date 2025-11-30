#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TheGame_Client.h"

class TheGame_Client : public QMainWindow
{
    Q_OBJECT

public:
    TheGame_Client(QWidget *parent = nullptr);
    ~TheGame_Client();

private:
    Ui::TheGame_ClientClass ui;
};

