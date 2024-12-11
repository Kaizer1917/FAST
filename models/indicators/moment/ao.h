#ifndef BINAPI_MODELS_INDICATORS_MOMENT_AO_H_
#define BINAPI_MODELS_INDICATORS_MOMENT_AO_H_

#include <vector>
#include <string>
#include <memory>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

class AO {
public:
    // Constructor
    AO(int fast_period = 5,
       int slow_period = 34,
       int offset = 0);

    // Calculate AO
    std::vector<double> calculate(const std::vector<double>& high,
                                const std::vector<double>& low);

    // Static factory method
    static std::shared_ptr<AO> create(int fast_period = 5,
                                    int slow_period = 34,
                                    int offset = 0);

    // Getters
    std::string get_name() const;
    std::string get_category() const;
    int get_fast_period() const;
    int get_slow_period() const;
    int get_offset() const;

private:
    int fast_period_;
    int slow_period_;
    int offset_;
    std::string name_;
    std::string category_;

    std::vector<double> sma(const std::vector<double>& data, int period) const;
    std::vector<double> verify_series(const std::vector<double>& series, size_t length) const;
};

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi

#endif // BINAPI_MODELS_INDICATORS_MOMENT_AO_H_