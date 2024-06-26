#pragma once

#include "logger.hpp"

#define LOG_TRACE(...)    SPDLOG_LOGGER_CALL(Logger::get_logger().get(), spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)    SPDLOG_LOGGER_CALL(Logger::get_logger().get(), spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_LOGGER_CALL(Logger::get_logger().get(), spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)     SPDLOG_LOGGER_CALL(Logger::get_logger().get(), spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_LOGGER_CALL(Logger::get_logger().get(), spdlog::level::err, __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CALL(Logger::get_logger().get(), spdlog::level::critical, __VA_ARGS__)