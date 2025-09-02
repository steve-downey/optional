include_guard(GLOBAL)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_INIT
    "-stdlib=libc++ -Wall -Wextra "
    CACHE STRING
    "CXX_FLAGS"
    FORCE
)

set(CMAKE_CXX_FLAGS_DEBUG_INIT
    "-O0 -fno-inline -g3"
    CACHE STRING
    "C++ DEBUG Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELEASE_INIT
    "-Ofast -g0 -DNDEBUG"
    CACHE STRING
    "C++ Release Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT
    "-O3 -g -DNDEBUG"
    CACHE STRING
    "C++ RelWithDebInfo Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_TSAN_INIT
    "-O3 -g -fsanitize=thread"
    CACHE STRING
    "C++ TSAN Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_ASAN_INIT
    "-O3 -g -fsanitize=address,undefined,leak"
    CACHE STRING
    "C++ ASAN Flags"
    FORCE
)
set(CMAKE_CXX_FLAGS_GCOV_INIT
    "-O0 -fno-inline -g --coverage"
    CACHE STRING
    "C++ GCOV Flags"
    FORCE
)
set(CMAKE_LINKER_FLAGS_GCOV_INIT "--coverage" CACHE STRING "Linker GCOV Flags" FORCE)
