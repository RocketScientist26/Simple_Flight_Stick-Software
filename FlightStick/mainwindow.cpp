#include "mainwindow.h"
#include "ui_mainwindow.h"

//MainWindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    //UI
    setFixedSize(366, 404);
    ui->setupUi(this);

    //Poll timer
    PollTimer = new QTimer(this);
    connect(PollTimer, SIGNAL(timeout()), this, SLOT(pollTimerOverflowed()));

    //HID
    hid_init();
    on_actionRefresh_triggered();
}
MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_actionUse_triggered(){
    if(!connected){
        QString serial = serials.at(ui->comboBox_Devices->currentIndex()).toLocal8Bit();
        hid_dev = hid_open(VID, PID, (const wchar_t *)&serial.data()[0]);
        if(hid_dev){
            if(hid_set_nonblocking(hid_dev, 1) == -1){
                appReset();
                return;
            }
            connected = true;

            //Enable UI elements here
            ui->comboBox_Devices->setEnabled(false);
            ui->pushButton_Refresh->setEnabled(false);
            ui->horizontalSlider_Backlight->setEnabled(true);
            ui->pushButton_Use->setCheckable(true);
            ui->pushButton_Use->setChecked(true);

            PollTimer->start(POLL_TIME_MS);
        }
    }else{
        appReset();
    }
}
void MainWindow::on_actionSetBacklight_triggered(){
    if((uint8_t)ui->horizontalSlider_Backlight->value() != backlight){
        write_rq = true;
    }
}
void MainWindow::on_actionRefresh_triggered(){
    ui->comboBox_Devices->clear();
    serials.clear();
    hid_dev_info = hid_enumerate(VID, PID);
    int i = 0;
    while(hid_dev_info){
        if(!QString::fromWCharArray(hid_dev_info->manufacturer_string).compare(MANUFACTURER_STRING)){
                ui->comboBox_Devices->addItem(QString::fromWCharArray(hid_dev_info->product_string).append(" - ").append(QString::fromWCharArray(hid_dev_info->serial_number)));
                serials.append(QString::fromWCharArray(hid_dev_info->serial_number));
                i++;
        }
        hid_dev_info = hid_dev_info->next;
    }
    if(i == 0){
        ui->comboBox_Devices->addItem("None");
        ui->comboBox_Devices->setEnabled(false);
        ui->pushButton_Use->setEnabled(false);
    }else{
        ui->comboBox_Devices->setEnabled(true);
        ui->pushButton_Use->setEnabled(true);
    }
}

void MainWindow::pollTimerOverflowed(){
    PollTimer->stop();
    if(connected){
        int res = hid_read(hid_dev, receive_buffer, sizeof(receive_buffer));
        if(res != sizeof(receive_buffer)){
            appReset();
        }else{
            joystick_x = *(int8_t *)&receive_buffer[0];
            joystick_y = *(int8_t *)&receive_buffer[1];
            button_1 = (bool)(receive_buffer[2] & 1);
            button_2 = (bool)((receive_buffer[2] >> 1) & 1);
            backlight = receive_buffer[3];

            ///Display on UI
            ui->label_X->setText(QString("X: ").append(QString::number(joystick_x)));
            ui->label_Y->setText(QString("Y: ").append(QString::number(joystick_y)));
            ui->widget_Indicator->setX((1.0f / 127.0f) * (float)joystick_x);
            ui->widget_Indicator->setY(-(1.0f / 127.0f) * (float)joystick_y);
            ui->pushButton_1->setChecked(button_1);
            ui->pushButton_1->setFlat(button_1);
            ui->pushButton_2->setChecked(button_2);
            ui->pushButton_2->setFlat(button_2);
            if(ui->horizontalSlider_Backlight->isSliderDown() == false){
                ui->horizontalSlider_Backlight->setValue(backlight);
            }

            if(write_rq){
                write_rq = false;
                transmit_buffer[1] = (uint8_t)ui->horizontalSlider_Backlight->value();
                if(hid_write(hid_dev, transmit_buffer, sizeof(transmit_buffer)) == -1){
                    appReset();
                    return;
                }
            }
            PollTimer->start(POLL_TIME_MS);
        }
    }
}

void MainWindow::appReset(){
    //Timers
    PollTimer->stop();

    hid_close(hid_dev);

    //App data
    connected = false;
    write_rq = false;

    //Data
    joystick_x = 0;
    joystick_y = 0;
    button_1 = false;
    button_2 = false;
    backlight = 0;

    //UI
    ui->comboBox_Devices->setEnabled(true);
    ui->pushButton_Refresh->setEnabled(true);
    ui->horizontalSlider_Backlight->setEnabled(false);
    ui->pushButton_Use->setChecked(false);
    ui->pushButton_Use->setCheckable(false);
    ui->label_X->setText("X: 0");
    ui->label_Y->setText("Y: 0");
    ui->widget_Indicator->setX(0);
    ui->widget_Indicator->setY(0);
    ui->horizontalSlider_Backlight->setValue(0);
    ui->pushButton_1->setChecked(false);
    ui->pushButton_1->setFlat(false);
    ui->pushButton_2->setChecked(false);
    ui->pushButton_2->setFlat(false);
}
