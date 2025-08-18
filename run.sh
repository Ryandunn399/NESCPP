#!/bin/bash

BUILD_DIR="build"
TARGET=""
RUN_AFTER=true
BUILD_ALL=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --tests)
            TARGET="NESCPPTests"
            shift
            ;;
        --main)
            TARGET="NESCPP"
            shift
            ;;
        --build-only)
            RUN_AFTER=false
            shift
            ;;
        --build-all)
            BUILD_ALL=true
            shift
            ;;
        --clean)
            echo "Cleaning build directory..."
            rm -rf $BUILD_DIR/*
            cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B $BUILD_DIR
            shift
            ;;
        --help)
            echo "Usage: $0 [OPTIONS]"
            echo ""
            echo "Target Options:"
            echo "  --tests      Build and run tests"
            echo "  --main       Build and run main program (default)"
            echo ""
            echo "Build Options:"
            echo "  --build-only Build but don't run"
            echo "  --build-all  Build entire project (cmake --build .) then run target"
            echo "  --clean      Clean build directory and reconfigure"
            echo "  --help       Show this help message"
            echo ""
            echo "Examples:"
            echo "  $0                          # Build and run main"
            echo "  $0 --tests                  # Build and run tests" 
            echo "  $0 --tests --build-all      # Full build then run tests"
            echo "  $0 --main --build-only      # Just build main, don't run"
            echo "  $0 --clean --tests          # Clean build then run tests"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

# Default to main if no target specified
if [ -z "$TARGET" ]; then
    TARGET="NESCPP"
    fi
    
    # Check if build directory exists
    if [ ! -d "$BUILD_DIR" ]; then
        echo "Build directory doesn't exist. Creating and configuring..."
        cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -S . -B $BUILD_DIR
        fi
        
        # Build phase
        if [ "$BUILD_ALL" = true ]; then
            echo "Building entire project..."
            cmake --build $BUILD_DIR
            if [ $? -ne 0 ]; then
                echo "Build failed!"
                exit 1
                fi
            else
            echo "Building target: $TARGET..."
            cmake --build $BUILD_DIR --target $TARGET
            if [ $? -ne 0 ]; then
                echo "Build failed!"
                exit 1
                fi
                fi
                
                # Run phase
                if [ "$RUN_AFTER" = true ]; then
                    echo ""
                    echo "Running $TARGET..."
                    echo "----------------------------------------"
                    ./$BUILD_DIR/$TARGET
                else
                echo "Build completed successfully. Executable: $BUILD_DIR/$TARGET"
                fi
