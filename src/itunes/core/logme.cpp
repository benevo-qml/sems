/****************************************************************************************
** logme.cpp is part of iThunder
**
** Copyright 2013 Alireza Savand <alireza.savand@gmail.com>
**
** iThunder is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** iThunder is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************************/

#include "logme.h"
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

//#ifdef QT_NO_DEBUG
# include <iostream>
//#endif

LogMe::LogMe(QObject *parent) :
    QObject(parent)
{
    if (parent == 0) {
        return;
    }

    _className  = parent->metaObject()->className();

}

LogMe::LogMe(const QString &className, QObject *parent)
    : QObject(parent), _className(className)
{
}

void LogMe::debug(const QString &debugMessage)
{
    this->debugMessage("debug", debugMessage);
}

void LogMe::info(const QString &infoMessage)
{
    this->logMessage("info", infoMessage);
}

void LogMe::success(const QString &successMessage)
{
    this->logMessage("success", successMessage);
}

void LogMe::error(const QString &errorMessage)
{
    this->logMessage("error", errorMessage);
}

void LogMe::log(const QString &logMsg)
{
    this->logMessage("log", logMsg);
}

void LogMe::setClassName(const QString &className)
{
    _className = className;
}

void LogMe::debugMessage(const QString &logLevel, const QString &logMessage) const
{
    qDebug() << QString("%1::%2 %3 : %4").arg(_className,
                                            logLevel.toUpper(),
                                            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"),
                                            logMessage);
}

void LogMe::logMessage(const QString &logLevel, const QString &logMessage) const
{
    std::cout << printf("%s::%s %s : %s\n", _className.toStdString().data(),
                                          logLevel.toUpper().toStdString().data(),
                                          QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz").toStdString().data(),
                                          logMessage.toStdString().data());
}

