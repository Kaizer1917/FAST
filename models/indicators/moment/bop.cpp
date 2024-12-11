#include "bop.h"
#include <algorithm>
#include <stdexcept>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

BOP::BOP(double scalar,
         bool use_talib,
         int offset)
    : scalar_(scalar != 0 ? scalar : 1.0)
    , use_talib_(use_talib)
    , offset_(offset)
    , name_("BOP")
    , category_("momentum") {
}

std::shared_ptr<BOP> BOP::create(double scalar,
                                bool use_talib,
                                int offset) {
    return std::make_shared<BOP>(scalar, use_talib, offset);
}

std::vector<double> BOP::calculate(const std::vector<double>& open,
                                 const std::vector<double>& high,
                                 const std::vector<double>& low,
                                 const std::vector<double>& close) {
    // Validate Arguments
    verify_series(open);
    verify_series(high);
    verify_series(low);
    verify_series(close);

    // Calculate Result
    std::vector<double> bop;
    if (use_talib_) {
        // Placeholder for TA-Lib BOP function
        // TODO: Implement TA-Lib integration
        return std::vector<double>(close.size(), 0.0);
    }

    std::vector<double> high_low_range = non_zero_range(high, low);
    std::vector<double> close_open_range = non_zero_range(close, open);
    bop.resize(close.size());
    
    for (size_t i = 0; i < close.size(); ++i) {
        bop[i] = scalar_ * close_open_range[i] / high_low_range[i];
    }

    // Apply offset if needed
    if (offset_ != 0) {
        std::vector<double> shifted(bop.size(), 0.0);
        std::copy(bop.begin(),
                 bop.end() - offset_,
                 shifted.begin() + offset_);
        bop = shifted;
    }

    return bop;
}

std::vector<double> BOP::verify_series(const std::vector<double>& series) const {
    if (series.empty()) {
        throw std::invalid_argument("Series cannot be empty");
    }
    return series;
}

std::vector<double> BOP::non_zero_range(const std::vector<double>& a,
                                      const std::vector<double>& b) const {
    std::vector<double> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = (a[i] - b[i] != 0) ? (a[i] - b[i]) : 1; // Avoid division by zero
    }
    return result;
}

// Getters
std::string BOP::get_name() const { return name_; }
std::string BOP::get_category() const { return category_; }
double BOP::get_scalar() const { return scalar_; }
int BOP::get_offset() const { return offset_; }
bool BOP::get_use_talib() const { return use_talib_; }

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi