//
// Created by Pew on 23-04-2023.
//

#pragma once

#include <utility>
#include <string_view>

#include "common.h"

NAMESPACE_BEGIN

/**
 * Returns a pair of indices that represent the start and end of the word at the given index.
 * @param text The text to search in.
 * @param start_index The index to start searching from.
 * @return A pair of indices that represent the start and end of the word at the given index.
 */
std::pair<unsigned long, unsigned long> contegiousText(std::string_view text, unsigned long start_index = 0);

NAMESPACE_END