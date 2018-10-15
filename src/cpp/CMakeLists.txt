set(target maxcut-benchmark)

set(sources
    Algorithm.cpp
    Batch.cpp
    Benchmark.cpp
    Runner.cpp
    main.cpp
    UnifAlgorithm.cpp
    AnnealingAlgorithm.cpp
    PMUTAlgorithm.cpp
    FMUTAlgorithm.cpp
    ActivityAlgorithm.cpp
    MathHelper.hpp
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
    FMUTAlgorithm.hpp
    ActivityAlgorithm.hpp
    MathHelper.hpp
)

set(libraries
    stdc++fs
    Threads::Threads
)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
