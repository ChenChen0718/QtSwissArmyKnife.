﻿/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>

#include "SAKModbusCommonClientSection.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"

SAKModbusClientControllerSerialPort::SAKModbusClientControllerSerialPort(QWidget *parent)
    :SAKModbusClientController(parent)
{
    init();
    mSerialPortSection = new SAKModbusCommonSerialPortSection(this);
    appendSection(mSerialPortSection);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    mClient = qobject_cast<QModbusRtuSerialMaster*>(device());
#else
    mClient = qobject_cast<QModbusRtuSerialClient*>(device());
#endif
    connect(mClient, &QModbusClient::stateChanged, this, [=](){
        bool isUnconnected = (mClient->state() == QModbusDevice::UnconnectedState);
        mSerialPortSection->setEnabled(isUnconnected);
        mClientSection->setUiEnable(isUnconnected);
    });
}

void SAKModbusClientControllerSerialPort::open()
{
    mSerialPortSection->initModbusDeviceParamerers(mClient);
    mClientSection->initModbusClientParameters(mClient);
    // The function is asynchronous
    mClient->connectDevice();
}

QModbusDevice *SAKModbusClientControllerSerialPort::initModbusDevice()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    auto dev = new QModbusRtuSerialMaster;
#else
    auto dev = new QModbusRtuSerialClient;
#endif
    return dev;
}
