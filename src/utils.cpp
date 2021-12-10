/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2011-2018 by Luca Bellonda and individual contributors  *
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

#include <QtGui>
#include <QXmlInputSource>
#include "xmlEdit.h"
#include "xmleditglobals.h"
#include "regola.h"
#include "utils.h"
#include "qxmleditconfig.h"

#define BATCH_ON_NOZERO    (0x55)

const int Utils::ReasonableIterationCount = 100 ;
bool Utils::isUnitTest = false ;
int Utils::silenceMessages1 = false ;
bool Utils::filler1 = false ; // filler to avoid memory corruption if possible. Used as a double security with a # def.
bool Utils::filler2 = false ;
int Utils::silenceMessages2 = false ;
const QString Utils::URIDropType("text/uri-list");

void Utils::setBatch(const bool asBatch)
{
    if(asBatch) {
        QXmlEditGlobals::batchMode = BATCH_ON_NOZERO ;
        silenceMessages1 = BATCH_ON_NOZERO;
        silenceMessages2 = BATCH_ON_NOZERO;
    } else {
        QXmlEditGlobals::batchMode = 0 ;
        silenceMessages1 = 0;
        silenceMessages2 = 0;
    }
}

bool Utils::isSilenceMode()
{
    return
        (BATCH_ON_NOZERO == QXmlEditGlobals::batchMode)
        && (BATCH_ON_NOZERO == silenceMessages1)
        && (BATCH_ON_NOZERO == silenceMessages2)
        && !filler1  && !filler2 ;
}

void Utils::error(const QString & message)
{
    Utils::error(NULL, message) ;
}

void Utils::error(QWidget *parent, const QString & message)
{
    qWarning("%s", message.toLatin1().data());
    if(isSilenceMode()) {
        return ;
    }
    QMessageBox::critical(parent, QXmlEditGlobals::appTitle(), message) ;
}

void Utils::warning(const QString & message)
{
    Utils::warning(NULL, message) ;
}

void Utils::warning(QWidget *parent, const QString & message)
{
    qWarning("%s", message.toLatin1().data());
    if(isSilenceMode()) {
        return ;
    }
    QMessageBox::warning(parent, QXmlEditGlobals::appTitle(), message) ;
}

void Utils::message(const QString & message)
{
#ifdef Q_DEBUG
    qDebug("%s", message.toLatin1().data());
#endif
    Utils::message(NULL, message);
}

void Utils::message(QWidget *parent, const QString & message)
{
    if(isSilenceMode()) {
        return ;
    }
    QMessageBox::information(parent, QXmlEditGlobals::appTitle(), message) ;
}

bool Utils::askYN(QWidget *parent, const QString & message)
{
    if(isSilenceMode()) {
        return false;
    }
    if(QMessageBox::Yes == QMessageBox::question(parent, QXmlEditGlobals::appTitle(), message, QMessageBox::Yes | QMessageBox::No)) {
        return true ;
    }
    return false;
}

bool Utils::askYN(const QString & message)
{
    return askYN(NULL, message);
}

void Utils::errorNoSel(QWidget *parent)
{
    error(parent, errorNoSelString());
}

QString Utils::errorNoSelString()
{
    return tr("No item selected");
}

void Utils::errorOutOfMem(QWidget *parent)
{
    error(parent, msgOutOfMem());
}

QString Utils::msgOutOfMem()
{
    return tr("Not enough memory to complete the operation");
}

void Utils::errorReadingUserSettings()
{
    error(NULL, tr("error reading user settings"));
}

void Utils::errorSavingUserSettings()
{
    error(NULL, tr("error saving user settings"));
}

void Utils::showWaitCursor()
{
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
}

void Utils::restoreCursor()
{
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}


void Utils::selectComboValue(QComboBox *combo, const int value)
{
    int items = combo->count();
    for(int itemIndex = 0 ; itemIndex < items ; itemIndex++) {
        if(combo->itemData(itemIndex).toInt() == value) {
            combo->setCurrentIndex(itemIndex);
            return;
        }
    }
}

void Utils::selectComboText(QComboBox *combo, const QString &text)
{
    int items = combo->count();
    for(int itemIndex = 0 ; itemIndex < items ; itemIndex++) {
        if(combo->itemText(itemIndex) == text) {
            combo->setCurrentIndex(itemIndex);
            return;
        }
    }
}

int Utils::comboSelectedCodeAsInt(QComboBox *combo, const int defaultValue)
{
    int itemIndex = combo->currentIndex();
    if(itemIndex >= 0) {
        int returnValue = combo->itemData(itemIndex).toInt() ;
        return returnValue ;
    }
    return defaultValue ;
}


void Utils::setupComboEncoding(QComboBox *combo)
{
    combo->clear();
    QList<int> mibsCodec = QTextCodec::availableMibs();
    QMap<QString, QTextCodec*> sortMap;

    foreach(int mib, mibsCodec) {
        QTextCodec *codec = QTextCodec::codecForMib(mib) ;
        sortMap.insert(codec->name().toUpper(), codec);
    }
    foreach(QString key, sortMap.keys()) {
        QTextCodec *codec = sortMap[key] ;
        combo->addItem(codec->name(), codec->mibEnum());
    }
}

void Utils::setupComboEncoding(QComboBox *combo, const bool allowEmpty, const QString &currentSelection)
{
    combo->clear();
    QList<int> mibsCodec = QTextCodec::availableMibs();
    QMap<QString, QTextCodec*> sortMap;

    foreach(int mib, mibsCodec) {
        QTextCodec *codec = QTextCodec::codecForMib(mib) ;
        sortMap.insert(codec->name().toUpper(), codec);
    }
    if(allowEmpty) {
        combo->addItem(tr("<Default value>"), -1);
        if(currentSelection.isEmpty()) {
            combo->setCurrentIndex(combo->count() - 1);
        }
    }
    foreach(QString key, sortMap.keys()) {
        QTextCodec *codec = sortMap[key] ;
        combo->addItem(codec->name(), codec->mibEnum());
        if(currentSelection == codec->name()) {
            combo->setCurrentIndex(combo->count() - 1);
        }
    }
}


bool Utils::decodeBoolean(const QString &str)
{
    bool isOk ;
    if(0 == str.compare("false", Qt::CaseInsensitive)) {
        return false;
    }
    if(0 == str.compare("true", Qt::CaseInsensitive)) {
        return true;
    }
    int val = str.toInt(&isOk, 10);
    if(!isOk) {
        return false;
    }
    if(val != 0) {
        return true;
    }
    return false;
}

int Utils::decodeInt(const QString &str, const int defaultVale)
{
    bool isOk ;
    int val = str.toInt(&isOk, 10);
    if(!isOk) {
        return defaultVale ;
    }
    return val ;
}

//-------------------------------------------------------

QString Utils::fromBase64Xml(const QString &text)
{
    QByteArray array(text.toLatin1());
    QByteArray array2 = QByteArray::fromBase64(array);
    // safe string
    QXmlInputSource xmlInputSource;
    xmlInputSource.setData(array2);
    QString strText = xmlInputSource.data();
    return strText;
}

QString Utils::fromBase64(const QString &text)
{
    QByteArray array(text.toLatin1());
    QByteArray array2 = QByteArray::fromBase64(array);
    // safe string
    QString strText = QString::fromUtf8(array2.data(), array2.length());
    return strText;
}

QString Utils::toBase64(const QString &text)
{
    QByteArray array;
    array.append(text.toUtf8());
    QByteArray converted = array.toBase64();
    QString strBase64 = converted.data();
    return strBase64 ;
}

//-------------------------------------------------------

void Utils::errorQtVersion(QWidget *parent)
{
    Utils::error(parent, tr("This feature is supported only if compiled with QT version specified in the manual."));
}

void Utils::errorFilePathInvalid(QWidget *parent)
{
    Utils::error(parent, tr("The file name is empty."));
}

void Utils::errorAccessingFile(QWidget *parent)
{
    Utils::error(parent, tr("Error accessing the file."));
}

//-------------------------------------------------------

QString Utils::getFileFilterForOpenFile()
{
    return tr("XML files (*.xml *.xsd *.xsl *.xslt *.fo *.scxml);;XML Schema files (*.xsd);;Style sheets files (*.xsl *.xslt);;SCXML files (*.scxml);;All files (*)");
}

QString Utils::getFileFilterForJavaSourceOpenFile()
{
    return tr("Java/Javascript (*.java *.js *.as *.txt);;All files (*)");
}

QString Utils::getFileFilterForCSV()
{
    return tr("CSV files (*.csv);;All files (*)");
}

QString Utils::getFileFilterForCSVOrText()
{
    return tr("CSV/TXT files (*.csv *.txt);;All files (*)");
}

//-------------------------------------------------------

void Utils::appendTextNode(QDomDocument &document, QDomElement &parent, const QString &tag, const QString &text)
{
    QDomElement nameNode = document.createElement(tag);
    QDomText textNode = document.createTextNode(text);
    nameNode.appendChild(textNode);
    parent.appendChild(nameNode);
}

QDomElement Utils::appendNode(QDomDocument &document, QDomElement &parent, const QString &tag)
{
    QDomElement nameNode = document.createElement(tag);
    parent.appendChild(nameNode);
    return nameNode ;
}

//-------------------------------------------------------

QStringList Utils::readUTF8FileStringList(const QString &filePath, bool& error)
{
    error = false;
    QStringList result;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = true;
        return result;
    }

    QTextStream inStream(&file);
    inStream.setCodec(QTextCodec::codecForName("UTF-8"));
    while(!inStream.atEnd()) {
        QString line = inStream.readLine();
        result.append(line);
    }
    if(file.error() != QFile::NoError) {
        error = true ;
    }
    file.close();
    return result;
}

QString Utils::readUTF8FileString(const QString &filePath, bool& error)
{
    error = false;
    QString result;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = true;
        return result;
    }

    QTextStream inStream(&file);
    inStream.setCodec(QTextCodec::codecForName("UTF-8"));
    while(!inStream.atEnd()) {
        QString line = inStream.readLine();
        result.append(line);
        result.append("\n");
    }
    if(file.error() != QFile::NoError) {
        error = true ;
    }
    file.close();
    return result;
}

//-----------------------------------------------------------------

bool Utils::writeXDocumentToFile(QDomDocument &document, const QString &filePath, const bool isOverWrite)
{
    QFile file(filePath);
    if(!isOverWrite && file.exists()) {
        Utils::error(tr("File already exists"));
        return false;
    }
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        Utils::error(tr("Error writing data"));
        return false ;
    }
    QTextStream streamOut(&file);
    streamOut.setCodec("UTF-8");
    streamOut << document.toString(4);
    streamOut.flush();
    file.close();
    return (file.error() == QFile::NoError);
}

//-----------------------------------------------------------------

bool Utils::checkNsPrefix(const QString &prefix, const bool emptyIsLegal)
{
    bool emptyTest = (prefix.isEmpty() && emptyIsLegal) || !prefix.isEmpty();
    if((emptyTest && !Utils::checkXMLName(prefix)) || (prefix.indexOf(":") >= 0)) {
        return false;
    }
    return true ;
}
//-----------------------------------------------------------------

// reference http://www.w3.org/XML/xml-V10-4e-errata#E09
//1-An Nmtoken (name token) is any mixture of name characters.
//2-A Name is an Nmtoken with a restricted set of initial characters

bool Utils::checkXMLName(const QString &theTag)
{
    if(theTag.isEmpty()) {
        return false ;
    }

    bool isFirst = true;
    const QChar *data = theTag.data();
    // Name ::= NameStartChar (NameChar)*
    while(!data->isNull()) {
        ushort code = data->unicode() ;
        // NameStartChar

        //":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF]
        // [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]

        if((':' == code)
                || ((code >= 'A') && (code <= 'Z'))
                || ((code >= 'a') && (code <= 'z'))
                || ('_' == code)
                || ((code >= 0xC0) && (code <= 0xD6))
                || ((code >= 0xD8) && (code <= 0xF6))
                || ((code >= 0xF8) && (code <= 0x2FF))
                || ((code >= 0x370) && (code <= 0x37D))
                || ((code >= 0x37F) && (code <= 0x1FFF))
                || ((code >= 0x200C) && (code <= 0x200D))
                || ((code >= 0x2070) && (code <= 0x218F))
                || ((code >= 0x2C00) && (code <= 0x2FEF))
                || ((code >= 0x3001) && (code <= 0xD7FF))
                || ((code >= 0xF900) && (code <= 0xFDCF))
                || ((code >= 0xFDF0) && (code <= 0xFFFD))     //range #x10000-#xEFFFF excluded
          ) {
            if(isFirst) {
                isFirst = false ;
            }
        } else {
            if(isFirst) {
                return false;
            }
            // NameChar
            //| "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
            if(!(('-' == code)
                    || ((code >= '0') && (code <= '9'))
                    || ('.' == code)
                    || (0xB7 == code)
                    || ((code >= 0x0300) && (code <= 0x036F))
                    || ((code >= 0x203F) && (code <= 0x2040))
                )) {
                return false ;
            }
        }
        ++data;
    }
    return true ;
}

bool Utils::canStartXmlName(const QChar &ch)
{
    ushort code = ch.unicode() ;
    // NameStartChar

    //":" | [A-Z] | "_" | [a-z] | [#xC0-#xD6] | [#xD8-#xF6] | [#xF8-#x2FF] | [#x370-#x37D] | [#x37F-#x1FFF]
    // [#x200C-#x200D] | [#x2070-#x218F] | [#x2C00-#x2FEF] | [#x3001-#xD7FF] | [#xF900-#xFDCF] | [#xFDF0-#xFFFD] | [#x10000-#xEFFFF]

    if((':' == code)
            || ((code >= 'A') && (code <= 'Z'))
            || ((code >= 'a') && (code <= 'z'))
            || ('_' == code)
            || ((code >= 0xC0) && (code <= 0xD6))
            || ((code >= 0xD8) && (code <= 0xF6))
            || ((code >= 0xF8) && (code <= 0x2FF))
            || ((code >= 0x370) && (code <= 0x37D))
            || ((code >= 0x37F) && (code <= 0x1FFF))
            || ((code >= 0x200C) && (code <= 0x200D))
            || ((code >= 0x2070) && (code <= 0x218F))
            || ((code >= 0x2C00) && (code <= 0x2FEF))
            || ((code >= 0x3001) && (code <= 0xD7FF))
            || ((code >= 0xF900) && (code <= 0xFDCF))
            || ((code >= 0xFDF0) && (code <= 0xFFFD))     //range #x10000-#xEFFFF excluded
      ) {
        return true;
    }
    return false;
}

bool Utils::canFollowXmlStartName(const QChar &ch)
{
    if(canStartXmlName(ch)) {
        return true;
    }
    ushort code = ch.unicode() ;
    // NameChar
    //| "-" | "." | [0-9] | #xB7 | [#x0300-#x036F] | [#x203F-#x2040]
    if(('-' == code)
            || ((code >= '0') && (code <= '9'))
            || ('.' == code)
            || (0xB7 == code)
            || ((code >= 0x0300) && (code <= 0x036F))
            || ((code >= 0x203F) && (code <= 0x2040))
      ) {
        return true ;
    }
    return false;
}

//--------------------

void Utils::deleteQWIList(QList<QTreeWidgetItem *> &list)
{
    foreach(QTreeWidgetItem * item, list) {
        delete item ;
    }
}

/*
void Utils::todo(const QString &inputMessage)
{
    if(isUnitTest) {
        printf("TODO:%s\n", inputMessage.toLatin1().data());
    } else {
        error(QString("TODO : %1").arg(inputMessage));
    }
}
*/

#if defined(QXMLEDIT_VERSION_IS_SNAPSHOT) && !defined(QXMLEDIT_DISABLE_TODO)
void Utils::TODO_THIS_RELEASE(const QString & inputMessage)
{
    if(!inputMessage.isEmpty()) {
        if(isUnitTest) {
            //fprintf(stderr, "TODO_THIS_RELEASE:%s\n", inputMessage.toLatin1().data());
        } else {
            fprintf(stderr, "TODO_THIS_RELEASE:%s\n", inputMessage.toLatin1().data());
            //message(QString("TODO IN THIS RELEASE: %1").arg(inputMessage));
        }
    }
}

void Utils::TEST_ME(const QString & inputMessage)
{
    if(isUnitTest) {
        Q_UNUSED(inputMessage)
        //fprintf(stderr, "TEST_ME:%s\n", inputMessage.toLatin1().data());
    } else {
        //message(QString("TEST_ME: %1").arg(inputMessage));
    }
}

/**
  use this function to mark fixes until automated tests are written.
  Don't make releases until there are marks on the current version.
  */

void Utils::TODO_CHECK_FIX(const QString & fix)
{
    printf("CHECK FIX:%s\n", fix.toLatin1().data());
}

void Utils::TODO_ERROR(const QString &inputMessage)
{
    Utils::TODO_THIS_RELEASE(inputMessage);
    Utils::error(inputMessage);
}

#endif

void Utils::TODO_NEXT_RELEASE(const QString &/*inputMessage*/)
{
    // This body intentionally blank.
}

bool Utils::askYNRemapNamespaces(QWidget *parent)
{
    return askYN(parent, QObject::tr("The prefix is already used for other namespaces. If you want to perform the operation, you must allow me to rename the existing prefix."));
}

/**
  Returns the new file name with the newExtension as estension.
  If there is already an extension it is changed
  */
QString Utils::changeFileType(const QString &inputString, const QString &newExtension)
{
    int index = inputString.lastIndexOf(".");
    if(index >= 0) {
        QString newName = inputString.left(index);
        newName += newExtension;
        return newName;
    }
    return inputString + newExtension;
}

// W3C Normalizaed string
QString Utils::normalizeStringXML(const QString &value)
{
    // canonical substitutions
    QString work = value;
    work = work.replace('\t', ' ');
    work = work.replace('\n', ' ');
    work = work.replace('\r', ' ');
    work = work.trimmed();
    // collapse spaces
    QString result;
    bool prevSpace = false ;
    int len = work.length();
    for(int i = 0 ; i < len ; i ++) {
        QChar ch = work.at(i);
        if(ch == ' ') {
            if(prevSpace) {
                continue ; // whitespace collapsing
            } else {
                prevSpace = true ;
                result.append(ch);
            }
        } else {
            prevSpace = false ;
            result.append(ch);
        }
    }
    return result;

}


void Utils::loadComboTextArray(QStringList strings, QList<QComboBox*> &combos)
{
    int index = 0;
    foreach(QString str, strings) {
        if(index < combos.size()) {
            combos.at(index)->setEditText(str);
        } else {
            break;
        }
        index ++;
    }
}


void Utils::loadComboTextArrays(QComboBox *combo, const QString &selectedValue, const QStringList &texts, const QStringList &values)
{
    int index = 0;
    bool found = false ;
    foreach(QString str, texts) {
        QString value = values.at(index) ;
        combo->insertItem(index, str, value);
        if(!found && (value == selectedValue)) {
            combo->setCurrentIndex(index);
            found = true ;
        }
        index ++;
    }
}

void Utils::loadComboCodedArrays(QComboBox *combo, const int selectedValue, const QStringList &texts, const QList<int> &values)
{
    int index = 0;
    bool found = false ;
    foreach(QString str, texts) {
        int value = values.at(index) ;
        combo->insertItem(index, str, value);
        if(!found && (value == selectedValue)) {
            combo->setCurrentIndex(index);
            found = true ;
        }
        index ++;
    }
}

void Utils::loadButtonMenu(QToolButton *button, const QStringList &texts, const QStringList &values, QObject *target, const char *method)
{
    QMenu *menu = new QMenu(button);
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);
    int index = 0;
    foreach(QString str, texts) {
        QString value = values.at(index) ;
        QAction *action = new QAction(str, menu);
        menu->addAction(action);
        action->setData(value);
        if(NULL != target) {
            QObject::connect(action, SIGNAL(triggered()), target, method);
        }
        index ++;
    }
}

QString Utils::getSizeForPresentation(const qint64 size)
{
    if(size >= (1024 * 1024 * 1024)) {
        qint64 div = 1024 * 1024 ;
        div *= 1024 ;
        int userSize = size / (div) ;
        return QString(tr("%1 Gb").arg(userSize));
    }
    if(size >= 1024 * 1024) {
        int userSize = size / (1024 * 1024) ;
        return QString(tr("%1 Mb").arg(userSize));
    }
    if(size >= 1024) {
        int userSize = size / 1024 ;
        return QString(tr("%1 Kb").arg(userSize));
    }
    return QString(tr("%1").arg(size));
}


QString Utils::decodeXMLError(const QString &error)
{
    if(error == "no error occurred") {
        return tr("no error occurred");
    }
    if(error == "error triggered by consumer") {
        return tr("error triggered by consumer");
    }
    if(error ==  "unexpected end of file") {
        return tr("unexpected end of file");
    }
    if(error == "more than one document type definition") {
        return tr("more than one document type definition");
    }
    if(error == "tag mismatch") {
        return tr("tag mismatch");
    }
    if(error == "error occurred while parsing content") {
        return tr("error occurred while parsing content");
    }
    if(error == "unexpected character") {
        return tr("unexpected character");
    }
    if(error == "invalid name for processing instruction") {
        return tr("invalid name for processing instruction");
    }
    if(error == "version expected while reading the XML declaration") {
        return tr("version expected while reading the XML declaration");
    }
    if(error == "wrong value for standalone declaration") {
        return tr("wrong value for standalone declaration");
    }
    if(error ==  "encoding declaration or standalone declaration expected while reading the XML declaration") {
        return tr("encoding declaration or standalone declaration expected while reading the XML declaration");
    }
    if(error == "standalone declaration expected while reading the XML declaration") {
        return tr("standalone declaration expected while reading the XML declaration");
    }
    if(error == "error occurred while parsing document type definition") {
        return tr("error occurred while parsing document type definition");
    }
    if(error == "letter is expected") {
        return tr("letter is expected");
    }
    if(error == "error occurred while parsing comment") {
        return tr("error occurred while parsing comment");
    }
    if(error == "error occurred while parsing reference") {
        return tr("error occurred while parsing reference");
    }
    if(error == "internal general entity reference not allowed in DTD") {
        return tr("internal general entity reference not allowed in DTD");
    }
    if(error == "external parsed general entity reference not allowed in attribute value") {
        return tr("external parsed general entity reference not allowed in attribute value");
    }
    if(error == "external parsed general entity reference not allowed in DTD") {
        return tr("external parsed general entity reference not allowed in DTD");
    }
    if(error == "unparsed entity reference n wrong context") {
        return tr("unparsed entity reference in wrong context");
    }
    if(error == "recursive entities") {
        return tr("recursive entities");
    }
    if(error == "error in the text declaration of an external entity") {
        return tr("error in the text declaration of an external entity");
    }
    return error;
}

QString Utils::escapeHTML(const QString &msgInput)
{
    QString msg = msgInput ;
    msg = msg.replace("&", "&amp;");
    msg = msg.replace("<", "&lt;");
    msg = msg.replace(">", "&gt;");
    return msg;
}

QString Utils::tableCellHeader(const QString &text)
{
    QString result = QString("<TH>%1</TH>").arg(Utils::escapeHTML(text));
    return result;
}

QString Utils::tableCell(const QString &text, const bool alignRight)
{
    if(alignRight) {
        return QString("<TD align='right'>%1</TD>").arg(Utils::escapeHTML(text));
    } else {
        return QString("<TD>%1</TD>").arg(Utils::escapeHTML(text));
    }
}

void Utils::setBkColorToWidget(QWidget *widget, const QColor &color)
{
    QString colorString = "background-color: " + color.name();
    widget->setStyleSheet(colorString);
}

void Utils::setBkColorAndBorderToWidget(QWidget *widget, const QColor &color)
{
    QString colorString = "background-color: " + color.name();
    colorString += ";border-color: #000; border-style:solid" ;
    widget->setStyleSheet(colorString);
}

// reference: rfc4180
QString Utils::valueStringCSV(const QString &value, const bool isSkipCommaBefore)
{
    QString ret ;
    if(!isSkipCommaBefore) {
        ret += separatorStringCSV() ;
    }
    if(!value.isEmpty()) {
        QString data = value ;
        data = data.replace("\"", "\"\"");
        ret += "\"";
        ret += data ;
        ret += "\"";
    }
    return ret;
}

// reference: rfc4180
QString Utils::separatorStringCSV()
{
    QChar separator = ',';
    return QString(separator);
}


void Utils::dumpStr(const QString &i1, const QString &i2)
{
    printf("%s : len=%d\n", i1.toLatin1().data(), i2.length());
    printf("%s\n", i2.toLatin1().data());
    for(int i = 0 ; i < i2.length() ; i ++) {
        printf("%s,", QString::number(i2.at(i).toLatin1(), 16).toLatin1().data());
    }
    printf("\n");
    fflush(stdout);
}

void Utils::addMaximizeToDialog(QDialog *dialog)
{
    dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowMaximizeButtonHint | Qt::CustomizeWindowHint);
}

void Utils::enableAndShowWidget(QWidget *widget, const bool how)
{
    widget->setEnabled(how);
    widget->setVisible(how);
}

bool Utils::caseInsensitiveLessThanCompareFunction(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}

bool Utils::fileIsXSLT(Regola *regola)
{
    QString normFile = regola->fileName().toLower();
    if(normFile.endsWith(".xslt") || normFile.endsWith(".xsl")) {
        return true;
    }
    //check for xslt namespace presence
    if(regola->hasXSLTNamespace()) {
        return true ;
    }
    return false;
}

bool Utils::fileIsSCXML(Regola *regola)
{
    QString normFile = regola->fileName().toLower();
    if(normFile.endsWith(".scxml")) {
        return true;
    }
    //check for xslt namespace presence
    if(regola->hasSCXMLNamespace()) {
        return true ;
    }
    return false;
}

QAction* Utils::findAction(QMainWindow *window, const QString &actionName)
{
    QList<QMenu*> menus = window->menuBar()->findChildren<QMenu*>();
    return findAction(menus, actionName);
}

QAction* Utils::findAction(QList<QMenu*> menus, const QString &actionName)
{
    foreach(QMenu * menu, menus) {
        QList<QAction*> actions = menu->actions();
        foreach(QAction * action, actions) {
            QString aName = action->objectName();
            if(aName == actionName) {
                return action;
            }
        }
    }
    return NULL ;
}

QString Utils::escape(const QString &str)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    return str.toHtmlEscaped();
#else
    return Qt::escape(str);
#endif

}


QString Utils::fmtNumber(const bool isHex, const int number, const int fieldWidth, const bool usePrefix)
{
    int finalNumber = number ;//& mask ;
    if(fieldWidth == 2) {
        finalNumber &= 0x00FF;
    }
    if(isHex) {
        if(usePrefix) {
            return QString("0x%1").arg(finalNumber, fieldWidth, 16, QChar('0'));
        } else {
            return QString("%1").arg(finalNumber, fieldWidth, 16, QChar('0'));
        }
    } else {
        return QString("%1").arg(finalNumber, fieldWidth, 10);
    }
}

QString Utils::convertHtmlToPlainText(const QString &inValue)
{
    QTextDocument doc;
    doc.setHtml(inValue);
    return doc.toPlainText();
}

QString Utils::loadTextFile(QWidget *window, const QString &filePath, bool &isError, bool isAbort)
{
    QString result;
    isError = true ;
    isAbort = false ;
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qint64 fileSize = file.size();
        if(fileSize > InputTextFileSizeLimit) {
            if(!Utils::askYN(window, QObject::tr("Warning: the size of the file to import is %1. Do you want to continue?").arg(Utils::getSizeForPresentation(fileSize)))) {
                isAbort = true ;
                isError = false ;
            }
        }
        if(!isAbort) {
            result = file.readAll();
            if(file.error() == QFile::NoError) {
                isError = false ;
            }
        }
        file.close();
        if(isError) {
            Utils::error(window, QObject::tr("Error reading file."));
        }
    } else {
        Utils::error(window, QString(QObject::tr("Unable to load file.\nError code is '%1'")).arg(file.error()));
    }
    return result ;
}

bool Utils::isStartingWithDataImage(const QString &data)
{
    return data.startsWith("data:image");
}

QString Utils::formatTextForTooltipHtml(const QString &input)
{
    QString escaped = Utils::escapeHTML(input);
    escaped = escaped.replace(" ", "&nbsp;");
    escaped = escaped.replace("\n", "<br/>");
    return escaped ;
}

QString Utils::normalizeFilePath(const QString &inputPath)
{
    if(inputPath.isEmpty()) {
        return inputPath;
    }
    QFileInfo fileInfo(inputPath);
    QString absPath = fileInfo.absoluteFilePath();
    return absPath ;
}


QByteArray Utils::translateData(const QString &string, const QByteArray &encoding)
{
    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);
    QTextStream stream(&buffer);
    stream.setCodec(QTextCodec::codecForName(encoding));
    stream.setGenerateByteOrderMark(false);
    stream << string ;
    stream.flush();
    buffer.close();
    QByteArray data = buffer.data();
    return data ;
}

bool Utils::isAsciiCompatible(const QByteArray &encoding)
{
    QByteArray result = translateData("a", encoding);
    if((result.length() == 1) && (*result.data() == 'a')) {
        return true ;
    }
    return false;
}

bool Utils::isEncoding8bitNotASCII(const QString &encoding)
{
    QByteArray result = translateData("a", encoding.toLatin1());
    if((result.length() == 1) && (*result.data() != 'a')) {
        return true ;
    }
    return false;
}


bool Utils::is8BitEncodingHonoredForStreamWriter(const QString &encoding)
{
    bool result = true;
    QBuffer ioDevice;
    if(ioDevice.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter outputStream(&ioDevice);
        outputStream.setCodec(encoding.toLatin1().data());
        ioDevice.setTextModeEnabled(false);
        outputStream.writeStartDocument();
        outputStream.writeStartElement("root");
        outputStream.writeEndElement();
        outputStream.writeEndDocument();
        ioDevice.close();

        // look at the first data
        QTextCodec *codec = QTextCodec::codecForName(encoding.toLatin1());
        if(NULL != codec) {
            QTextEncoder *encoder = codec->makeEncoder(QTextCodec::IgnoreHeader);
            if(NULL != encoder) {
                QByteArray codedLt = encoder->fromUnicode("<");
                if(codedLt.length() == 1) {
                    QByteArray dataConverted = ioDevice.data();
                    if((dataConverted.length() > 0) && (dataConverted[0] != codedLt[0])) {
                        result = false ;
                    } // if check len
                } // test len
                delete encoder;
            } // if encoder
        } // if codec
    } // open device: pessimistic evaluation
    return result ;
}

void Utils::replaceWidget(QLayout *layout, QWidget *oldWidget, QWidget *newWidget, const bool setAlignment, const Qt::Alignment alignment, const bool preferredSize)
{
    if(preferredSize) {
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        newWidget->setSizePolicy(sizePolicy);
    }
    layout->removeWidget(oldWidget);
    oldWidget->setParent(NULL);
    oldWidget->deleteLater();
    layout->addWidget(newWidget);
    if(setAlignment) {
        layout->setAlignment(newWidget, alignment);
    }
    layout->update();
}

void Utils::updateEditableCombo(QComboBox *combo, const QStringList & values)
{
    QString currentTextValue = combo->currentText();
    //--------
    combo->clear();
    loadComboTextArrays(combo, currentTextValue, values, values);
    combo->setEditText(currentTextValue);
}

void Utils::updateNonEditableCombo(QComboBox *combo, const QStringList & values)
{
    QString currentTextValue = combo->currentText();
    combo->clear();
    loadComboTextArrays(combo, currentTextValue, values, values);
}

bool Utils::isRFC4288(const QString &name)
{
    QStringList values = name.split("/");
    if(values.size() != 2) {
        return false;
    }
    if(!isRegNameRFC4288(values[0])) {
        return false;
    }
    if(!isRegNameRFC4288(values[1])) {
        return false;
    }
    return true ;
}

bool Utils::isRegNameRFC4288(const QString &name)
{
    if((name.length() < 1) || (name.length() > 127)) {
        return false;
    }
    QRegExp re;
    re.setPattern("[0-9,a-z,A-Z,\\!,\\#,\\$,\\&,\\.,\\+,\\-,\\^,\\_]*");
    return re.exactMatch(name);
}

void Utils::setComboCurrentText(QComboBox *combo, const QString &text)
{
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
    combo->setCurrentText(text);
#else
    combo->setEditText(text);
    selectComboText(combo, text);
#endif
}

void Utils::copyPairsToHash(QHash<QString, QString> &hash, QList<QPair<QString, QString> > &pairs)
{
    QPair<QString, QString> pair;
    foreach(pair, pairs) {
        hash.insert(pair.first, pair.second);
    }
}

qint64 Utils::infoSizeAboutLocalDevice(QIODevice *device, const QString &fileName)
{
    bool found = false;
    quint64 size = -1 ;
    if(NULL != device) {
        if(device->isOpen()) {
            if(!device->isSequential()) {
                size = device->size();
                found = true;
            }
        }
    }
    if(!found) {
        QFileInfo info(fileName) ;
        if(info.isFile()) {
            size = info.size();
        }
    }
    return size ;
}

QString Utils::pushCurrentElementPath(const QString &current, const QString &name)
{
    return current + "/" + name ;
}

QString Utils::popCurrentElementPath(const QString &current)
{
    int index = current.lastIndexOf("/");
    if(index >= 0) {
        return current.left(index);
    }
    return current ;
}

bool Utils::writeStringToFile(const QString &file, const QString &dataString, const QString &encoding)
{
    QFile dataFile(file);
    return writeStringToFile(&dataFile, dataString, encoding);
}

bool Utils::writeStringToFile(QFile *dataFile, const QString &dataString, const QString &encoding)
{
    bool ok = false ;
    if(dataFile->open(QFile::WriteOnly)) {
        ok = true ;
        QTextStream streamOut(dataFile);
        streamOut.setCodec(QTextCodec::codecForName(encoding.toLatin1().data()));
        streamOut << dataString ;
        streamOut.flush();
        if(dataFile->error() != QFile::NoError) {
            ok = false;
        }
        dataFile->close();
    }
    return ok;
}

bool Utils::writeDataToFile(const QString &file, const QByteArray &dataIn)
{
    bool ok = false ;
    QFile data(file);
    if(data.open(QFile::WriteOnly)) {
        ok = true ;
        qint64 written = data.write(dataIn);
        data.flush();
        if(data.error() != QFile::NoError) {
            ok = false;
        }
        data.close();
        if(written != dataIn.length()) {
            ok = false;
        }
    }
    return ok;
}

QByteArray Utils::readBytesFromFile(const QString &file)
{
    QFile data(file);
    if(data.open(QFile::ReadOnly)) {
        QByteArray result = data.readAll();
        if(data.error() == QFile::NoError) {
            return result ;
        } else {
            Utils::error(QObject::tr("Error reading file: (%1) %2").arg(data.error()).arg(data.errorString()));
        }
        data.close();
    }
    return QByteArray() ;
}

QString Utils::askFileNameToOpen(QWidget* parent, const QString &startFolder)
{
    QString filePath = QFileDialog::getOpenFileName(parent, tr("Open File"),
                       QXmlEditData::sysFilePathForOperation(startFolder),
                       Utils::getFileFilterForOpenFile());
    return filePath ;
}

QString Utils::pathFromParent(Element* parent, const QString &tag)
{
    const QString basePath = (NULL != parent) ? parent->pathString() : "";
    const QString path = basePath + "/" + tag;
    return path;
}


QString Utils::truncateString(const QString &msgText, const int maxLength)
{
    if(msgText.length() < maxLength) {
        return msgText;
    }
    return msgText.left(maxLength) + "...";
}

QString Utils::truncateString(const QString &msgText)
{
    return truncateString(msgText, DEFAULT_TRUNCATE_STRING_LENGTH);
}
