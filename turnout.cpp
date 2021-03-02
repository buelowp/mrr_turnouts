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

#include "turnout.h"

Turnout::Turnout(int pin, QString name, Direction dir, QObject *parent) : m_direction(dir), m_pin(pin), m_name(name), QObject(parent)
{
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW);
    m_state = State::MAIN;
}

Turnout::Turnout(int pin, Turnout::Direction dir, QObject* parent) : QObject(parent), m_direction(dir), m_pin(pin)
{
    m_name = QString("%1").arg(m_pin);
    pinMode(m_pin, OUTPUT);
    digitalWrite(m_pin, LOW);
    m_state = State::MAIN;    
}

Turnout::~Turnout()
{
}

void Turnout::no()
{
    digitalWrite(m_pin, HIGH);
    QThread::sleep(3);
    emit stateChange(m_name);
}

void Turnout::nc()
{
    digitalWrite(m_pin, LOW);
    QThread::sleep(3);
    emit stateChange(m_name);
}

QString Turnout::direction()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Turnout::Direction>();  
    return metaEnum.valueToKey(m_direction);
}

QString Turnout::state()
{
    QMetaEnum metaEnum = QMetaEnum::fromType<Turnout::State>();   
    return metaEnum.valueToKey(m_state);    
}
