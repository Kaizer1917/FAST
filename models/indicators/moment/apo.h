#ifndef BINAPI_MODELS_INDICATORS_MOMENT_APO_H_
#define BINAPI_MODELS_INDICATORS_MOMENT_APO_H_

#include <vector>
#include <string>
#include <memory>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

class APO {
public:
    // Constructor
    APO(int fast_period = 12,
        int slow_period = 26,
        const std::string& ma_mode = "sma",
        bool use_talib = true,
        int offset = 0);

    // Calculate APO
    std::vector<double> calculate(const std::vector<double>& close);

    // Static factory method
    static std::shared_ptr<APO> create(int fast_period = 12,
                                     int slow_period = 26,
                                     const std::string& ma_mode = "sma",
                                     bool use_talib = true,
                                     int offset = 0);

    // Getters
    std::string get_name() const;
    std::string get_category() const;
    int get_fast_period() const;
    int get_slow_period() const;
    std::string get_ma_mode() const;
    int get_offset() const;

private:
    int fast_period_;
    int slow_period_;
    std::string ma_mode_;
    bool use_talib_;
    int offset_;
    std::string name_;
    std::string category_;

    std::vector<double> ma(const std::string& ma_mode, const std::vector<double>& close, int length) const;
    std::vector<double> verify_series(const std::vector<double>& close, int min_length) const;
};

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi

#endif // BINAPI_MODELS_INDICATORS_MOMENT_APO_H_