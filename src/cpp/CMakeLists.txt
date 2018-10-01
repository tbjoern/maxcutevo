set(target maxcut-benchmark)

set(sources
    Batch.cpp
    Benchmark.cpp
    Runner.cpp
    main.cpp
    UnifAlgorithm.cpp
    AnnealingAlgorithm.cpp
    PMUTAlgorithm.cpp
)

set(includes
    Algorithm.hpp
    Batch.hpp
    Benchmark.hpp
    Runner.hpp
    UnifAlgorithm.hpp
    types.hpp
    AnnealingAlgorithm.hpp
    PMUTAlgorithm.hpp
)

set(libraries)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
