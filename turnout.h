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

#ifndef TURNOUT_H
#define TURNOUT_H

#include <QtCore/QtCore>

#include <wiringPi.h>

class Turnout : public QObject
{
    Q_OBJECT
public:
    enum Direction {
        LEFT,
        RIGHT,
        WYE,
    };
    Q_ENUM(Direction);
    
    enum State {
        MAIN,
        SIDING,
    };
    Q_ENUM(State)
    
    Turnout(int pin, QString name, Direction dir, QObject *parent = nullptr);
    Turnout(int pin, Direction dir, QObject *parent = nullptr);
    ~Turnout();
    
    void no();
    void nc();
    QString name() { return m_name; }
    QString direction();
    QString state();
    int pin() { return m_pin; }
    virtual bool isOnMain() { return m_state == State::MAIN; }
    virtual void setState(State state) = 0;

signals:
    void stateChange(QString name);
    
protected:
    Direction m_direction;
    int m_pin;
    QString m_name;
    State m_state;
};

#endif // TURNOUT_H
