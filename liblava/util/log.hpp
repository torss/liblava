// file      : liblava/util/log.hpp
// copyright : Copyright (c) 2018-present, Lava Block OÜ
// license   : MIT; see accompanying LICENSE file

#pragma once

#include <liblava/core/types.hpp>
#include <liblava/core/version.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/file_sinks.h>

namespace lava {

using logger = std::shared_ptr<spdlog::logger>;

inline logger log(name name = _lava_) { return spdlog::get(name); }

inline string to_string(string_ref id, string_ref name) {

    return fmt::format("{} | {}", str(id), str(name));
}

inline string to_string(internal_version const& version) {

    return fmt::format("{}.{}.{}", version.major, version.minor, version.patch);
}

inline name to_string(version_stage stage) {

    switch (stage) {

        case version_stage::preview: return "preview";
        case version_stage::alpha: return "alpha";
        case version_stage::beta: return "beta";
        case version_stage::rc: return "rc";
        default:
            return "";
    }
}

inline string to_string(version const& version) {

    string stage_str = to_string(version.stage);
    if ((version.rev > 1) && (version.stage != version_stage::release))
        stage_str += fmt::format(" {}", version.rev);

    if (version.release == 0)
        return fmt::format("{} {}", version.year, str(stage_str));
    else
        return fmt::format("{}.{} {}", version.year, version.release, str(stage_str));
}

constexpr name _lava_log_file_ = "lava.log";

struct log_config {

    name logger = _lava_;
    name file = _lava_log_file_;

    i32 level = -1;
    bool debug = false;
};

inline void setup_log(log_config config = {}) {

    if (config.debug) {

        auto log = spdlog::stdout_color_mt(config.logger);
        log->set_level((config.level < 0) ? spdlog::level::debug : (spdlog::level::level_enum)config.level);
    }
    else {

        auto log = spdlog::basic_logger_mt(config.logger, config.file);
        log->set_level((config.level < 0) ? spdlog::level::warn : (spdlog::level::level_enum)config.level);
    }
}

} // lava
