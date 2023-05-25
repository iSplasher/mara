//
// Created by Pew on 23-04-2023.
//

#include "../include/utility.h"

using namespace NAMESPACE;

std::pair<unsigned long, unsigned long> NAMESPACE::contegiousText(std::string_view text, unsigned long start_index) {
    unsigned long start = start_index;
    unsigned long end = start_index;

    // left
    while (start > 0 && text[start] != ' ') {
        start--;
    }

    // right
    while (end < text.size() && text[end] != ' ') {
        end++;
    }

    // special case
    if (text[start] == ' ') start++;
    if (text[end] == ' ') end--;

    return {start, end};
}
