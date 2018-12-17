set(target maxcut-benchmark)

set(sources
    Algorithm.cpp
    Batch.cpp
    Benchmark.cpp
    main.cpp
    MathHelper.hpp
    # ActivityAlgorithm.cpp
    # ActivityDeterministicAlgorithm.cpp
    # AnnealingAlgorithm.cpp
    # FMUTAlgorithm.cpp
    # GreedyAlgorithm.cpp
    # GreedyActivityAlgorithm.cpp
    # GreedyPMUTAlgorithm.cpp
    # PMUTAlgorithm.cpp
    # UnifActivityAlgorithm.cpp
    UnifAlgorithm.cpp
)

set(includes
    Algorithm.hpp
    Batch.hpp
    Benchmark.hpp
    types.hpp
    MathHelper.hpp
    # ActivityAlgorithm.hpp
    # ActivityDeterministicAlgorithm.hpp
    # AnnealingAlgorithm.hpp
    # FMUTAlgorithm.hpp
    # GreedyAlgorithm.hpp
    # GreedyActivityAlgorithm.hpp
    # GreedyPMUTAlgorithm.hpp
    # PMUTAlgorithm.hpp
    # UnifActivityAlgorithm.hpp
    UnifAlgorithm.hpp
)

set(libraries
    stdc++fs
    Threads::Threads
)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
