#include <common/RelativeLockTime.hpp>
#include <common/Utilities.hpp>
#include <QDataStream>
#include <stdexcept>
#include <stdutils/uchar_vector.h>

namespace Coin {

RelativeLockTime::RelativeLockTime()
    : RelativeLockTime(Units::Time, 0)
    {}

RelativeLockTime::RelativeLockTime(const uint32_t &blocks)
    : RelativeLockTime(Units::Blocks, blocks)
    {}

RelativeLockTime::RelativeLockTime(const Units &units, const LockTimeCounter &counter)
    : _counter(counter),
      _units(units)
    {}

RelativeLockTime RelativeLockTime::fromTimeUnits(const LockTimeCounter &counter) {
    return RelativeLockTime(Units::Time, counter);
}

RelativeLockTime RelativeLockTime::fromSecondsDuration(const std::chrono::seconds &seconds) {
    return RelativeLockTime(Units::Time, secondsToCounter(seconds));
}

RelativeLockTime RelativeLockTime::fromBlockUnits(const LockTimeCounter &blocks) {
    return RelativeLockTime(Units::Blocks, blocks);
}

bool RelativeLockTime::operator==(const RelativeLockTime & rhs) const {
    return _units == rhs.units() &&
           _counter == rhs.counter();
}

bool RelativeLockTime::operator!=(const RelativeLockTime & rhs) const {
    return !(*this == rhs);
}

bool RelativeLockTime::operator<(const RelativeLockTime & rhs) const {
    if(_units != rhs.units())
        throw std::runtime_error("cannot compare relative locktimes based on different units");

    return _counter < rhs.counter();
}

bool RelativeLockTime::operator>(const RelativeLockTime & rhs) const {
    if(_units != rhs.units())
        throw std::runtime_error("cannot compare relative locktimes based on different units");

    return _counter > rhs.counter();
}

RelativeLockTime::Units RelativeLockTime::units() const {
    return _units;
}

void RelativeLockTime::units(const Units &u) {
    _units = u;
}

void RelativeLockTime::counter(const LockTimeCounter &c) {
    _counter = c;
}

RelativeLockTime::LockTimeCounter RelativeLockTime::counter() const {
    return _counter;
}

std::chrono::seconds RelativeLockTime::getDuration() const {
    if(_units != Units::Time)
        throw std::runtime_error("cannot get duration in seconds of a block based relative locktime");

    return counterToSeconds(_counter);
}

void RelativeLockTime::setBlocks(const LockTimeCounter &blocks) {
    _units = Units::Blocks;
    _counter = blocks;
}

void RelativeLockTime::setDuration(const std::chrono::seconds &seconds) {
    _units = Units::Time;
    _counter = secondsToCounter(seconds);
}

void RelativeLockTime::setTime(const LockTimeCounter &time) {
    _units = Units::Time;
    _counter = time;
}

uint32_t RelativeLockTime::toSequenceNumber() const {
    uint32_t seq = _counter & 0x0000ffff;

    if(_units == Units::Time)
        return seq |= 0x00400000;
    else
        return seq;
}

RelativeLockTime RelativeLockTime::fromSequenceNumber(const uint32_t & seq) {
    LockTimeCounter counter = seq & 0x0000ffff;
    uchar type = (seq >> 16) & 0x40;
    if(type == 0)
        return RelativeLockTime::fromBlockUnits(counter);
    else
        return RelativeLockTime::fromTimeUnits(counter);
}

uchar_vector RelativeLockTime::toScriptData() const {
    uint32_t value = 0x0000ffff & _counter;

    if(_units == Units::Time)
        value |= 0x00400000;

    return Coin::serializeScriptNum(value);
}

RelativeLockTime RelativeLockTime::fromScriptData(const uchar_vector &data) {
    if(data.size() != 3) {
        throw std::runtime_error("Invalid LockTime data");
    }

    // decode the locktime
    int32_t decodedLockTime = Coin::deserializeScriptNum(data);

    if(decodedLockTime < 0) {
        throw std::runtime_error("locktime is negative!");
    }

    LockTimeCounter counter = decodedLockTime & 0x0000ffff;
    uchar locktimeType = (decodedLockTime >> 16) & 0x40;

    if(locktimeType == 0)
        return RelativeLockTime::fromBlockUnits(counter);
    else
        return RelativeLockTime::fromTimeUnits(counter);

}

RelativeLockTime::LockTimeCounter RelativeLockTime::secondsToCounter(const std::chrono::seconds &seconds) {
    return seconds.count() >> 9;
}

std::chrono::seconds RelativeLockTime::counterToSeconds(const LockTimeCounter &counter) {
    uint32_t seconds = counter << 9;
    return std::chrono::seconds(seconds);
}

}
