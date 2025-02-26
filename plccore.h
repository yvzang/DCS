#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <functional>
#include <map>
#include <condition_variable>
#include <memory>
#include "plc_protocol.h"

typedef std::string DeviceNameType;

typedef enum{
    PROTOCOL_TYPE_FX = 0,
    PROTOCOL_TYPE_MC,
    PROTOCOL_TYPE_S7,
    PROTOCOL_TYPE_MODBUS_TCP,
    PROTOCOL_TYPE_MODBUS_RTU
}ProtolcolType;

typedef enum{
    PLC_ACCESS_READ = 0,
    PLC_ACCESS_WRITE
}PLCReadWrite_t;

typedef enum{
    PLC_ADDRESS_BOOL = 0,
    PLC_ADDRESS_BYTE,
    PLC_ADDRESS_SHORT,
    PLC_ADDRESS_USHORT,
    PLC_ADDRESS_INT32,
    PLC_ADDRESS_UINT32,
    PLC_ADDRESS_FLOAT,
}PLCAddress_t;

typedef enum{
    PLCDATABLOCK_TYPE_BOOL = 0,
    PLCDATABLOCK_TYPE_WORD
}PLCDataBlockType;

typedef struct _DataPayload
{
    std::vector<bool> dbool;
    std::vector<byte> dbyte;
    std::vector<short> dshort;
    std::vector<ushort> dushort;
    std::vector<int32> dint32;
    std::vector<uint32> duint32;
    std::vector<float> dfloat;
} DataPayload;

typedef std::function<void()> PLCTaskCallback;

typedef struct _DeviceTask{
    DeviceNameType deviceID;
    PLCReadWrite_t rw;
    PLCAddress_t addrType;
    std::string address;
    size_t size;
    std::shared_ptr<DataPayload> payload;
    PLCTaskCallback PLCTaskSuccessCallback = nullptr;
    PLCTaskCallback PLCTaskFailedCallback = nullptr;
}DeviceTask;

typedef struct _DeviceDescribe{
    DeviceNameType deviceName;
    //PLC connection setting
    ProtolcolType protoType;
    std::string ipAddr;
    int port;
    siemens_plc_types_e siemensPLCType;
    //modbus setting
    int modbus_slave;
    std::string serialDeviceName;
    int baut;
    char parity;
    int dataBits;
    int stopBits;
} DeviceDescribe;

class DeviceManager{
private:
    std::atomic<bool> Stop_;
    std::thread CoreThread_;

    int TaskMaxThreshHold_;
    std::condition_variable TaskFull_;
    std::condition_variable TaskEmpty_;
    std::mutex TaskMutex_;
    std::atomic<uint16_t> TaskSize_;
    std::queue<std::shared_ptr<DeviceTask>> TaskQueue_;

    std::map<DeviceNameType, std::shared_ptr<PLCProtocol>> deviceList_;
public:
    DeviceManager();
    ~DeviceManager();

    bool registerDevice(DeviceDescribe describe);
    bool reconnectDevice(DeviceNameType id);
    void unregisterDevice(DeviceNameType id);

    bool submitePLCTask(std::shared_ptr<DeviceTask> task);

    bool isRunning(){return !Stop_.load();};
    void run();
    void stop();
private:
    void coreThreadFunc_();
};