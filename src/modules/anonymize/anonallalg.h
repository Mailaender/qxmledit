/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2014-2022 by Luca Bellonda and individual contributors  *
 *    as indicated in the AUTHORS file                                    *
 *  lbellonda _at_ gmail.com                                              *
 *                                                                        *
 * This library is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU Library General Public            *
 * License as published by the Free Software Foundation; either           *
 * version 2 of the License, or (at your option) any later version.       *
 *                                                                        *
 * This library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 * Library General Public License for more details.                       *
 *                                                                        *
 * You should have received a copy of the GNU Library General Public      *
 * License along with this library; if not, write to the                  *
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA  02110-1301  USA                                            *
 **************************************************************************/


#ifndef ANONALLALG_H
#define ANONALLALG_H

#include "libQXmlEdit_global.h"
#include "anonbase.h"
#include "modules/anonymize/algstat/anonalgvalue.h"

class LIBQXMLEDITSHARED_EXPORT AnonAllAlg : public AnonAlg
{
    bool _useLegacyAlgorithm ;
    QMap<QString, AnonStatAlgValue*> _anonStatData;
    AlgStatRandomProvider *_algStatRandomProvider;

    AnonStatAlgValue* valueFor(const QString &path);
    bool scanned();
public:
    AnonAllAlg(const bool parmAutodelete, AnonProducer *theProducer);
    virtual ~AnonAllAlg();
    virtual QString processText(AnonAlgStatContext &context, const QString &path, const QString &input) ;
    virtual void scan(AnonAlgStatContext &context, const QString &path, const QString &input) ;
    virtual bool needScan();
    bool isUseLegacy();
    void setUseLegacy(const bool value);
    void setAlgStatRandomProvider(AlgStatRandomProvider *newValue);
    virtual QString dumpAsString();
#ifdef  QXMLEDIT_TEST
    friend class TestAnonymize;
#endif
};

#endif // ANONALLALG_H
