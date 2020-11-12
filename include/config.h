// config.h

#pragma once

// === psi46test version ====================================================

#define TITLE        "ROC4sens Tester"
#define VERSION      "V0.3"
#define TIMESTAMP    "15.6.2020"


// === set profiling options ================================================
// if defined a profiling infos are collected during execution
// and a report is created after termination (only Windows)

// #define ENABLE_PROFILING

// add profiling infos for rpc calls (ENABLE_PROFILING must be defined)

// #define ENABLE_RPC_PROFILING


// === thread safe CTestboard class =========================================
// if defined -> CTestboard is thread safe (Boost thread library needed)

// #define ENABLE_MULTITHREADING

