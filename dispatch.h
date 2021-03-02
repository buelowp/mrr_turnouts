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

#ifndef DISPATCH_H
#define DISPATCH_H

#include <QtCore/QtCore>
#include "messaging.h"
#include "leftturnout.h"
#include "rightturnout.h"
#include "wye.h"
#include "turnout.h"

class Dispatch : public QObject
{
    Q_OBJECT

public:
    Dispatch(Messaging *client);
    ~Dispatch();
    
    void addTurnout(Turnout *turnout, bool persist = true);

public slots:
    void turnoutStateChange(QString name);
    void messageReceived(QJsonDocument doc);
    void onConnected();
    
private:
    void readTurnoutsFromDisk();
    
    QMap<QString, Turnout*> m_turnouts;
    Messaging *m_client;
};

#endif // DISPATCH_H
