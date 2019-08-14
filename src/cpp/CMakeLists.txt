
set(target maxcut-benchmark)

set(sources
    AdjList.cpp
    AlgorithmFactory.cpp
    Batch.cpp
    Graph.cpp
    main.cpp
    MathHelper.cpp
    algorithm/Algorithm.cpp
    algorithm/ActivityAlgorithm.cpp
    algorithm/FMUT.cpp
    algorithm/Greedy.cpp
    algorithm/GreedyActivity.cpp
    algorithm/PMUT.cpp
    algorithm/PMUTActivity.cpp
    algorithm/Random.cpp
    algorithm/Unif.cpp
    algorithm/UnifSigmoid.cpp
)

set(includes
    AdjList.hpp
    AlgorithmFactory.hpp
    Batch.hpp
    Graph.hpp
    types.hpp
    MathHelper.hpp
    algorithm/Algorithm.hpp
    algorithm/ActivityAlgorithm.hpp
    algorithm/FMUT.hpp
    algorithm/Greedy.hpp
    algorithm/GreedyActivity.hpp
    algorithm/PMUT.hpp
    algorithm/PMUTActivity.hpp
    algorithm/Random.hpp
    algorithm/Unif.hpp
    algorithm/UnifSigmoid.hpp
)

set(libraries
    stdc++fs
    Threads::Threads
    nlohmann_json::nlohmann_json
)

if(OpenMP_CXX_FOUND)
    set(libraries ${libraries} OpenMP::OpenMP_CXX)
endif(OpenMP_CXX_FOUND)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
