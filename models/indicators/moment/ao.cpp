#include "ao.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

AO::AO(int fast_period,
       int slow_period,
       int offset)
    : fast_period_(fast_period > 0 ? fast_period : 5)
    , slow_period_(slow_period > 0 ? slow_period : 34)
    , offset_(offset)
    , category_("momentum") {
    
    if (slow_period_ < fast_period_) {
        std::swap(fast_period_, slow_period_);
    }
    
    name_ = "AO_" + std::to_string(fast_period_) + "_" + std::to_string(slow_period_);
}

std::shared_ptr<AO> AO::create(int fast_period,
                              int slow_period,
                              int offset) {
    return std::make_shared<AO>(fast_period, slow_period, offset);
}

std::vector<double> AO::calculate(const std::vector<double>& high,
                                const std::vector<double>& low) {
    size_t length = std::max(fast_period_, slow_period_);
    std::vector<double> high_verified = verify_series(high, length);
    std::vector<double> low_verified = verify_series(low, length);

    // Calculate median price
    std::vector<double> median_price(high_verified.size());
    for (size_t i = 0; i < high_verified.size(); ++i) {
        median_price[i] = 0.5 * (high_verified[i] + low_verified[i]);
    }

    // Calculate SMAs
    std::vector<double> fast_sma = sma(median_price, fast_period_);
    std::vector<double> slow_sma = sma(median_price, slow_period_);

    // Calculate AO
    std::vector<double> ao_values(fast_sma.size());
    for (size_t i = 0; i < fast_sma.size(); ++i) {
        ao_values[i] = fast_sma[i] - slow_sma[i];
    }

    // Apply offset if needed
    if (offset_ != 0) {
        std::vector<double> ao_offset(ao_values.size(), 0.0);
        std::copy(ao_values.begin(),
                 ao_values.end() - offset_,
                 ao_offset.begin() + offset_);
        ao_values = ao_offset;
    }

    return ao_values;
}

std::vector<double> AO::sma(const std::vector<double>& data, int period) const {
    std::vector<double> result(data.size(), 0.0);
    if (period <= 0 || period > data.size()) {
        throw std::invalid_argument("Invalid period for SMA");
    }
    
    for (size_t i = period - 1; i < data.size(); ++i) {
        result[i] = std::accumulate(data.begin() + i - period + 1,
                                  data.begin() + i + 1,
                                  0.0) / period;
    }
    
    return result;
}

std::vector<double> AO::verify_series(const std::vector<double>& series, size_t length) const {
    if (series.size() < length) {
        throw std::invalid_argument("Series length is less than " + std::to_string(length));
    }
    return series;
}

// Getters
std::string AO::get_name() const { return name_; }
std::string AO::get_category() const { return category_; }
int AO::get_fast_period() const { return fast_period_; }
int AO::get_slow_period() const { return slow_period_; }
int AO::get_offset() const { return offset_; }

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi