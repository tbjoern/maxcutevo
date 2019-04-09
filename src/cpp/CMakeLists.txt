
set(target maxcut-benchmark)

set(sources
    Batch.cpp
    Graph.cpp
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
    Graph.hpp
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
    nlohmann_json::nlohmann_json
)

if(OpenMP_CXX_FOUND)
    set(libraries ${libraries} OpenMP::OpenMP_CXX)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
