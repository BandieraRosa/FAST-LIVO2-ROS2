#ifndef FAST_LIVO_LOG_PATHS_H
#define FAST_LIVO_LOG_PATHS_H

#include <cstdlib>
#include <string>

// Keep the upstream default while allowing each run to own its output directory.
inline std::string livo_log_directory()
{
  const char *configured = std::getenv("LIVO_LOG_DIR");
  std::string directory = configured && *configured ? configured : std::string(ROOT_DIR) + "Log";
  if (directory.back() != '/') directory += '/';
  return directory;
}

#endif
