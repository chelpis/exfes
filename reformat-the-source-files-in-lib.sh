#!/bin/bash

set -e

cd "$( dirname "${BASH_SOURCE[0]}" )"

find lib -type f -name '*.[ch]' ! -name binomials.c -exec echo clang-format -style='{BasedOnStyle: WebKit, IndentWidth: 4, ContinuationIndentWidth: 8, SpacesBeforeTrailingComments: 2, PointerAlignment: Right}' -i {} ';'
