set(target mathhelpertest)

set(sources
    MathHelperTest.cpp
    ../cpp/MathHelper.cpp
)

set(includes
    ../cpp/MathHelper.hpp
)

set(libraries)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
