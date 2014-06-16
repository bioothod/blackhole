#pragma once

#include <boost/assert.hpp>

#include "blackhole/logger.hpp"

namespace blackhole {

template<class Logger>
wrapper_t<Logger>::wrapper_t(logger_type& log, log::attributes_t&& attributes) :
    log(log),
    attributes(attributes)
{
    BOOST_ASSERT(!this->attributes.empty());
}

template<class Logger>
log::record_t
wrapper_t<Logger>::open_record() const {
    return log.open_record(attributes);
}

template<class Logger>
log::record_t
wrapper_t<Logger>::open_record(log::attribute_pair_t attribute) const {
    log::attributes_t attributes = this->attributes;
    attributes.insert(attribute);
    return log.open_record(std::move(attributes));
}

template<class Logger>
log::record_t
wrapper_t<Logger>::open_record(log::attributes_t attributes) const {
    log::attributes_t attributes_ = this->attributes;
    attributes_.insert(attributes.begin(), attributes.end());
    return log.open_record(std::move(attributes_));
}

template<class Logger>
template<typename Level>
typename std::enable_if<
    std::is_same<Logger, verbose_logger_t<Level>>::value,
    log::record_t
>::type
wrapper_t<Logger>::open_record(Level level) const {
    log::attributes_t attributes = this->attributes;
    attributes.insert(keyword::severity<Level>() = level);
    return log.open_record(std::move(attributes));
}

template<class Logger>
void
wrapper_t<Logger>::push(log::record_t&& record) const {
    log.push(std::move(record));
}

} // namespace blackhole