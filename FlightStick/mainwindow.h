#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Hidapi/hidapi.h"
#include <QMainWindow>
#include <QTimer>

#define VID 0x0480
#define PID 0x5110
#define MANUFACTURER_STRING "Rocket Scientist"
#define POLL_TIME_MS 2

namespace Ui{class MainWindow;}
class MainWindow : public QMainWindow{
    Q_OBJECT
    public:
        //MainWindow
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        //Timer
        void onPollTimerOverflowed();
        //UI
        void on_actionUse_triggered();
        void on_actionSetBacklight_triggered();
        void on_actionRefresh_triggered();

    private:
        //MainWindow
        Ui::MainWindow *ui;

        //App
        QTimer *poll_timer;
        bool connected;
        bool write_rq;
        QStringList serials;
        hid_device_info *hid_dev_info;
        hid_device *hid_dev;

        //Raw USB HID transmit/receive data
        /*
            -- Receive --
            Data: [X][Y][BUTTON_1][BUTTON_2][6_UNUSED_BITS][BRIGHTNESS]
            Bits: [XXXXXXXX][YYYYYYYY][BUTTON_1][BUTTON_2][0][0][0][0][0][0][BBBBBBBB]
        */
        uint8_t receive_buffer[4] = { 0, 0, 0b00000000, 0 };
        /*
            -- Transmit --
            Data: [REPORT_ID_BYTE_ALWAYS_0][BRIGHTNESS]
            Bits: [IIIIIIII][BBBBBBBB]
        */
        uint8_t transmit_buffer[2] = { 0, 100 };

        //Values
        int8_t joystick_x;
        int8_t joystick_y;
        bool button_1;
        bool button_2;
        uint8_t backlight;

        //Private functions
        void appReset();
};

#endif
