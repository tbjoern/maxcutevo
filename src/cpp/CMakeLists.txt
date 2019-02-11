
set(target maxcut-benchmark)

set(sources
    Batch.cpp
    Benchmark.cpp
    main.cpp
    MathHelper.hpp
    algorithm/Algorithm.cpp
    algorithm/ActivityAlgorithm.cpp
    algorithm/Greedy.cpp
    algorithm/PMUT.cpp
    algorithm/Unif.cpp
)

set(includes
    Batch.hpp
    Benchmark.hpp
    types.hpp
    MathHelper.hpp
    algorithm/Algorithm.hpp
    algorithm/ActivityAlgorithm.hpp
    algorithm/Greedy.hpp
    algorithm/PMUT.hpp
    algorithm/Unif.hpp
)

set(libraries
    stdc++fs
    Threads::Threads
)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
