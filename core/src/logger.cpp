#include "logger.hpp"

// Inicjalizacja statycznego członka klasy
std::shared_ptr<spdlog::logger> Logger::logger = nullptr;

void Logger::init() {
    // Ustawienie formatu logów
    spdlog::set_pattern("%Y-%m-%d %H:%M:%S [%^%l%$] %v");

    // Utworzenie sinków
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs.txt", true);

    // Utworzenie loggera z dwoma sinkami
    // spdlog::sinks_init_list sink_list = { console_sink, file_sink };
    spdlog::sinks_init_list sink_list = { console_sink };
    logger = std::make_shared<spdlog::logger>("SQLame", sink_list);

    // Ustawienie poziomu logowania
    logger->set_level(spdlog::level::trace);
    spdlog::set_default_logger(logger);
}

std::shared_ptr<spdlog::logger>& Logger::get_logger() {
    return logger;
}
