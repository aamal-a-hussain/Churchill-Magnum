alias b := build
alias r := run
alias c := clean

cores := if os() == "macos" { `sysctl -n hw.ncpu` } else if os() == "linux" { `nproc` } else { "1" }
format_dirs := "src include"
format_exts := "-name '*.cpp' -o -name '*.c' -o -name '*.h' -o -name '*.hpp'"

default:
    @just --list

build build_type='Debug':
    @mkdir -p build
    cd build && cmake -S .. -B . -DCMAKE_BUILD_TYPE={{ build_type }} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    cd build && cmake --build . -j{{ cores }}

run :
    @./build/Debug/bin/ChurchillMagnum

clean:
    @rm -rf build 

format:
    find {{ format_dirs }} -type f \( {{ format_exts }} \) -exec clang-format --verbose -i {} +
