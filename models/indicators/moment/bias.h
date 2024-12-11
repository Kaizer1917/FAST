#ifndef BINAPI_MODELS_INDICATORS_MOMENT_BIAS_H_
#define BINAPI_MODELS_INDICATORS_MOMENT_BIAS_H_

#include <vector>
#include <string>
#include <memory>

namespace binapi {
namespace models {
namespace indicators {
namespace moment {

class Bias {
public:
    // Constructor
    Bias(int length = 26,
         const std::string& mamode = "sma",
         int offset = 0);

    // Calculate Bias
    std::vector<double> calculate(const std::vector<double>& close);

    // Static factory method
    static std::shared_ptr<Bias> create(int length = 26,
                                      const std::string& mamode = "sma",
                                      int offset = 0);

    // Getters
    std::string get_name() const;
    std::string get_category() const;
    int get_length() const;
    std::string get_ma_mode() const;
    int get_offset() const;

private:
    int length_;
    std::string mamode_;
    int offset_;
    std::string name_;
    std::string category_;

    std::vector<double> ma(const std::vector<double>& close, int length) const;
    std::vector<double> verify_series(const std::vector<double>& close, int min_length) const;
};

} // namespace moment
} // namespace indicators
} // namespace models
} // namespace binapi

#endif // BINAPI_MODELS_INDICATORS_MOMENT_BIAS_H_