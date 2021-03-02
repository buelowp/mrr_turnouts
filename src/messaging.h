/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2021  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MESSAGING_H
#define MESSAGING_H

#include <QtCore/QtCore>
#include <qmqtt.h>

class Messaging : public QMQTT::Client
{
    Q_OBJECT

public:
    explicit Messaging(const QHostAddress host, const quint16 port = 1883, QObject* parent = nullptr);
    virtual ~Messaging() {}
    
public slots:
    void onConnected();
    void onDisconnected();
    void onReceived(const QMQTT::Message& message);
    void onError(const QMQTT::ClientError error);

signals:
    void clientConnected();
    void messageReceived(QJsonDocument doc);
};

#endif // MESSAGING_H
