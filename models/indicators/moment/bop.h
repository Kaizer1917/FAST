#ifndef BINAPI_MODELS_INDICATORS_MOMENT_BOP_H_
#define BINAPI_MODELS_INDICATORS_MOMENT_BOP_H_

#include <vector>
#include <string>
#include <memory>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

class BOP {
public:
    // Constructor
    BOP(double scalar = 1.0,
        bool use_talib = true,
        int offset = 0);

    // Calculate BOP
    std::vector<double> calculate(const std::vector<double>& open,
                                const std::vector<double>& high,
                                const std::vector<double>& low,
                                const std::vector<double>& close);

    // Static factory method
    static std::shared_ptr<BOP> create(double scalar = 1.0,
                                     bool use_talib = true,
                                     int offset = 0);

    // Getters
    std::string get_name() const;
    std::string get_category() const;
    double get_scalar() const;
    int get_offset() const;
    bool get_use_talib() const;

private:
    double scalar_;
    bool use_talib_;
    int offset_;
    std::string name_;
    std::string category_;

    std::vector<double> verify_series(const std::vector<double>& series) const;
    std::vector<double> non_zero_range(const std::vector<double>& a, 
                                     const std::vector<double>& b) const;
};

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi

#endif // BINAPI_MODELS_INDICATORS_MOMENT_BOP_H_