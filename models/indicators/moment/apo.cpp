#include "apo.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

APO::APO(int fast_period,
         int slow_period,
         const std::string& ma_mode,
         bool use_talib,
         int offset)
    : fast_period_(fast_period > 0 ? fast_period : 12)
    , slow_period_(slow_period > 0 ? slow_period : 26)
    , ma_mode_(ma_mode)
    , use_talib_(use_talib)
    , offset_(offset)
    , category_("momentum") {
    
    if (slow_period_ < fast_period_) {
        std::swap(fast_period_, slow_period_);
    }
    
    name_ = "APO_" + std::to_string(fast_period_) + "_" + std::to_string(slow_period_);
}

std::shared_ptr<APO> APO::create(int fast_period,
                                int slow_period,
                                const std::string& ma_mode,
                                bool use_talib,
                                int offset) {
    return std::make_shared<APO>(fast_period, slow_period, ma_mode, use_talib, offset);
}

std::vector<double> APO::calculate(const std::vector<double>& close) {
    auto series = verify_series(close, std::max(fast_period_, slow_period_));
    if (series.empty()) return {};

    std::vector<double> apo_result;
    if (use_talib_) {
        // Placeholder for TA-Lib APO function
        // TODO: Implement TA-Lib integration
        return {};
    }

    std::vector<double> fast_ma = ma(ma_mode_, series, fast_period_);
    std::vector<double> slow_ma = ma(ma_mode_, series, slow_period_);
    
    apo_result.resize(series.size());
    for (size_t i = 0; i < series.size(); ++i) {
        apo_result[i] = fast_ma[i] - slow_ma[i];
    }

    // Apply offset if needed
    if (offset_ != 0) {
        std::vector<double> shifted(apo_result.size(), 0.0);
        std::copy(apo_result.begin(),
                 apo_result.end() - offset_,
                 shifted.begin() + offset_);
        apo_result = shifted;
    }

    return apo_result;
}

std::vector<double> APO::ma(const std::string& ma_mode, const std::vector<double>& close, int length) const {
    std::vector<double> result(close.size(), 0.0);
    
    if (ma_mode == "sma") {
        for (size_t i = length - 1; i < close.size(); ++i) {
            result[i] = std::accumulate(close.begin() + i - length + 1,
                                      close.begin() + i + 1,
                                      0.0) / length;
        }
    }
    else if (ma_mode == "ema") {
        double multiplier = 2.0 / (length + 1);
        
        // Initialize first EMA with SMA
        if (close.size() >= length) {
            result[length-1] = std::accumulate(close.begin(),
                                             close.begin() + length,
                                             0.0) / length;
            
            // Calculate subsequent EMAs
            for (size_t i = length; i < close.size(); ++i) {
                result[i] = (close[i] - result[i-1]) * multiplier + result[i-1];
            }
        }
    }
    else if (ma_mode == "wma") {
        std::vector<double> weights(length);
        double weight_sum = 0;
        
        for (int i = 0; i < length; ++i) {
            weights[i] = length - i;
            weight_sum += weights[i];
        }
        
        for (size_t i = length - 1; i < close.size(); ++i) {
            double sum = 0;
            for (int j = 0; j < length; ++j) {
                sum += close[i - j] * weights[j];
            }
            result[i] = sum / weight_sum;
        }
    }
    
    return result;
}

std::vector<double> APO::verify_series(const std::vector<double>& close, int min_length) const {
    if (close.size() < min_length) {
        throw std::invalid_argument("Input series length is less than " + std::to_string(min_length));
    }
    return close;
}

// Getters
std::string APO::get_name() const { return name_; }
std::string APO::get_category() const { return category_; }
int APO::get_fast_period() const { return fast_period_; }
int APO::get_slow_period() const { return slow_period_; }
std::string APO::get_ma_mode() const { return ma_mode_; }
int APO::get_offset() const { return offset_; }

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi