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

#include "rightturnout.h"

RightTurnout::RightTurnout(int pin, QString name) : Turnout(pin, name, Turnout::Direction::RIGHT)
{
}

RightTurnout::RightTurnout(int pin) : Turnout(pin, Turnout::Direction::RIGHT)
{
}

RightTurnout::~RightTurnout()
{
}

void RightTurnout::switchMain()
{
    m_state = State::MAIN;
    nc();
}

void RightTurnout::switchSiding()
{
    m_state = State::SIDING;
    no();
}

void RightTurnout::setState(Turnout::State state)
{
    if (state == State::MAIN)
        switchMain();
    else if (state == State::SIDING)
        switchSiding();
}
