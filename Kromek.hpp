#ifndef __AA_KROMEK_HPP__
#define __AA_KROMEK_HPP__

#include <map>
#include <iostream>
#include <string_view>
#include <vector>
#include <memory>

namespace Kromek {

class Sensor {
public:
    Sensor(unsigned id);
    ~Sensor();
    // Acquisition
    bool aquiring() const;
    void start();
    void stop();
    void clear();
    // Data
    unsigned counts();
    std::vector<unsigned> spectrum();
    unsigned realtime();
    unsigned livetime();
    // Info
    unsigned id() const { return m_id; }
    int serial() const { return m_serial; }
    std::string name() const;
    std::string manufacturer() const;
    int vendor() const;
    int product() const;
    std::string info() const;
protected:
    void populate();
private:
    unsigned m_id;
    unsigned m_serial;
    // Acquistion
    unsigned m_realtime;
    unsigned m_livetime;
};

class Driver {
public:
    using Sensors = std::map<unsigned, std::shared_ptr<Sensor>>;
    explicit Driver();
    ~Driver();
    std::string version() const;
    // Sensors
    std::shared_ptr<Sensor> sensor(unsigned serial);
    const std::shared_ptr<Sensor> sensor(unsigned serial) const { return sensor(serial); }
    Sensors &sensors() { return s_sensors; }
    const Sensors &sensors() const { return s_sensors; }
protected:
    static void addSensor(unsigned id);
    static void removeSensor(unsigned id);
    static void deviceChangedCallback(unsigned id, int added, void *caller);
    static void errorCallback(void *caller, unsigned id, int code, const char *message);
private:
    // Devices
    static Sensors s_sensors; // {serial, sensor}
};

}

#endif