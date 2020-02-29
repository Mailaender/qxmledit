/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2011-2020 by Luca Bellonda and individual contributors  *
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

#include "testsplit.h"
#include "comparexml.h"
#include "utils.h"
#include <QDesktopServices>
#include <QDir>
#include <QDateTime>
#include "modules/services/systemservices.h"
#include "qxmleditconfig.h"

#if 1
#define INPUT_FILE  "../test/data/splitter_test.xml"
#define NUM_FILES_PER_FOLDER    2
#define SPLIT_PATH "/site/items/item"
#else
#define INPUT_FILE  QDir::homePath()+QString("/Desktop/test xml/standard.xml")
#define SPLIT_PATH "/site/regions/africa/item"
#define NUM_FILES_PER_FOLDER    10
#endif

#define SPLIT_PATH_FILTER_TEXT  "/a/b"

#define SPLITTED_FILES_NUMBER   (10)
#define SPLIT_FILE_0    "../test/data/split/splitter_res0.xml"
#define SPLIT_FILE_1    "../test/data/split/splitter_res1.xml"
#define SPLIT_FILE_2    "../test/data/split/splitter_res2.xml"
#define SPLIT_FILE_3    "../test/data/split/splitter_res3.xml"
#define SPLIT_FILE_4    "../test/data/split/splitter_res4.xml"
#define SPLIT_FILE_5    "../test/data/split/splitter_res5.xml"
#define SPLIT_FILE_6    "../test/data/split/splitter_res6.xml"
#define SPLIT_FILE_7    "../test/data/split/splitter_res7.xml"
#define SPLIT_FILE_8    "../test/data/split/splitter_res8.xml"
#define SPLIT_FILE_9    "../test/data/split/splitter_res9.xml"
#define INPUT_FILE_FOR_TESTSPLIT    "../test/data/test_split_cases.xml"
#define INPUT_FILE_FOR_TESTSPLITBYDEPTH "../test/data/vis/testvis.xml"
#define INPUT_FILE_FOR_TESTSPLITBYDEPTH1 "../test/data/vis/testvis_1.xml"
#define INPUT_FILE_FOR_TESTSPLITBYDEPTH2 "../test/data/vis/testvis_2.xml"
#define INPUT_FILTER_TEXT   "../test/data/split/split_filter_text_src.xml"
#define RES_FILTER_TEXT_1   "../test/data/split/res_filter_text_1.xml"
#define RES_FILTER_TEXT_2   "../test/data/split/res_filter_text_2.xml"
#define RES_FILTER_TEXT_FILTER  "../test/data/split/res_filter_text_filter.xml"
#define RES_FILTER_TEXT_1_ORIG  "../test/data/split/res_filter_text_filter_1orig.xml"
#define RES_FILTER_TEXT_2_ORIG  "../test/data/split/res_filter_text_filter_2orig.xml"

#define INPUT_FILE_FOR_TESTFILTER "../test/data/split/filter_input.xml"
#define INPUT_FILE_FOR_TESTFILTER_COMPLEX "../test/data/split/filter_input_complex.xml"
#define INPUT_FILE_FOR_TESTFILTER_CFR "../test/data/split/filter_input_cfr.xml"
#define FILTER_RESULT_1    "../test/data/split/filter_res1.xml"
#define FILTER_RESULT_2    "../test/data/split/filter_res2.xml"
#define FILTER_RESULT_3    "../test/data/split/filter_res3.xml"
#define FILTER_RESULT_4    "../test/data/split/filter_res4.xml"
#define FILTER_RESULT_1_CFR    "../test/data/split/cfr_res1.xml"
#define FILTER_RESULT_2_CFR    "../test/data/split/cfr_res2.xml"

#define FILTER_RESULT_DEPTH0    "../test/data/split/splitter_res_depth0.xml"
#define FILTER_RESULT_DEPTH1    "../test/data/split/splitter_res_depth1.xml"
#define FILTER_RESULT_DEPTH2    "../test/data/split/splitter_res_depth2.xml"
//---------------------------------------------------------------------------------------

TestSplit::TestSplit()
{
    _showXML = false ;
}

TestSplit::~TestSplit()
{
}

bool TestSplit::error()
{
    return TestBase::error(_errorString);
}

bool TestSplit::testParameters()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setExtractDocuments(true);
    if(ExtractionOperation::ParamErrorNoInputFile != op.checkParameters()) {
        return false;
    }
    op.setInputFile(INPUT_FILE);
    if(ExtractionOperation::ParamErrorNoSplitPath != op.checkParameters()) {
        return false;
    }
    op.setSplitPath(SPLIT_PATH);
    op.setExtractRange();
    if(ExtractionOperation::ParamErrorNoMinRange != op.checkParameters()) {
        return false;
    }
    op.setMinDoc(1);
    if(ExtractionOperation::ParamErrorNoMaxRange != op.checkParameters()) {
        return false;
    }
    op.setMinDoc(100);
    op.setMaxDoc(1);
    if(ExtractionOperation::ParamErrorNoRange != op.checkParameters()) {
        return error();
    }
    op.setMinDoc(1);
    op.setMaxDoc(100);
    if(ExtractionOperation::ParamErrorNoExtractFolder != op.checkParameters()) {
        return error();
    }
    op.setExtractFolder(SystemServices::tempLocation());
    op.setIsMakeSubFolders(true);
    op.setSubFoldersEachNFiles(0);
    if(ExtractionOperation::ParamErrorSubFolderEach != op.checkParameters()) {
        return error();
    }
    op.setSubFoldersEachNFiles(1);
    if(ExtractionOperation::ParamErrorSubFolderNamePattern != op.checkParameters()) {
        return error();
    }
    QStringList list;
    list.append("a");
    op.setSubfolderNamePattern(list);
    if(ExtractionOperation::ParamErrorFilesNamePattern != op.checkParameters()) {
        return error();
    }
    op.setFilesNamePattern(list);
    op.setExtractCfr();
    if(ExtractionOperation::ParamErrorCfrAttr != op.checkParameters()) {
        return error();
    }

    op.setComparisonType((ExtractionOperation::ECfrOp) - 1);
    if(ExtractionOperation::ParamErrorCfrType != op.checkParameters()) {
        return error();
    }
    op.setComparisonType(ExtractionOperation::CFR_EQ);
    op.setAttributeName("test");

    if(!checkResetParametersFilterText(op) ) {
        return error();
    }
    op.setOperationType((ExtractionOperation::EOperationType)-1);
    if(ExtractionOperation::ParamErrorBadOperationType != op.checkParameters() ) {
        return error("check error EOperationType");
    }
    return true ;
}

bool TestSplit::checkResetParametersFilterText(ExtractionOperation &op)
{
    op.setFilterTextForPath(true);
    if(ExtractionOperation::ParamErrorNoDeleteTextPath != op.checkParameters()) {
        return error();
    }
    op.setPathForDeleteText(" ");
    if(ExtractionOperation::ParamErrorNoDeleteTextPath != op.checkParameters()) {
        return error();
    }
    op.setPathForDeleteText(";a");
    if(ExtractionOperation::ParamErrorNoDeleteTextPath != op.checkParameters()) {
        return error();
    }
    op.setPathForDeleteText("1a");
    if(ExtractionOperation::ParamErrorNoDeleteTextPath != op.checkParameters()) {
        return error();
    }
    op.setPathForDeleteText("a");
    if(ExtractionOperation::ParamErrorNoDeleteTextPath == op.checkParameters()) {
        return error();
    }
    op.setPathForDeleteText("/a/b/c");
    if(ExtractionOperation::ParamErrorNoDeleteTextPath == op.checkParameters()) {
        return error("checkResetParametersFilterText: abs path");
    }
    op.setFilterTextForPath(false);
    return true;
}

bool TestSplit::testParametersNoExtract()
{
    ExtractResults results;
    ExtractionOperation op(&results);

    op.setExtractDocuments(false);
    if(ExtractionOperation::ParamErrorNoInputFile != op.checkParameters()) {
        return error();
    }
    op.setInputFile(INPUT_FILE);
    if(ExtractionOperation::ParamErrorNoSplitPath != op.checkParameters()) {
        return error();
    }
    op.setSplitPath(SPLIT_PATH);
    op.setSplitType((ExtractionOperation::ESplitType)-1);
    if(ExtractionOperation::ParamErrorSplitType != op.checkParameters()) {
        return error();
    }
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setSplitDepth(-1);
    if(ExtractionOperation::ParamErrorSplitDepth != op.checkParameters()) {
        return error();
    }
    op.setSplitDepth(0);
    if(ExtractionOperation::ParamErrorSplitDepth != op.checkParameters()) {
        return error();
    }
    op.setSplitDepth(1);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setSplitDepth(1000);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setSplitType(ExtractionOperation::SplitUsingPath);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }

    // from now on, no bad check allowed
    op.setExtractRange();
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setMinDoc(1);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setMinDoc(100);
    op.setMaxDoc(1);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setMinDoc(1);
    op.setMaxDoc(100);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setExtractFolder(SystemServices::tempLocation());
    op.setIsMakeSubFolders(true);
    op.setSubFoldersEachNFiles(0);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setSubFoldersEachNFiles(1);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    QStringList list;
    list.append("a");
    op.setSubfolderNamePattern(list);
    if(ExtractionOperation::ParamNoError != op.checkParameters()) {
        return error();
    }
    op.setFilesNamePattern(list);

    if(!checkResetParametersFilterText(op) ) {
        return error();
    }

    return true ;
}

bool TestSplit::testUnitOperation()
{
    _testName = "testUnitOperation" ;
    ExtractResults results;
    ExtractionOperation op(&results);
    if(!op.isUseNamespaces()) {
        return error("Operation not ns default");
    }
    if(!op.filtersId().isEmpty()) {
        return error("Filters not default");
    }
    op.setUseNamespaces(false);
    if(op.isUseNamespaces()) {
        return error("Operation not ns default");
    }
    op.setFiltersId("a");
    if(op.filtersId().isEmpty()) {
        return error("Filters empty");
    }
    QMap<QString, QVariant> configBackend;
    Config::setBackend(&configBackend);
    op.saveSettings();
    ExtractionOperation op2(&results);
    op2.loadSettings();
    Config::setBackend(NULL);
    if(op2.isUseNamespaces()) {
        return error("Operation not ns loaded");
    }
    if(op2.filtersId().isEmpty()) {
        return error("Filters not loaded");
    }
    return true ;
}

bool TestSplit::testSplit()
{
    if(!testUnitOperation()) {
        return false;
    }
    _testName = "testSplit" ;
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE);
    op.setSplitPath(SPLIT_PATH);
    op.setExtractRange();
    op.setMinDoc(1);
    op.setMinDoc(100);
    op.setMaxDoc(1);
    op.setMinDoc(1);
    op.setMaxDoc(100);
    QString extFolder(SystemServices::tempLocation());
    extFolder.append(QDir::separator());
    extFolder.append("qxmledit_test");
    op.setExtractFolder(extFolder);
    op.setIsMakeSubFolders(true);
    op.setSubFoldersEachNFiles(0);
    op.setSubFoldersEachNFiles(NUM_FILES_PER_FOLDER);
    QStringList list;
    list.append(COUNTER_TOKEN_PTRN);
    list.append("_");
    list.append(DATE_TOKEN_PTRN);
    list.append("_");
    list.append(TIME_TOKEN_PTRN);
    list.append(SPACE_TOKEN_PTRN);
    list.append("");
    list.append(TS_TOKEN_PTRN);
    list.append(SEQUENCE_TOKEN_PTRN);

    op.setSubfolderNamePattern(list);
    op.setFilesNamePattern(list);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    return true ;
}

QString TestSplit::newTS()
{
    QString timeStamp;
    do {
        QDateTime dt = QDateTime::currentDateTime();
        timeStamp = QString::number(dt.toMSecsSinceEpoch());
    } while(timeStamp == _lastTimeStamp) ;
    _lastTimeStamp = timeStamp ;
    return timeStamp;
}

bool TestSplit::compareData(const QString &dataXML, const QString &fileName)
{
    QDomDocument document1;
    QDomDocument document2;
    CompareXML compare;
    if(!compare.loadFileIntoDocument(fileName, document1)) {
        return error();
    }
    if(!document2.setContent(dataXML)) {
        return error();
    }
    bool result = compare.compareDomDocuments(document1, document2);
    return result ;
}

bool TestSplit::checkSplit(ExtractResults &results, const int id, const QString &referenceFile)
{
    QDateTime dt = QDateTime::currentDateTime();
    const qint64 now = dt.toMSecsSinceEpoch();
    do {
        dt = QDateTime::currentDateTime();
    } while(dt.toMSecsSinceEpoch()==now);

    StringOperationResult sresult;
    results.loadFragment(id + 1, sresult);
    if(sresult.isError()) {
        return error(QString("Split in loadFragment: %1").arg(id));
    }
    if(_showXML) {
        Utils::message(sresult.result());
    }
    if(!compareData(sresult.result(), referenceFile)) {
        return error(QString("Split fragment not correct: %1").arg(id));
    }
    return true ;
}

bool TestSplit::checkFilter(const QString &file1, const QString &file2)
{
    return checkFilter(file1, file2, true);
}

bool TestSplit::checkFilter(const QString &file1, const QString &file2, const bool expectedPositiveResult)
{
    CompareXML compare;
    if(!compare.compareFiles(file1, file2)) {
        if(expectedPositiveResult) {
            compare.dumpErrorCause();
        }
        return error(QString("Filter fragment not correct: %1 vs %2").arg(file1).arg(file2));
    }
    return true ;
}

bool TestSplit::checkCSV(const QString &file1, const QString &fileReference)
{
    if(!compareFiles(file1, fileReference)) {
        return false;
    }
    return true ;
}

bool TestSplit::testSplitAndNavigate()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLIT);
    op.setSplitPath(SPLIT_PATH);
    op.setExtractDocuments(false);
    op.setExtractAllDocuments();
    // do operation
    op.performExtraction();
    if(op.isError()) {
        fprintf(stderr, "%s\n", (QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage())).toLatin1().data());
        return false;
    }
    if(!checkSplit(results, 0, SPLIT_FILE_0)) {
        fprintf(stderr, "Fragment 0 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 1, SPLIT_FILE_1)) {
        fprintf(stderr, "Fragment 1 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 2, SPLIT_FILE_2)) {
        fprintf(stderr, "Fragment 2 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 3, SPLIT_FILE_3)) {
        fprintf(stderr, "Fragment 3 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 4, SPLIT_FILE_4)) {
        fprintf(stderr, "Fragment 4 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 5, SPLIT_FILE_5)) {
        fprintf(stderr, "Fragment 5 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 6, SPLIT_FILE_6)) {
        fprintf(stderr, "Fragment 6 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 7, SPLIT_FILE_7)) {
        fprintf(stderr, "Fragment 7 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 8, SPLIT_FILE_8)) {
        fprintf(stderr, "Fragment 8 is not correct\n");
        return false;
    }
    if(!checkSplit(results, 9, SPLIT_FILE_9)) {
        fprintf(stderr, "Fragment 9 is not correct\n");
        return false;
    }
    return true ;
}

void TestSplit::setupFilterParameters(ExtractionOperation *operation, const bool isReverseRange, const QString &extractFolder,
                                      const QString &timeStamp, const QString &fileInput,
                                      const int minDoc, const int maxDoc)
{
    operation->setInputFile(fileInput);
    operation->setSplitPath(SPLIT_PATH);
    operation->setExtractDocuments(true);
    operation->setExtractRange();
    operation->setMinDoc(minDoc);
    operation->setMaxDoc(maxDoc);
    operation->setReverseRange(isReverseRange);
    operation->setExtractFolder(extractFolder);
    operation->setIsMakeSubFolders(true);
    operation->setSubFoldersEachNFiles(1);
    operation->setOperationType(ExtractionOperation::OperationFilter);
    QStringList list;
    list.append(timeStamp);
    operation->setFilesNamePattern(list);
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    operation->setSubfolderNamePattern(listF);
}

void TestSplit::setupFilterParametersCfr(ExtractionOperation *operation, const QString &extractFolder,
        const QString &timeStamp, const QString &fileInput,
        const QString &attrName, const ExtractionOperation::ECfrOp op, const QString &cfrTerm)
{
    operation->setInputFile(fileInput);
    operation->setSplitPath(SPLIT_PATH);
    operation->setExtractDocuments(true);
    operation->setExtractCfr();
    operation->setAttributeName(attrName);
    operation->setComparisonTerm(cfrTerm);
    operation->setComparisonType(op);
    operation->setExtractFolder(extractFolder);
    operation->setIsMakeSubFolders(true);
    operation->setSubFoldersEachNFiles(1);
    operation->setOperationType(ExtractionOperation::OperationFilter);
    QStringList list;
    list.append(timeStamp);
    operation->setFilesNamePattern(list);
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    operation->setSubfolderNamePattern(listF);
}

bool TestSplit::splitAndNavigateFilter(const bool isReverseRange, const QString &fileReference, const QString &fileResult,
                                       const int minDoc, const int maxDoc)
{
    ExtractResults results;
    ExtractionOperation op(&results);
    QString extractFolder(SystemServices::tempLocation());

    QString timeStamp = newTS();
    setupFilterParameters(&op, isReverseRange, extractFolder, timeStamp, fileReference, minDoc, maxDoc);

    QStringList list;
    list.append(timeStamp);
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    extractFolder.append(QDir::separator());
    extractFolder.append(timeStamp);
    extractFolder.append("qxmledit_test");
    QString fileName = op.makeAName(extractFolder, 1, list, 1);
    fileName.append(".xml");
    // QString fileName = operation.makeAName(const QString &nameBase, const int currentIndex, const QStringList &tokensList);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        fprintf(stderr, "%s\n", (QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage())).toLatin1().data());
        return false;
    }
    if(!checkFilter(fileResult, fileName)) {
        fprintf(stderr, "Filter is not correct\n");
        return false;
    }
    return true ;
}

bool TestSplit::splitAndNavigateFilterCfr(const QString &fileReference, const QString &fileResult,
        const QString &attrName, const ExtractionOperation::ECfrOp ops, const QString &cfrTerm)
{
    ExtractResults results;
    ExtractionOperation op(&results);
    QString extractFolder(SystemServices::tempLocation());

    QString timeStamp = newTS();
    setupFilterParametersCfr(&op, extractFolder, timeStamp, fileReference, attrName, ops, cfrTerm);

    QStringList list;
    list.append(timeStamp);
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    extractFolder.append(QDir::separator());
    extractFolder.append(timeStamp);
    extractFolder.append("qxmledit_test");
    QString fileName = op.makeAName(extractFolder, 1, list, 1);
    fileName.append(".xml");
    // QString fileName = operation.makeAName(const QString &nameBase, const int currentIndex, const QStringList &tokensList);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        fprintf(stderr, "%s\n", (QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage())).toLatin1().data());
        return error();
    }
    if(!checkFilter(fileResult, fileName)) {
        fprintf(stderr, "Filter is not correct\n");
        return error();
    }
    return true ;
}

bool TestSplit::testSplitAndNavigateFilter()
{
    return splitAndNavigateFilter(false, INPUT_FILE_FOR_TESTFILTER, FILTER_RESULT_1);
}

bool TestSplit::testSplitAndNavigateFilterMoreComplex()
{
    return splitAndNavigateFilter(false, INPUT_FILE_FOR_TESTFILTER_COMPLEX, FILTER_RESULT_2);
}

bool TestSplit::testSplitAndNavigateFilterReverse()
{
    return splitAndNavigateFilter(true, INPUT_FILE_FOR_TESTFILTER, FILTER_RESULT_3);
}

bool TestSplit::testSplitAndNavigateFilterMore()
{
    return splitAndNavigateFilter(false, INPUT_FILE_FOR_TESTFILTER, FILTER_RESULT_4, 4, 4);
}

bool TestSplit::testSplitAndNavigateFilterIdEqual()
{
    return splitAndNavigateFilterCfr(INPUT_FILE_FOR_TESTFILTER_CFR, FILTER_RESULT_1_CFR, "attr1", ExtractionOperation::CFR_EQ, "uno");
}

bool TestSplit::testSplitAndNavigateFilterIdDifferent()
{
    return splitAndNavigateFilterCfr(INPUT_FILE_FOR_TESTFILTER_CFR, FILTER_RESULT_2_CFR, "attr1", ExtractionOperation::CFR_NEQ, "uno");
}

//----------------------------------------------------------------------
bool TestSplit::testSplitByDepth0()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH);
    op.setSplitDepth(1);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH0)) {
        return error("Split by depth 0 is not correct");
    }
    return true ;
}

bool TestSplit::testSplitByDepth0_1()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH1);
    op.setSplitDepth(1);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH0)) {
        return error("Split by depth 0-1 is not correct");
    }
    return true ;
}

bool TestSplit::testSplitByDepth0_2()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH2);
    op.setSplitDepth(1);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH0)) {
        return error("Split by depth 0-2 is not correct");
    }
    return true ;
}

bool TestSplit::testSplitByDepth1()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH);
    op.setSplitDepth(2);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH1)) {
        return error("Split by depth 1 is not correct");
    }
    return true ;
}

bool TestSplit::testSplitByDepth1_1()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH1);
    op.setSplitDepth(2);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH1)) {
        return error("Split by depth 1-1 is not correct\n");
    }
    return true ;
}

bool TestSplit::testSplitByDepth1_2()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH2);
    op.setSplitDepth(2);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH1)) {
        return error("Split by depth 1-2 is not correct\n");
    }
    return true ;
}

bool TestSplit::testSplitByDepth2()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH);
    op.setSplitDepth(3);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH2)) {
        return error("Split by depth 2-1 is not correct\n");
    }
    return true ;
}

bool TestSplit::testSplitByDepth2_1()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH1);
    op.setSplitDepth(3);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH2)) {
        return error("Split by depth 2-1 is not correct\n");
    }
    return true ;
}

bool TestSplit::testSplitByDepth2_2()
{
    ExtractResults results;
    ExtractionOperation op(&results);
    op.setInputFile(INPUT_FILE_FOR_TESTSPLITBYDEPTH2);
    op.setSplitDepth(3);
    op.setSplitType(ExtractionOperation::SplitUsingDepth);
    op.setExtractDocuments(false);
    op.setExtractRange();
    op.setMaxDoc(1);
    op.setMinDoc(1);
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(!checkSplit(results, 0, FILTER_RESULT_DEPTH2)) {
        return error("Split by depth 2-1 is not correct\n");
    }
    return true ;
}

bool TestSplit::testSplitByDepth()
{
    if(!testSplitByDepth0()) {
        return error();
    }
    if(!testSplitByDepth1()) {
        return error();
    }
    if(!testSplitByDepth2()) {
        return error();
    }
    return true ;
}


/*
test relative:
a
 b
   c
     d
1:c
2:b/c
3:a/b/c

test absolute:
a/b/c

test to do: filter and split

*/
bool TestSplit::testSplitFilterTextAbsolute()
{
    if(!testFilterTextAbsoluteSplit()) {
        return error();
    }
    if(!testFilterTextAbsoluteFilter()) {
        return error();
    }
    if(!testFilterTextAbsoluteSplitWrongPath()) {
        return error();
    }
    if(!testFilterTextAbsoluteFilterWrongPath()) {
        return error();
    }
    return true ;
}

bool TestSplit::testSplitFilterTextRelative()
{
    if(!testFilterTextRelative1Split()) {
        return false;
    }
    if(!testFilterTextRelative2Split()) {
        return false;
    }
    if(!testFilterTextRelative1Filter()) {
        return false;
    }
    if(!testFilterTextRelative2Filter()) {
        return false;
    }
    if(!testFilterTextRelative1SplitWrongPath()) {
        return false;
    }
    if(!testFilterTextRelativeFilterWrongPath()) {
        return false;
    }
    if(!testFilterTextRelative2FilterExpectedFalse()) {
        return false;
    }
    return true ;
}

//----------

void TestSplit::setupFilterParametersFilterText(ExtractionOperation *operation,
                                                const QString &extractFolder,
                                              const QString &timeStamp, const QString &fileInput,
                                                const QString &pathForText, const bool isFilter)
{
    operation->setInputFile(fileInput);
    operation->setSplitPath(SPLIT_PATH_FILTER_TEXT);
    operation->setExtractDocuments(true);
    operation->setExtractAllDocuments();
    operation->setExtractFolder(extractFolder);
    operation->setIsMakeSubFolders(true);
    operation->setSubFoldersEachNFiles(1000);
    operation->setOperationType(isFilter?ExtractionOperation::OperationFilter:ExtractionOperation::OperationSplit);
    if( isFilter ) {
        operation->setSplitDepth(1);
        operation->setSplitType(ExtractionOperation::SplitUsingDepth);
    }
    QStringList list;
    list.append(timeStamp);
    list.append("%counter%");
    operation->setFilesNamePattern(list);
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    operation->setSubfolderNamePattern(listF);
    operation->setFilterTextForPath(true);
    operation->setPathForDeleteText(pathForText);
}

bool TestSplit::splitAndNavigateFilterText(const QString &fileReference, const bool isFilter,
                                               const QString &fileResult1, const QString &fileResult2,
                                               const QString &relativePath)
{
    return splitAndNavigateFilterText(fileReference, isFilter, fileResult1, fileResult2, relativePath, true );
}

bool TestSplit::splitAndNavigateFilterText(const QString &fileReference, const bool isFilter,
                                               const QString &fileResult1, const QString &fileResult2,
                                               const QString &relativePath, const bool expectPositiveResult )
{
    ExtractResults results;
    ExtractionOperation op(&results);
    QString extractFolder(SystemServices::tempLocation());

    QString timeStamp = newTS();
    setupFilterParametersFilterText(&op, extractFolder, timeStamp, fileReference, relativePath, isFilter);

    QStringList list;
    list.append(timeStamp);
    list.append("%counter%");
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    extractFolder.append(QDir::separator());
    extractFolder.append(timeStamp);
    extractFolder.append("qxmledit_test");
    QString fileName1 = op.makeAName(extractFolder, 1, list, 1);
    fileName1.append(".xml");
    QString fileName2 = op.makeAName(extractFolder, 2, list, 2);
    fileName2.append(".xml");
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split relative Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    bool result = checkFilter(fileResult1, fileName1, expectPositiveResult);
    if(result != expectPositiveResult) {
        return error("Filter is not correct 1");
    }
    if(!fileResult2.isEmpty()){
        result = checkFilter(fileResult2, fileName2);
        if(result != expectPositiveResult) {
            return error("Filter is not correct 2");
        }
    }
    return true ;
}

bool TestSplit::testFilterTextRelative1Split()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, false, RES_FILTER_TEXT_1, RES_FILTER_TEXT_2, "c");
}

bool TestSplit::testFilterTextRelative2Split()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, false, RES_FILTER_TEXT_1, RES_FILTER_TEXT_2, "b/c");
}

bool TestSplit::testFilterTextRelative1Filter()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, true, RES_FILTER_TEXT_FILTER, "", "c");
}

bool TestSplit::testFilterTextRelative2Filter()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, true, RES_FILTER_TEXT_FILTER, "", "b/c");
}

bool TestSplit::testFilterTextRelative2FilterFalse()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, true, RES_FILTER_TEXT_FILTER, "", "b/c/f");
}

bool TestSplit::testFilterTextRelative1SplitWrongPath()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, false, RES_FILTER_TEXT_1_ORIG, RES_FILTER_TEXT_2_ORIG, "f");
}

bool TestSplit::testFilterTextRelativeFilterWrongPath()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, true, INPUT_FILTER_TEXT, "", "e/f");
}

bool TestSplit::testFilterTextRelative2FilterExpectedFalse()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, false, RES_FILTER_TEXT_1_ORIG, "", "b/c", false);
}

//-----

bool TestSplit::testFilterTextAbsoluteSplit()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, false, RES_FILTER_TEXT_1, RES_FILTER_TEXT_2, "/a/b/c");
}


bool TestSplit::testFilterTextAbsoluteFilter()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, true, RES_FILTER_TEXT_FILTER, "", "/a/b/c");
}


bool TestSplit::testFilterTextAbsoluteSplitWrongPath()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, false, RES_FILTER_TEXT_1_ORIG, RES_FILTER_TEXT_2_ORIG, "/a/b/c/d/e");
}


bool TestSplit::testFilterTextAbsoluteFilterWrongPath()
{
    return splitAndNavigateFilterText(INPUT_FILTER_TEXT, true, INPUT_FILTER_TEXT, "", "/a/b/c/d/e");
}

#define BASE_DATA    "../test/data/split/group/"

#define INPUT_FILE_CSV_I0   BASE_DATA "simplei.xml"
#define OUTPUT_FILE_CSV_OCSV   BASE_DATA "simplepath.csv"
#define OUTPUT_FILE_CSV_OXML   BASE_DATA "simplepath.xml"
#define OUTPUT_FILE_CSV_O1CSV   BASE_DATA "simplen.csv"
#define OUTPUT_FILE_CSV_O1XML   BASE_DATA "simplen.xml"

#define INPUT_FILE_CSV_I1   BASE_DATA "simplei1.xml"

#define OUTPUT_FILE_CSV_1CSV   OUTPUT_FILE_CSV_OCSV
#define OUTPUT_FILE_CSV_1XML   OUTPUT_FILE_CSV_OXML
#define OUTPUT_FILE_CSV_11CSV   OUTPUT_FILE_CSV_O1CSV
#define OUTPUT_FILE_CSV_11XML   OUTPUT_FILE_CSV_O1XML

//----------
#define INPUT_FILE_CSV_I2   BASE_DATA "simplei2.xml"
#define OUTPUT_FILE_CSV_2CSV   BASE_DATA "simplepath2.csv"
#define OUTPUT_FILE_CSV_2XML   BASE_DATA "simplepath2.xml"
#define OUTPUT_FILE_CSV_21CSV   BASE_DATA "simplen2.csv"
#define OUTPUT_FILE_CSV_21XML   BASE_DATA "simplen2.xml"
//----------
#define INPUT_FILE_CSV_I3   BASE_DATA "simplei3.xml"
#define OUTPUT_FILE_CSV_3CSV   BASE_DATA "simplepath3.csv"
#define OUTPUT_FILE_CSV_3XML   BASE_DATA "simplepath3.xml"
#define OUTPUT_FILE_CSV_31CSV   BASE_DATA "simplen3.csv"
#define OUTPUT_FILE_CSV_31XML   BASE_DATA "simplen3.xml"
//----------

bool TestSplit::testFast()
{
    return true ;
}

bool TestSplit::testSplitGroup()
{
    _testName = "testSplitGroup";

    // CSV
    // one attribute
    if(!group(INPUT_FILE_CSV_I0, OUTPUT_FILE_CSV_OCSV, OUTPUT_FILE_CSV_OXML, OUTPUT_FILE_CSV_O1CSV, OUTPUT_FILE_CSV_O1XML) ) {
        return false ;
    }
    // same attributes on more lines
    if(!group(INPUT_FILE_CSV_I1, OUTPUT_FILE_CSV_1CSV, OUTPUT_FILE_CSV_1XML, OUTPUT_FILE_CSV_11CSV, OUTPUT_FILE_CSV_11XML) ) {
        return false ;
    }
    // with lacunes, even in first position
    if(!group(INPUT_FILE_CSV_I2, OUTPUT_FILE_CSV_2CSV, OUTPUT_FILE_CSV_2XML, OUTPUT_FILE_CSV_21CSV, OUTPUT_FILE_CSV_21XML) ) {
        return false ;
    }
    // differen text features
    if(!group(INPUT_FILE_CSV_I3, OUTPUT_FILE_CSV_3CSV, OUTPUT_FILE_CSV_3XML, OUTPUT_FILE_CSV_31CSV, OUTPUT_FILE_CSV_31XML) ) {
        return false ;
    }
    return true ;
}

bool TestSplit::group(const QString &fileReference, const QString &fileResultCSV, const QString &fileResultXML, const QString &fileResultCSVN, const QString &fileResultXMLN )
{
    if(!groupCSVXML(false, fileReference, fileResultCSV, true ) ) {
        return false;
    }
    if(!groupCSVXML(false, fileReference, fileResultCSVN, false ) ) {
        return false;
    }
    if(!groupCSVXML(true, fileReference, fileResultXML, true ) ) {
        return false;
    }
    if(!groupCSVXML(true, fileReference, fileResultXMLN, false ) ) {
        return false;
    }
    return true ;
}

bool TestSplit::groupCSVXML(const bool isXMLOrCSV, const QString &fileReference, const QString &fileResult, const bool isPath)
{
    _testName = QString("groupCSVXML %1 %2 %3").arg(isXMLOrCSV).arg(fileResult).arg(isPath);
    ExtractResults results;
    ExtractionOperation op(&results);
    QString extractFolder(SystemServices::tempLocation());

    QString timeStamp = newTS();
    setupFilterParametersFilterText(&op, extractFolder, timeStamp, fileReference, "", !isPath);
    if(!isPath) {
        op.setSplitDepth(3);
        op.setSplitType(ExtractionOperation::SplitUsingDepth);
    } else {
        op.setSplitPath("/root/one/two");
        op.setSplitType( ExtractionOperation::SplitUsingPath);
    }

    op.setOperationType(isXMLOrCSV?ExtractionOperation::OperationExportAndGroupXML:ExtractionOperation::OperationExportAndGroupCSV);

    QStringList list;
    list.append(timeStamp);
    list.append("%counter%");
    QStringList listF;
    listF.append(timeStamp);
    listF.append("qxmledit_test");
    extractFolder.append(QDir::separator());
    extractFolder.append(timeStamp);
    extractFolder.append("qxmledit_test");
    QString fileName1 = op.makeAName(extractFolder, 1, list, 1);
    fileName1.append(isXMLOrCSV?".xml":".csv");
    // do operation
    op.performExtraction();
    if(op.isError()) {
        return error(QString("Split relative Error: %1 %2").arg(op.error()).arg(op.errorMessage()));
    }
    if(isXMLOrCSV) {
        bool result = checkFilter(fileResult, fileName1);
        if(!result) {
            return error("Filter is not correct 1\n");
        }
    } else {
        bool result = checkCSV(fileName1, fileResult);
        if(!result) {
            return false;
        }
    }
    return true ;
}
