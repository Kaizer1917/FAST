#include "bias.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

Bias::Bias(int length,
           const std::string& mamode,
           int offset)
    : length_(length > 0 ? length : 26)
    , mamode_(mamode)
    , offset_(offset)
    , category_("momentum") {
    name_ = "BIAS_" + std::to_string(length_);
}

std::shared_ptr<Bias> Bias::create(int length,
                                  const std::string& mamode,
                                  int offset) {
    return std::make_shared<Bias>(length, mamode, offset);
}

std::vector<double> Bias::calculate(const std::vector<double>& close) {
    auto series = verify_series(close, length_);
    if (series.empty()) return {};

    // Calculate Result
    std::vector<double> bma = ma(series, length_);
    std::vector<double> bias_result(series.size(), 0.0);
    
    for (size_t i = 0; i < series.size(); ++i) {
        if (i < bma.size() && bma[i] != 0) {
            bias_result[i] = (series[i] / bma[i]) - 1;
        }
    }

    // Offset
    if (offset_ != 0) {
        std::vector<double> shifted_bias(bias_result.size(), 0.0);
        std::copy(bias_result.begin(), 
                 bias_result.end() - offset_, 
                 shifted_bias.begin() + offset_);
        bias_result = shifted_bias;
    }

    return bias_result;
}

std::vector<double> Bias::ma(const std::vector<double>& close, int length) const {
    std::vector<double> result(close.size(), 0.0);
    
    if (mamode_ == "sma") {
        for (size_t i = length - 1; i < close.size(); ++i) {
            result[i] = std::accumulate(close.begin() + i - length + 1,
                                      close.begin() + i + 1,
                                      0.0) / length;
        }
    }
    else if (mamode_ == "ema") {
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
    
    return result;
}

std::vector<double> Bias::verify_series(const std::vector<double>& close, int min_length) const {
    if (close.size() < min_length) {
        throw std::invalid_argument("Input series length is less than " + std::to_string(min_length));
    }
    return close;
}

// Getters
std::string Bias::get_name() const { return name_; }
std::string Bias::get_category() const { return category_; }
int Bias::get_length() const { return length_; }
std::string Bias::get_ma_mode() const { return mamode_; }
int Bias::get_offset() const { return offset_; }

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi