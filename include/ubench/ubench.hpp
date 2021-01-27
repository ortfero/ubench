/* This file is part of ubench library
 * Copyright 2020-2021 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <vector>


namespace ubench {


    enum class result_code { ok, optimized, debug, unstable };


    inline char const *describe(result_code rc) noexcept {
        switch(rc) {
        case result_code::ok: return "ok";
        case result_code::optimized: return "optimized";
        case result_code::debug: return "debug";
        default: return "unknown";
        }
    }


    using result_time = std::chrono::duration<double, std::nano>;


    struct result {
        result_code code {result_code::ok};
        result_time time {0.};

    };   // result


    template<typename Stream>
    Stream &operator<<(Stream &os, result const &r) noexcept {
        char time[64];
        std::snprintf(time, sizeof(time), "%.1f", r.time.count());
        time[63] = '\0';
        os << time << " ns";
        if(r.code == result_code::ok)
            return os;
        os << " (probably " << describe(r.code) << ')';
        return os;
    }


#ifdef _MSC_VER
#    define UBENCH_NOINLINE __declspec(noinline)
#else
#    define UBENCH_NOINLINE __attribute__(noinline)
#endif


    template<size_t max_samples = 30, typename F>
    result UBENCH_NOINLINE run(F &&f) {
        static_assert(max_samples % 2 == 0, "max_samples should be even");

        using namespace std;
        using namespace std::chrono;

        constexpr auto max_run_count = 1000000u;
        constexpr auto scale = 10u;

        auto start = steady_clock::now();
        f();
        auto elapsed = (steady_clock::now() - start).count();

        // determine the run_count
        auto run_count = 10u;
        for(; run_count <= max_run_count; run_count *= scale) {
            auto const last_elapsed = elapsed;
            start                   = steady_clock::now();
            for(auto i = 0u; i != run_count; ++i)
                f();
            elapsed      = (steady_clock::now() - start).count();
            auto const k = unsigned(std::round(double(elapsed) / last_elapsed));
            if(k == scale)
                break;
        }
        if(run_count > max_run_count)
            return {result_code::optimized,
                    result_time {elapsed * scale / run_count}};

        vector<double> samples;
        samples.reserve(max_samples + 1);
        samples.push_back(double(elapsed) / run_count);

        // get samples
        for(auto i = 0u; i != max_samples; ++i) {
            start = steady_clock::now();
            for(auto j = 0u; j != run_count; ++j)
                f();
            elapsed           = (steady_clock::now() - start).count();
            auto const sample = double(elapsed) / run_count;
            samples.push_back(sample);
        }

        std::sort(samples.begin(), samples.end());

        // return median
        result_time const rt {samples[max_samples / 2]};

#ifdef _MSC_VER
#    ifdef NDEBUG
        return {result_code::ok, rt};
#    else
        return {result_code::debug, rt};
#    endif   // NDEBUG
#else
#    ifdef __OPTIMIZE__
        return {result_code::ok, rt};
#    else
        return {result_code::debug, rt};
#    endif   // __OPTIMIZE__
#endif   // _MSC_VER
    }


}   // namespace ubench