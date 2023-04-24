//
// Created by Pew on 20-04-2023.
//
#include "../include/logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>


using namespace NAMESPACE;

bool NAMESPACE::setupLogger() {

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    spdlog::set_default_logger(std::make_shared<spdlog::logger>("main_logger", console_sink));
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::trace);

    return true;
}
