//
// Created by Pew on 20-04-2023.
//

#pragma once

#include <spdlog/spdlog.h>

#ifdef ENV_TEST
#pragma message("ENV_TEST is defined, logger will use test::log instead of spdlog")

#include <iostream>
#include <boost/ut.hpp>

namespace test = boost::ut;

static auto spdlog_formatter = spdlog::pattern_formatter();
#endif


#include "common.h"

#include <string_view>
#include <format>

NAMESPACE_BEGIN

template<typename T, typename = std::void_t<> >
inline constexpr bool HAS_TO_STRING = false;
template<typename T>
inline constexpr bool HAS_TO_STRING<T, std::void_t<decltype(std::declval<T>().toString())>> = true;


struct logger {
    // Log levels supported by spdlog
    enum class Level {
        trace,
        debug,
        info,
        warn,
        error,
        critical
    };

    // Log a message with a specific log level
    template<typename... T>
    static void log(Level level, std::string_view msg, T &&...args) {
        const auto message = std::vformat(msg, std::make_format_args(args...));

#ifdef ENV_TEST
        spdlog::memory_buf_t buf;
        spdlog_formatter.format(spdlog::details::log_msg("main_logger", spdlog::level::info, message), buf);
        std::cout << fmt::to_string(buf) << std::endl;
        return;
#endif

        switch (level) {
            case Level::trace:
                spdlog::trace(message);
                break;
            case Level::debug:
                spdlog::debug(message);
                break;
            case Level::info:
                spdlog::info(message);
                break;
            case Level::warn:
                spdlog::warn(message);
                break;
            case Level::error:
                spdlog::error(message);
                break;
            case Level::critical:
                spdlog::critical(message);
                break;
            default:
                spdlog::info(message);
        }
    }

    // Shorthand methods for specific log levels

    template<typename... T>
    static void trace(T &&...args) {

        log(Level::trace, std::forward<T>(args)...);
    }

    template<typename... T>
    static void debug(T &&...args) {
        log(Level::debug, std::forward<T>(args)...);
    }

    template<typename... T>
    static void info(T &&...args) {
        log(Level::info, std::forward<T>(args)...);
    }

    template<typename... T>
    static void warn(T &&...args) {
        log(Level::warn, std::forward<T>(args)...);
    }

    template<typename... T>
    static void error(T &&...args) {
        log(Level::error, std::forward<T>(args)...);
    }

    template<typename... T>
    static void critical(T &&...args) {
        log(Level::critical, std::forward<T>(args)...);
    }
};

bool setupLogger();


NAMESPACE_END