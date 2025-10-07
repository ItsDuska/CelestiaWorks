#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifdef CELESTIA_WORKS_EXPORTS
#define CELESTIA_WORKS __declspec(dllexport)
#else
#define CELESTIA_WORKS __declspec(dllimport)
#endif
#else
#define CELESTIA_WORKS __attribute__((visibility("default")))
#endif