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

#include "messaging.h"

Messaging::Messaging(const QHostAddress host, const quint16 port, QObject* parent) : QMQTT::Client(host, port, parent)
{
    connect(this, &Messaging::connected, this, &Messaging::onConnected);
    connect(this, &Messaging::disconnected, this, &Messaging::onDisconnected);
    connect(this, &Messaging::received, this, &Messaging::onReceived);
    connect(this, &Messaging::error, this, &Messaging::onError);
    
    setAutoReconnect(true);
    setAutoReconnectInterval(5000);
}

void Messaging::onConnected()
{
    subscribe("mrr/turnout/#");
    emit clientConnected();
}

void Messaging::onDisconnected()
{
    qWarning() << __PRETTY_FUNCTION__ << ": disconnected for some reason";
}

void Messaging::onError(const QMQTT::ClientError error)
{
    qWarning() << __PRETTY_FUNCTION__ << ": mqtt error" << error;
}

void Messaging::onReceived(const QMQTT::Message& message)
{
    if (message.topic() == "mrr/turnout/dispatch") {
        QJsonDocument doc = QJsonDocument::fromJson(message.payload());
        if (doc.isObject()) {
            emit messageReceived(doc);
        }
    }
    if (message.topic() == "mrr/turnout/create") {
        QJsonDocument doc = QJsonDocument::fromJson(message.payload());
        if (doc.isObject()) {
            emit messageReceived(doc);
        }
    }
}
