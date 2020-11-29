#include "GenericCanSetupPage.h"
#include "ui_GenericCanSetupPage.h"
#include <core/Backend.h>
#include <driver/CanInterface.h>
#include <core/MeasurementInterface.h>
#include <window/SetupDialog/SetupDialog.h>
#include <QList>
#include <QtAlgorithms>

GenericCanSetupPage::GenericCanSetupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GenericCanSetupPage),
    _mi(0),
    _enable_ui_updates(false)
{
    ui->setupUi(this);
    connect(ui->cbBitrate, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbSamplePoint, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbBitrateFD, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbSamplePointFD, SIGNAL(currentIndexChanged(int)), this, SLOT(updateUI()));

    connect(ui->cbConfigOS, SIGNAL(stateChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbListenOnly, SIGNAL(stateChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbOneShot, SIGNAL(stateChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbLoopBack, SIGNAL(stateChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbTripleSampling, SIGNAL(stateChanged(int)), this, SLOT(updateUI()));
    connect(ui->cbAutoRestart, SIGNAL(stateChanged(int)), this, SLOT(updateUI()));
}

GenericCanSetupPage::~GenericCanSetupPage()
{
    delete ui;
}

void GenericCanSetupPage::onSetupDialogCreated(SetupDialog &dlg)
{
    dlg.addPage(this);
    connect(&dlg, SIGNAL(onShowInterfacePage(SetupDialog&,MeasurementInterface*)), this, SLOT(onShowInterfacePage(SetupDialog&,MeasurementInterface*)));
}

void GenericCanSetupPage::onShowInterfacePage(SetupDialog &dlg, MeasurementInterface *mi)
{
    _mi = mi;
    CanInterface *intf = backend().getInterfaceById(_mi->canInterface());

    _enable_ui_updates = false;

    ui->laDriver->setText(intf->getDriver()->getName());
    ui->laInterface->setText(intf->getName());
    ui->laInterfaceDetails->setText(intf->getDetailsStr());

    fillBitratesList(intf, _mi->bitrate());
    fillSamplePointsForBitrate(intf, _mi->bitrate(), _mi->samplePoint());

    ui->cbConfigOS->setChecked(!_mi->doConfigure());
    ui->cbListenOnly->setChecked(_mi->isListenOnlyMode());
    ui->cbOneShot->setChecked(_mi->isOneShotMode());
    ui->cbTripleSampling->setChecked(_mi->isTripleSampling());
    ui->cbAutoRestart->setChecked(_mi->doAutoRestart());

    disenableUI(_mi->doConfigure());
    dlg.displayPage(this);

    _enable_ui_updates = true;
}

void GenericCanSetupPage::updateUI()
{
    if (_enable_ui_updates && (_mi!=0)) {
        CanInterface *intf = backend().getInterfaceById(_mi->canInterface());

        _mi->setDoConfigure(!ui->cbConfigOS->isChecked());
        _mi->setListenOnlyMode(ui->cbListenOnly->isChecked());
        _mi->setOneShotMode(ui->cbOneShot->isChecked());
        _mi->setLoopBackMode(ui->cbLoopBack->isChecked());
        _mi->setTripleSampling(ui->cbTripleSampling->isChecked());
        _mi->setAutoRestart(ui->cbAutoRestart->isChecked());
        _mi->setBitrate(ui->cbBitrate->currentData().toUInt());
        _mi->setSamplePoint(ui->cbSamplePoint->currentData().toUInt());

        _enable_ui_updates = false;

        disenableUI(_mi->doConfigure());
        fillSamplePointsForBitrate(
            intf,
            ui->cbBitrate->currentData().toUInt(),
            ui->cbSamplePoint->currentData().toUInt()
        );
        _enable_ui_updates = true;

    }


}

void GenericCanSetupPage::fillBitratesList(CanInterface *intf, unsigned selectedBitrate)
{
    QList<uint32_t> bitrates;
    foreach (CanTiming t, intf->getAvailableBitrates()) {
        if (!bitrates.contains(t.getBitrate())) {
            bitrates.append(t.getBitrate());
        }
    }
    qSort(bitrates);

    ui->cbBitrate->clear();
    foreach (uint32_t br, bitrates) {
        ui->cbBitrate->addItem(QString::number(br), br);
    }
    ui->cbBitrate->setCurrentText(QString::number(selectedBitrate));
}

void GenericCanSetupPage::fillSamplePointsForBitrate(CanInterface *intf, unsigned selectedBitrate, unsigned selectedSamplePoint)
{
    QList<uint32_t> samplePoints;
    foreach(CanTiming t, intf->getAvailableBitrates()) {
        if (t.getBitrate() == selectedBitrate) {
            if (!samplePoints.contains(t.getSamplePoint())) {
                samplePoints.append(t.getSamplePoint());
            }
        }
    }
    qSort(samplePoints);

    ui->cbSamplePoint->clear();
    foreach (uint32_t sp, samplePoints) {
        ui->cbSamplePoint->addItem(CanTiming::getSamplePointStr(sp), sp);
    }
    ui->cbSamplePoint->setCurrentText(CanTiming::getSamplePointStr(selectedSamplePoint));
}

void GenericCanSetupPage::disenableUI(bool enabled)
{

    CanInterface *intf = backend().getInterfaceById(_mi->canInterface());
    uint32_t caps = intf->getCapabilities();

    ui->cbBitrate->setEnabled(enabled);
    ui->cbSamplePoint->setEnabled(enabled);
    ui->cbConfigOS->setEnabled(caps & CanInterface::capability_config_os);

    ui->cbBitrateFD->setEnabled(enabled && (caps & CanInterface::capability_canfd));
    ui->cbSamplePointFD->setEnabled(enabled && (caps & CanInterface::capability_canfd));
    ui->cbListenOnly->setEnabled(enabled && (caps & CanInterface::capability_listen_only));
    ui->cbOneShot->setEnabled(enabled && (caps & CanInterface::capability_one_shot));
    ui->cbLoopBack->setEnabled(enabled && (caps & CanInterface::capability_loop_back));
    ui->cbTripleSampling->setEnabled(enabled && (caps & CanInterface::capability_triple_sampling));
    ui->cbAutoRestart->setEnabled(enabled && (caps & CanInterface::capability_auto_restart));
}

Backend &GenericCanSetupPage::backend()
{
    return Backend::instance();
}
