#pragma once

#include <cstdint>
#include <stdexcept>
#include <cstdlib>
#include <fstream>
#include <string>


namespace sunnypilot::hardware {

enum class HardwareProfile {
  STANDARD,
  C3XL,
};

inline HardwareProfile get_hardware_profile() {
  static const HardwareProfile cached = [] {
    std::string value;
    const char *env = std::getenv("SUNNYPILOT_HARDWARE_PROFILE");
    if (env && *env) value = env;
    else {
      const char *path = std::getenv("SUNNYPILOT_HARDWARE_PROFILE_FILE");
      std::ifstream(path ? path : "/data/hardware_profile") >> value;
    }
    return value == "c3xl" ? HardwareProfile::C3XL : HardwareProfile::STANDARD;
  }();
  return cached;
}

inline bool is_c3xl() {
  return get_hardware_profile() == HardwareProfile::C3XL;
}

inline uint8_t resolve_internal_panda_type(uint8_t raw_type) {
  if (!is_c3xl()) {
    return raw_type;
  }

  constexpr uint8_t PANDA_TYPE_UNKNOWN = 0U;
  constexpr uint8_t PANDA_TYPE_TRES = 9U;
  if ((raw_type == PANDA_TYPE_UNKNOWN) || (raw_type == PANDA_TYPE_TRES)) {
    return PANDA_TYPE_TRES;
  }
  throw std::runtime_error("C3XL internal SPI Panda reported an unexpected hardware type");
}

}  // namespace sunnypilot::hardware
