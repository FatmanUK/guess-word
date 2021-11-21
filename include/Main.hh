#pragma once

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <sstream>
using std::istringstream;
using std::ostringstream;

#include <iostream>
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

#include <fstream>
using std::ifstream;

#include <algorithm>
using std::count;

#include <system_error>
using std::runtime_error;
using std::exception;

#include <random>
using std::default_random_engine;
using std::uniform_int_distribution;

#include <chrono>
using std::chrono::high_resolution_clock;

#include <cstring>
using std::memset;

#include "docopt.h"

#ifndef CONFDIR
        #error CONFDIR should be defined by the build system. Check \
                your build config.
#endif

const unsigned short ver_maj{0};
const unsigned short ver_min{1};
const unsigned short ver_rel{0};

