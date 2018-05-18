#!/bin/bash

set -e

cd "$( dirname "${BASH_SOURCE[0]}" )"

clang-format -style='{BasedOnStyle: WebKit, IndentWidth: 4, ContinuationIndentWidth: 8, SpacesBeforeTrailingComments: 2, PointerAlignment: Right}' -i lib/*.[ch]
