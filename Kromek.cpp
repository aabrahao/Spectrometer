#include "Kromek.hpp"
#include "Helpers.hpp"
#include <SpectrometerDriver.h>
#include <sstream>
#include <format>

const size_t g_size = 126;  // Max number of characters for a USB product.
char g_buffer[g_size];

unsigned fromId(unsigned id) {
    int bytes;
    kr_GetDeviceSerial(id, g_buffer, g_size, &bytes);
    return std::stoi(std::string(g_buffer, bytes));
}

//////////////////////////////////////////////////////////////
// Sensor

Kromek::Sensor::Sensor(unsigned id) 
: m_id(id), m_serial(fromId(id)) {
    log() << "Kromek: Sensor " << info() << " connected!" << std::endl;
}

Kromek::Sensor::~Sensor() {
    log() << "Kromek: Sensor " << info() << " desconnected!" << std::endl;
}

bool Kromek::Sensor::aquiring() const { 
    return kr_IsAcquiringData(m_id); 
}

void Kromek::Sensor::start() {
    if (aquiring())
        stop();
    auto okay = kr_BeginDataAcquisition(m_id, m_realtime*1000, m_livetime*1000);
    if ( okay != ERROR_OK)
        log() << "Kromek error: Ops, unable to read from device";
}

void Kromek::Sensor::stop() {
    kr_StopDataAcquisition(m_id);
}

void Kromek::Sensor::clear() {
    kr_ClearAcquiredData(m_id);
}

unsigned Kromek::Sensor::counts() {
    unsigned counts = 0;
    kr_GetAcquiredData(m_id, nullptr, &counts, nullptr, nullptr);
    return counts;
}

std::vector<unsigned> Kromek::Sensor::spectrum() {
    std::vector<unsigned> spectrum(TOTAL_RESULT_CHANNELS);
    kr_GetAcquiredData(m_id, spectrum.data(), nullptr, nullptr, nullptr);
    return spectrum;
}

unsigned Kromek::Sensor::realtime() {
    unsigned time = 0;
    kr_GetAcquiredData(m_id, nullptr, nullptr, &time, nullptr);
    return time;
}

unsigned Kromek::Sensor::livetime() {
    unsigned time = 0;
    kr_GetAcquiredData(m_id, nullptr, nullptr, nullptr, &time);
    return time;
}

std::string Kromek::Sensor::name() const {
    int bytes;
    kr_GetDeviceName(m_id, g_buffer, g_size, &bytes);
    return std::string(g_buffer, bytes);
}

std::string Kromek::Sensor::manufacturer() const {
    int bytes;
    kr_GetDeviceManufacturer(m_id, g_buffer, g_size, &bytes);
    return std::string(g_buffer, bytes);
}

int Kromek::Sensor::vendor() const {
    int value;
    kr_GetDeviceVendorID(m_id, &value);
    return value;
}

int Kromek::Sensor::product() const {
    int value;
    kr_GetDeviceProductID(m_id, &value);
    return value;
}

std::string Kromek::Sensor::info() const {
    return std::format("[{}] {} SN#{}", id(), name(), serial());
}

//////////////////////////////////////////////////////////////
// Driver

Kromek::Driver::Sensors Kromek::Driver::s_sensors;

Kromek::Driver::Driver() {
    log() << "Opening Kromek driver, version " + version() << std::endl;
    kr_SetDeviceChangedCallback(deviceChangedCallback, this);
    kr_Initialise(errorCallback, this);
}

Kromek::Driver::~Driver() {
    s_sensors.clear();
    kr_Destruct();
    log() << "Closing kromek driver." << std::endl;
}

std::string Kromek::Driver::version() const {
    int product, major, minor, build;
    kr_GetVersionInformation(&product, &major, &minor, &build);
    return std::format("{}-{}.{}.{}", product, major, minor, build);
}

std::shared_ptr<Kromek::Sensor> Kromek::Driver::sensor(unsigned serial) {
    auto s = s_sensors.find(serial);
    if (s == s_sensors.end())
        return nullptr;
    else
        return s->second;
}

void Kromek::Driver::addSensor(unsigned id) {
    removeSensor(id);
    auto s = std::make_shared<Sensor>(id);
    s_sensors.insert({s->serial(), s});
}

void Kromek::Driver::removeSensor(unsigned id) {
    for (auto s = s_sensors.begin(); s != s_sensors.end(); ++s) {
        if (s->second->id() == id) {
            s_sensors.erase(s);
            break;
        }
    }
}

//////////////////////////////////////////////////////////////
// Driver Callbacks

void Kromek::Driver::deviceChangedCallback(unsigned id, BOOL added, void *caller) {  
    if (added)
        addSensor(id);
    else
        removeSensor(id);
}
    
void Kromek::Driver::errorCallback(void *caller, unsigned id, int code, const char *message) {
    log() << "Kromek error: Ops, ";
    switch (code) {
        case ERROR_NOT_INITIALISED:
            log() << "library not initialised!";
            break;
        case ERROR_ACQUISITION_COMPLETE:
            log() << "acquisition completed!";
            break;
        default:
            log() << "Unknown message" << message;
    }
}