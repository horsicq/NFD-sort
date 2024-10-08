/* Copyright (c) 2019-2023 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef SCANPROGRESS_H
#define SCANPROGRESS_H

#include <QFutureWatcher>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>
#include <QSemaphore>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtConcurrent>

#include "staticscan.h"

class ScanProgress : public QObject {
    Q_OBJECT
public:
    enum CF {
        CF_FT_TYPE_NAME = 0,
        CF_ARCH_FT_TYPE_NAME,
        CF_FT_ARCH_TYPE_NAME
    };

    enum CT {
        CT_IDENT = 0,
        CT_IDENT_UNK,
        CT_UNK
    };

    enum UP {
        UP_NONE = 0,
        UP_EP_BYTES,
        UP_HEADER_BYTES,
        UP_OVERLAY_BYTES
    };

    enum FF {
        FF_ORIGINAL = 0,
        FF_MD5,
        FF_MD5_ORIGINAL
    };

    struct SCAN_OPTIONS {
        bool bRecursive;
        bool bDeepScan;
        bool bSubdirectories;
        bool bHeuristic;
        bool bVerbose;
        QSet<XBinary::FT> stFileTypes;
        bool bAllTypes;
        QSet<SpecAbstract::RECORD_TYPE> stTypes;
        qint32 nCopyCount;
        QString sResultDirectory;
        QSqlDatabase dbSQLLite;
        bool bContinue;
        bool bDebug;
        bool bIsTest;
        CF copyFormat;
        CT copyType;
        FF fileFormat;
        bool bRemoveCopied;
    };
    struct STATS {
        qint32 nTotal;
        qint32 nCurrent;
        qint64 nElapsed;
        QString sStatus;
        qint32 nNumberOfThreads;
    };

    explicit ScanProgress(QObject *parent = nullptr);

    void setData(QString sDirectoryName, ScanProgress::SCAN_OPTIONS *pOptions);

    quint32 getFileCount(quint32 nCRC);
    void setFileCount(quint32 nCRC, quint32 nCount);
    void setFileStat(QString sFileName, QString sTimeCount, QString sDate);
    void createTables();
    QString getCurrentFileName();
    QString getCurrentFileNameAndLock();
    qint64 getNumberOfFile();
    void findFiles(QString sDirectoryName);

    void startTransaction();
    void endTransaction();

    void _processFile(QString sFileName);

    static QString createPath(CF copyFormat, XScanEngine::SCANSTRUCT ss);

private slots:
    void scan_finished();

signals:
    void completed(qint64 nElapsedTime);

public slots:
    void process();
    void stop();
    STATS getCurrentStats();
    static bool createDatabase(QSqlDatabase *pDb, QString sDatabaseName);

private:
    // const int N_MAXNUMBEROFTHREADS = 8;
#ifdef Q_OS_WIN
    const int N_MAXNUMBEROFTHREADS = 8;
#else
    const int N_MAXNUMBEROFTHREADS = 1;
#endif
    QString _sDirectoryName;
    SCAN_OPTIONS *_pOptions;
    bool bIsStop;
    STATS currentStats;
    QElapsedTimer *pElapsedTimer;
    QMutex mutex;
    QSemaphore *pSemaphore;
};

#endif  // SCANPROGRESS_H
