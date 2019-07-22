set(target mathhelpertest)

set(sources
    MathHelperTest.cpp
)

set(includes
    ../cpp/MathHelper.hpp
)

set(libraries)

create_executable(${project} ${target} "${sources}" "${includes}" "${libraries}")
