#include <iostream>
#include "numeric"
#include "chrono"
#include <time.h>
#include <Eigen/Dense>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/ostream.h"

namespace spd = spdlog;
constexpr int MAX_MATRIX_SIZE = 100;
constexpr int TOTAL_ITERATIONS = 1000;

double_t calc_time(int32_t size);

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    auto console = spd::stdout_color_mt("console");
    console->info("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
    console->info("Eigen Benchmarks (in ns)");
    console->info("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");

    std::map<int32_t, double_t> averages;
    std::map<int32_t, double_t> maxs;
    std::map<int32_t, double_t> mins;

    for (int i = 0; i < MAX_MATRIX_SIZE; ++i)
    {
        std::vector<double_t> timings;
        for (int j = 0; j < TOTAL_ITERATIONS; ++j)
        {
            auto time_taken = calc_time(i + 1);
            timings.push_back(time_taken);
        }

        auto avg = (std::accumulate(timings.begin(), timings.end(), 0.0) / timings.size());
        auto minimum = *std::min_element(std::begin(timings), std::end(timings));
        auto maximum = *std::max_element(std::begin(timings), std::end(timings));
        console->info("{} -> Mean: {} Min: {} Max: {}", i + 1, avg, minimum, maximum);
    }

    console->info("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
}

double_t calc_time(int32_t size)
{
    Eigen::MatrixXd m = Eigen::MatrixXd::Random(size, size);
    auto invertible = m * m.transpose();
    auto begin = std::chrono::steady_clock::now();
    Eigen::MatrixXd m_inv = m.inverse();
    auto end = std::chrono::steady_clock::now();
    auto time_diff = end - begin;
    auto time_taken = static_cast<double_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(time_diff).count());
    return time_taken;
}
