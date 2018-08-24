set(target maxcut-benchmark)

set(includes
    AlgorithmFactory.hpp
    Algorithm.hpp
    Batch.hpp
    Benchmark.hpp
    CMakeLists.txt
    Runner.hpp
    types.hpp
)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
