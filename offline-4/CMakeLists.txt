cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 20)

project(offline-4)

add_executable(offline-4
    "src/main.cpp"
    "src/student.cpp"
    "src/group.cpp"
    "src/staff.cpp"
    "src/individual.cpp"
    "src/timer.cpp"
    "src/random.cpp"
)