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

#include "dispatch.h"

Dispatch::Dispatch(Messaging *client) : m_client(client)
{    
    connect(client, &Messaging::messageReceived, this, &Dispatch::messageReceived);
    connect(client, &Messaging::clientConnected, this, &Dispatch::onConnected);
}

Dispatch::~Dispatch()
{
}

void Dispatch::onConnected()
{
    qDebug() << __PRETTY_FUNCTION__ << ": MQTT connnected";
    readTurnoutsFromDisk();
}

void Dispatch::readTurnoutsFromDisk()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "mrr", "turnouts");
    
    foreach (const QString &group, settings.childGroups()) {
        settings.beginGroup(group);
        QString direction;
        QString name;
        int pin;
        foreach (const QString &key, settings.childKeys()) {
            if (key == "direction") {
                direction = settings.value(key).toString();
            }
            if (key == "name") {
                name = settings.value(key).toString();
            }
            if (key == "pin") {
                pin = settings.value(key).toInt();
            }
        }

        if (direction == "LEFT") {
            LeftTurnout *turnout = new LeftTurnout(pin, name);
            addTurnout(turnout, false);
        }
        if (direction == "RIGHT") {
            RightTurnout *turnout = new RightTurnout(pin, name);
            addTurnout(turnout, false);
        }
        if (direction == "WYE") {
            Wye *turnout = new Wye(pin, name);
            addTurnout(turnout, false);
        }
        settings.endGroup();
    }
}

void Dispatch::addTurnout(Turnout* turnout, bool persist)
{
    QJsonObject parent;
    QJsonObject object;
    QJsonDocument doc;
    QMQTT::Message message;
    
    if (!m_turnouts.contains(turnout->name())) {
        m_turnouts.insert(turnout->name(), turnout);
        object["name"] = turnout->name();
        object["direction"] = turnout->direction();
        object["state"] = turnout->state();
        object["gpio"] = turnout->pin();
        parent.insert("turnout", object);
        doc.setObject(parent);
        message.setTopic("mrr/turnout/new");
        message.setPayload(doc.toJson());
        message.setQos(1);
        m_client->publish(message);
        
        if (persist) {
            QSettings settings(QSettings::IniFormat, QSettings::UserScope, "mrr", "turnouts");
            settings.beginGroup(turnout->name());
            settings.setValue("name", turnout->name());
            settings.setValue("pin", turnout->pin());
            settings.setValue("direction", turnout->direction());
            settings.endGroup();
        }
        connect(turnout, SIGNAL(stateChange(QString)), this, SLOT(turnoutStateChange(QString)));
        qDebug() << __PRETTY_FUNCTION__ << ": Added turnout" << turnout->name() << "on pin" << turnout->pin() << "turning" << turnout->direction();
    }
}

void Dispatch::turnoutStateChange(QString name)
{
    if (m_turnouts.contains(name)) {
        Turnout *turnout = m_turnouts[name];
        QJsonObject parent;
        QJsonObject object;
        QJsonDocument doc;
        QMQTT::Message message;
        
        object["name"] = name;
        object["direction"] = turnout->direction();
        object["state"] = turnout->state();
        parent.insert("turnout", object);
        message.setTopic("mrr/turnout/statechange");
        message.setPayload(doc.toJson());
        message.setQos(1);
        m_client->publish(message);
    }
    else {
        qWarning() << __PRETTY_FUNCTION__ << ": Unable to find turnout named" << name << "in the turnout map";
    }
}

void Dispatch::messageReceived(QJsonDocument doc)
{
    QJsonObject obj = doc.object();
    if (obj.contains("statechange")) {
        QJsonObject device = obj["statechange"].toObject();
        if (m_turnouts.contains(device["name"].toString())) {
            qDebug() << __PRETTY_FUNCTION__ << ": Found turnout with name" << device["name"].toString();
            Turnout *turnout = m_turnouts[device["name"].toString()];
            if (device["state"].toString() == "main") {
                turnout->setState(Turnout::State::MAIN);
            }
            if (device["state"].toString() == "siding") {
                turnout->setState(Turnout::State::SIDING);
            }
        }
    }
    if (obj.contains("create")) {
        QJsonObject device = obj["create"].toObject();
        if (device.contains("name") && device.contains("pin") && device.contains("direction")) {
            if (device["direction"] == "left") {
                LeftTurnout *turnout = new LeftTurnout(device["pin"].toInt(), device["name"].toString());
                addTurnout(turnout);
            }
            if (device["direction"] == "right") {
                RightTurnout *turnout = new RightTurnout(device["pin"].toInt(), device["name"].toString());
                addTurnout(turnout);
            }
            if (device["direction"] == "left") {
                Wye *turnout = new Wye(device["pin"].toInt(), device["name"].toString());
                addTurnout(turnout);
            }
        }
    }
    if (obj.contains("delete")) {
        QSettings settings("mrr", "turnouts");
        QJsonObject device = obj["delete"].toObject();
        QString settingsName = QString("%1").arg(device["name"].toString());
        settings.remove(settingsName);
        qDebug() << __PRETTY_FUNCTION__ << "removing" << m_turnouts.remove(device["name"].toString()) << "turnouts named" << device["name"].toString();
    }
}
