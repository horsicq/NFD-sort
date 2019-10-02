// copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));

    ui->checkBoxAllFileTypes->setChecked(true);
    ui->checkBoxAllTypes->setChecked(true);

    ui->lineEditDirectoryName->setText(QDir::currentPath());
    ui->lineEditOut->setText(QDir::currentPath());

    nCopyCount=0;

    dbSQLLite;

    dbSQLLite=QSqlDatabase::addDatabase("QSQLITE", "sqllite");
    dbSQLLite.setDatabaseName(":memory:");

    if(!dbSQLLite.open())
    {
        QMessageBox::critical(this,tr("Error"),tr("Cannot open SQLITE database"));
        exit(1);
    }
}

GuiMainWindow::~GuiMainWindow()
{
    delete ui;
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenDirectory_clicked()
{
    QString sInitDirectory=ui->lineEditDirectoryName->text();

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditDirectoryName->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonOut_clicked()
{
    QString sInitDirectory=ui->lineEditOut->text();

    QString sDirectoryName=QFileDialog::getExistingDirectory(this,tr("Open directory..."),sInitDirectory,QFileDialog::ShowDirsOnly);

    if(!sDirectoryName.isEmpty())
    {
        ui->lineEditOut->setText(sDirectoryName);
    }
}

void GuiMainWindow::on_pushButtonScan_clicked()
{
    _scan();
}

void GuiMainWindow::_scan()
{
    QSqlQuery srcQuery(dbSQLLite);
    srcQuery.exec("DROP TABLE if exists records");
    srcQuery.exec("DROP TABLE if exists files");
    srcQuery.exec("CREATE TABLE if not exists records(FILECRC text,FILECOUNT text,PRIMARY KEY(FILECRC))");
    srcQuery.exec("CREATE TABLE if not exists files(FILENAME text,TIMECOUNT text,DATETIME text,PRIMARY KEY(FILENAME))");

    nCopyCount=ui->spinBoxCopyCount->value();
    sResultDirectory=ui->lineEditOut->text();

    stFileTypes.clear();

    if(ui->checkBoxBinary->isChecked())             stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_BINARY);
    if(ui->checkBoxMSDOS->isChecked())              stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_MSDOS);
    if(ui->checkBoxPE32->isChecked())               stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_PE32);
    if(ui->checkBoxPE64->isChecked())               stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_PE64);
    if(ui->checkBoxELF32->isChecked())              stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_ELF32);
    if(ui->checkBoxELF64->isChecked())              stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_ELF64);
    if(ui->checkBoxMACHO32->isChecked())            stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_MACH32);
    if(ui->checkBoxMACHO64->isChecked())            stFileTypes.insert(SpecAbstract::RECORD_FILETYPE_MACH64);

    stTypes.clear();

    if(ui->checkBoxArchive->isChecked())            stTypes.insert(SpecAbstract::RECORD_TYPE_ARCHIVE);
    if(ui->checkBoxCertificate->isChecked())        stTypes.insert(SpecAbstract::RECORD_TYPE_CERTIFICATE);
    if(ui->checkBoxCompiler->isChecked())           stTypes.insert(SpecAbstract::RECORD_TYPE_COMPILER);
    if(ui->checkBoxConverter->isChecked())          stTypes.insert(SpecAbstract::RECORD_TYPE_CONVERTER);
    if(ui->checkBoxDatabase->isChecked())           stTypes.insert(SpecAbstract::RECORD_TYPE_DATABASE);
    if(ui->checkBoxDebugData->isChecked())          stTypes.insert(SpecAbstract::RECORD_TYPE_DEBUGDATA);
    if(ui->checkBoxDongleProtection->isChecked())   stTypes.insert(SpecAbstract::RECORD_TYPE_DONGLEPROTECTION);
    if(ui->checkBoxDOSExtender->isChecked())        stTypes.insert(SpecAbstract::RECORD_TYPE_DOSEXTENDER);
    if(ui->checkBoxFormat->isChecked())             stTypes.insert(SpecAbstract::RECORD_TYPE_FORMAT);
    if(ui->checkBoxGeneric->isChecked())            stTypes.insert(SpecAbstract::RECORD_TYPE_GENERIC);
    if(ui->checkBoxImage->isChecked())              stTypes.insert(SpecAbstract::RECORD_TYPE_IMAGE);
    if(ui->checkBoxInstaller->isChecked())          stTypes.insert(SpecAbstract::RECORD_TYPE_INSTALLER);
    if(ui->checkBoxInstallerData->isChecked())      stTypes.insert(SpecAbstract::RECORD_TYPE_INSTALLERDATA);
    if(ui->checkBoxLibrary->isChecked())            stTypes.insert(SpecAbstract::RECORD_TYPE_LIBRARY);
    if(ui->checkBoxLinker->isChecked())             stTypes.insert(SpecAbstract::RECORD_TYPE_LINKER);
    if(ui->checkBoxNETObfuscator->isChecked())      stTypes.insert(SpecAbstract::RECORD_TYPE_NETOBFUSCATOR);
    if(ui->checkBoxPacker->isChecked())             stTypes.insert(SpecAbstract::RECORD_TYPE_PACKER);
    if(ui->checkBoxProtector->isChecked())          stTypes.insert(SpecAbstract::RECORD_TYPE_PROTECTOR);
    if(ui->checkBoxProtectorData->isChecked())      stTypes.insert(SpecAbstract::RECORD_TYPE_PROTECTORDATA);
    if(ui->checkBoxSFX->isChecked())                stTypes.insert(SpecAbstract::RECORD_TYPE_SFX);
    if(ui->checkBoxSFXData->isChecked())            stTypes.insert(SpecAbstract::RECORD_TYPE_SFXDATA);
    if(ui->checkBoxSignTool->isChecked())           stTypes.insert(SpecAbstract::RECORD_TYPE_SIGNTOOL);
    if(ui->checkBoxSourceCode->isChecked())         stTypes.insert(SpecAbstract::RECORD_TYPE_SOURCECODE);
    if(ui->checkBoxStub->isChecked())               stTypes.insert(SpecAbstract::RECORD_TYPE_STUB);
    if(ui->checkBoxTool->isChecked())               stTypes.insert(SpecAbstract::RECORD_TYPE_TOOL);

    SpecAbstract::SCAN_OPTIONS options={0};
    options.bRecursive=ui->checkBoxRecursive->isChecked();
    options.bDeepScan=ui->checkBoxDeepScan->isChecked();
    options.bSubdirectories=ui->checkBoxScanSubdirectories->isChecked();

    DialogStaticScan ds(this);
    connect(&ds, SIGNAL(scanFileStarted(QString)),this,SLOT(scanFileStarted(QString)),Qt::DirectConnection);
    connect(&ds, SIGNAL(scanResult(SpecAbstract::SCAN_RESULT)),this,SLOT(scanResult(SpecAbstract::SCAN_RESULT)),Qt::DirectConnection);
    ds.setData(ui->lineEditDirectoryName->text(),&options);
    ds.exec();
}

quint32 GuiMainWindow::getFileCount(quint32 nCRC)
{
    quint32 nResult=0;

    QSqlQuery query(dbSQLLite);

    query.exec(QString("SELECT FILECOUNT FROM records where FILECRC='%1'").arg(nCRC));

    if(query.next())
    {
        nResult=query.value("FILECOUNT").toString().trimmed().toUInt();
    }

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }

    return nResult;
}

void GuiMainWindow::setFileCount(quint32 nCRC, quint32 nCount)
{
    QSqlQuery query(dbSQLLite);

    query.exec(QString("INSERT OR REPLACE INTO records(FILECRC,FILECOUNT) VALUES('%1','%2')").arg(nCRC).arg(nCount));

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }
}

void GuiMainWindow::setFileStat(QString sFileName, qint64 nTimeCount)
{
    QSqlQuery query(dbSQLLite);

    query.exec(QString("INSERT OR REPLACE INTO files(FILENAME,TIMECOUNT,DATETIME) VALUES('%1','%2','%3')")
               .arg(sFileName)
               .arg(nTimeCount)
               .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));

    if(query.lastError().text().trimmed()!="")
    {
        qDebug(query.lastQuery().toLatin1().data());
        qDebug(query.lastError().text().toLatin1().data());
    }
}

void GuiMainWindow::on_checkBoxAllFileTypes_toggled(bool checked)
{
    ui->checkBoxBinary->setChecked(checked);
    ui->checkBoxMSDOS->setChecked(checked);
    ui->checkBoxPE32->setChecked(checked);
    ui->checkBoxPE64->setChecked(checked);
    ui->checkBoxELF32->setChecked(checked);
    ui->checkBoxELF64->setChecked(checked);
    ui->checkBoxMACHO32->setChecked(checked);
    ui->checkBoxMACHO64->setChecked(checked);
}

void GuiMainWindow::on_checkBoxBinary_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxMSDOS_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxPE32_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxPE64_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxELF32_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxELF64_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxMACHO32_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxMACHO64_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllFileTypes);

    if(!checked)
    {
        ui->checkBoxAllFileTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxAllTypes_toggled(bool checked)
{
    ui->checkBoxArchive->setChecked(checked);
    ui->checkBoxCertificate->setChecked(checked);
    ui->checkBoxCompiler->setChecked(checked);
    ui->checkBoxConverter->setChecked(checked);
    ui->checkBoxDatabase->setChecked(checked);
    ui->checkBoxDebugData->setChecked(checked);
    ui->checkBoxDongleProtection->setChecked(checked);
    ui->checkBoxDOSExtender->setChecked(checked);
    ui->checkBoxFormat->setChecked(checked);
    ui->checkBoxGeneric->setChecked(checked);
    ui->checkBoxImage->setChecked(checked);
    ui->checkBoxInstaller->setChecked(checked);
    ui->checkBoxInstallerData->setChecked(checked);
    ui->checkBoxLibrary->setChecked(checked);
    ui->checkBoxLinker->setChecked(checked);
    ui->checkBoxNETObfuscator->setChecked(checked);
    ui->checkBoxPacker->setChecked(checked);
    ui->checkBoxProtector->setChecked(checked);
    ui->checkBoxProtectorData->setChecked(checked);
    ui->checkBoxSFX->setChecked(checked);
    ui->checkBoxSFXData->setChecked(checked);
    ui->checkBoxSignTool->setChecked(checked);
    ui->checkBoxSourceCode->setChecked(checked);
    ui->checkBoxStub->setChecked(checked);
    ui->checkBoxTool->setChecked(checked);
}

void GuiMainWindow::on_checkBoxArchive_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxCertificate_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxCompiler_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxConverter_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDatabase_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDebugData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDongleProtection_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxDOSExtender_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxFormat_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxGeneric_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxImage_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxInstaller_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxInstallerData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxLibrary_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxLinker_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxNETObfuscator_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxPacker_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxProtector_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxProtectorData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSFX_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSFXData_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSignTool_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxSourceCode_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxStub_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_checkBoxTool_toggled(bool checked)
{
    QSignalBlocker blocker(ui->checkBoxAllTypes);

    if(!checked)
    {
        ui->checkBoxAllTypes->setChecked(false);
    }
}

void GuiMainWindow::on_pushButtonInfo_clicked()
{
    QMessageBox::information(this,tr("Info"),tr("Bugreports: horsicq@gmail.com"));
}

void GuiMainWindow::scanFileStarted(QString sFileName)
{
    setFileStat(sFileName,0);
}

void GuiMainWindow::scanResult(SpecAbstract::SCAN_RESULT scanResult)
{
    int nCount=scanResult.listRecords.count();

    for(int i=0;i<nCount;i++)
    {
        SpecAbstract::SCAN_STRUCT ss=scanResult.listRecords.at(i);

        if(stFileTypes.contains(ss.id.filetype)&&stTypes.contains(ss.type))
        {
            QString sResult=SpecAbstract::recordNameIdToString(ss.name);

            if(ss.sVersion!="")
            {
                sResult+=QString("(%1)").arg(ss.sVersion);
            }

            if(ss.sInfo!="")
            {
                sResult+=QString("[%1]").arg(ss.sInfo);
            }

            sResult=XBinary::convertFileNameSymbols(sResult);

            quint32 nCRC=XBinary::getCRC32(sResult);

            bool bCopy=true;

            int nCurrentCount=getFileCount(nCRC);

            if(nCopyCount)
            {
                if(nCurrentCount>=nCopyCount)
                {
                    bCopy=false;
                }
            }

            if(bCopy)
            {
                QString sFileName=sResultDirectory;

                XBinary::createDirectory(sFileName);
                sFileName+=QDir::separator()+SpecAbstract::recordFiletypeIdToString(ss.id.filetype);
                XBinary::createDirectory(sFileName);
                sFileName+=QDir::separator()+SpecAbstract::recordTypeIdToString(ss.type);
                XBinary::createDirectory(sFileName);
                sFileName+=QDir::separator()+sResult;
                XBinary::createDirectory(sFileName);
                sFileName+=QDir::separator()+XBinary::getBaseFileName(scanResult.sFileName);

                if(XBinary::copyFile(scanResult.sFileName,sFileName))
                {
                    setFileCount(nCRC,nCurrentCount+1);
                    setFileStat(scanResult.sFileName,scanResult.nScanTime);
                }
            }
        }
    }
}
