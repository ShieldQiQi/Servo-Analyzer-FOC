#ifndef CANDLEAPIINTERFACE_H
#define CANDLEAPIINTERFACE_H

#include <driver/CanInterface.h>
#include <core/MeasurementInterface.h>
#include "api/candle.h"
#include "CandleApiTiming.h"
#include <windows.h>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

class CandleApiDriver;

using namespace std;

class CandleApiInterface : public CanInterface
{
public:
    CandleApiInterface(CandleApiDriver *driver, candle_handle handle);
    virtual ~CandleApiInterface();

    virtual QString getName() const;
    virtual QString getDetailsStr() const;

    virtual void applyConfig(const MeasurementInterface &mi);

    virtual unsigned getBitrate();

    virtual uint32_t getCapabilities();
    virtual QList<CanTiming> getAvailableBitrates();

    virtual void open();
    virtual void close();

    virtual int sendMessage(const CanMessage &msg);
    virtual int sendMessage(const CanMessage &msg, unsigned int timeout_ms);
    virtual bool readMessage(CanMessage &msg, unsigned int timeout_ms);

    virtual bool updateStatistics();
    virtual uint32_t getState();
    virtual int getNumRxFrames();
    virtual int getNumRxErrors();
    virtual int getNumTxFrames();
    virtual int getNumTxErrors();
    virtual int getNumRxOverruns();
    virtual int getNumTxDropped();

    wstring getPath() const;

    void update(candle_handle dev);

private:

    uint64_t _hostOffsetStart;
    uint32_t _deviceTicksStart;

    candle_handle _handle;
    MeasurementInterface _settings;
    Backend &_backend;

    uint64_t _numRx;
    uint64_t _numTx;
    uint64_t _numTxErr;
    uint16_t _echo_id;
    uint16_t _echo_id_rx;
    int _TxErr;

    QMutex _mutex;
    QWaitCondition _tx_wait;
    QList<CandleApiTiming> _timings;

    bool setBitTiming(uint32_t bitrate, uint32_t samplePoint);
};

#endif // CANDLEAPIINTERFACE_H
