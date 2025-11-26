//
// Created by D412 on 2025-08-13.
//

#ifndef TYPES_H
#define TYPES_H
#define MAX_COMPONENTS 32
#define MAX_ENTITIES 1000
#include <bitset>
#include <typeinfo>
#include <typeindex>
#include <cstdint>
#include <stdexcept>
#include <array>


using EntityID = int64_t;
using Signature = std::bitset<MAX_COMPONENTS>;
using ComponentType = std::uint8_t;

#endif //TYPES_H
