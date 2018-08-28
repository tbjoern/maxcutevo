set(target maxcut-benchmark)

set(sources
    Batch.cpp
    Benchmark.cpp
    Runner.cpp
    main.cpp
)

set(includes
    Algorithm.hpp
    Batch.hpp
    Benchmark.hpp
    Runner.hpp
    types.hpp
)

set(libraries)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
