#!/bin/bash

set -e # Exit if any command fails

repo_root=$(git rev-parse --show-toplevel)
build_dir="build"

pushd $repo_root 1> /dev/null
    case "$1" in
        -c|--clean)
            echo "Cleaning build directory ..."
            rm -rf "$build_dir"
            exit 0
            ;;
        -b|--build)
            echo "Building project..."
            if  [ ! -d "$build_dir" ]; then
                mkdir "$build_dir"
            fi

            cd "$build_dir"
            cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ ..
            cmake --build . --parallel $(nproc)
            ;;
        -r|--run)
            echo "Running project..."
            exec "./$build_dir/Debug/bin/ChurchillMagnum"
            ;;
    esac

popd 1> /dev/null
