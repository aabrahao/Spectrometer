#include "SpectrometerDriver.h"

Kromek::Kromek(int id) :
    m_id(id) {
    kr_SetDeviceChangedCallback(krDeviceChangedCallback, this);
    kr_Initialise(krErrorCallback, this);
}

std::string Kromek::version() const {
    int productVer, majVer, minVer, buildVer;
    kr_GetVersionInformation(&productVer, &majVer, &minVer, &buildVer);

    return productVer
}

Kromek::~Kromek() {
    kr_Destruct();
}

bool Kromek::aquiring() const {
    return kr_IsAcquiringData(deviceID);
}

void Kromek::start() {
    unsigned int deviceID = GetSelectedDeviceID();
    if (deviceID == 0)
        return;
    if (aquiring())
        stop();
    int okay = kr_BeginDataAcquisition(m_id, m_realTime * 1000, m_liveTime * 1000);
    if ( okay != ERROR_OK)
        std::out << "Ops, Error unable to read from device");
    update();
}

void Kromek::stop() {
    unsigned int deviceID = GetSelectedDeviceID();
    if (deviceID == 0)
        return;
    kr_StopDataAcquisition(deviceID);
}

void Kromek::clear() {
    unsigned int deviceID = GetSelectedDeviceID();
    if (deviceID == 0)
        return;
    kr_ClearAcquiredData(deviceID);
}

void Kromek::errorCallback(int /*deviceID*/, int code, std::string errorMsg) {
    std::string msg;
    switch (code) {
        case ERROR_NOT_INITIALISED:
            msg = "Library not initialised";
            break;
        case ERROR_ACQUISITION_COMPLETE:
            msg = "Acquisition Completed";
            update();
            break;
        default:
            msg = errorMsg;
    }
    return msg;
}

void Kromek::deviceChangedCallback(unsigned int /*detectorID*/, BOOL /*added*/, void *pCallbackObject) {

}

void Kromek::updateDetectorList() {
    // Enumerate each of the attached detectors
    unsigned int detectorID = 0;
    while ((detectorID = kr_GetNextDetector(detectorID)) != 0) {
        const unsigned int cNumberOfCharacters = 126;  // max number of characters for a USB product.
        // Retrieve individual details about the device
        char deviceName[cNumberOfCharacters];
        int bytesOut;
        kr_GetDeviceName(detectorID, deviceName, cNumberOfCharacters, &bytesOut);
        char manufacturer[cNumberOfCharacters];
        kr_GetDeviceManufacturer(detectorID, manufacturer, cNumberOfCharacters, &bytesOut);
        char serialNumber[cNumberOfCharacters];
        kr_GetDeviceSerial(detectorID, serialNumber, cNumberOfCharacters, &bytesOut);
        int vendorID, productID;
        kr_GetDeviceVendorID(detectorID, &vendorID);
        kr_GetDeviceProductID(detectorID, &productID);
    }
}

// Update the display of the currently selected detectors acquisition data
void Kromek::update() {
    unsigned int totalCounts = 0;
    unsigned int realTime = 0;
    unsigned int liveTime  = 0;

    std::vector<unsigned int> spectrumData(TOTAL_RESULT_CHANNELS);

    unsigned int deviceID = GetSelectedDeviceID();
    if (deviceID == 0)
        return;

    kr_GetAcquiredData(deviceID, spectrumData.data(), &totalCounts, &realTime, &liveTime);
}
