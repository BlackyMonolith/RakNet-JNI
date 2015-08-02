set ( ARCHITECTURE_CHECK_CODE "
    #if defined(__ia64) || defined(__itanium__) || defined(_M_IA64)
        // Intel Itanium
        #error cmake_ARCH x64
    #elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
        // PowerPC
        #if defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || \\
            defined(__64BIT__)     || defined(_LP64)     || defined(__LP64__)
            #error cmake_ARCH x64
        #else
            #error cmake_ARCH x86
        #endif
    #elif defined(__x86_64__) || defined(_M_X64)
        // x86 64-Bit
        #error cmake_ARCH x64
    #elif defined(__i386) || defined(_M_IX86)
        // x86 32-Bit
        #error cmake_ARCH x86
    #else
        // Default back to 32-Bit
        #error cmake_ARCH x86
    #endif
" )

function ( target_architecture output_var )
    file ( WRITE "${CMAKE_BINARY_DIR}/raknet_jni_archdetect.c" "${ARCHITECTURE_CHECK_CODE}" )

    enable_language ( C )

    try_run (
        run_result_unused
        compile_result_unused
        "${CMAKE_BINARY_DIR}"
        "${CMAKE_BINARY_DIR}/raknet_jni_archdetect.c"
        COMPILE_OUTPUT_VARIABLE ARCH
        CMAKE_FLAGS CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    )

    string ( REGEX MATCH "cmake_ARCH ([a-zA-Z0-9_]+)" ARCH "${ARCH}" )
    string ( REPLACE "cmake_ARCH " "" ARCH "${ARCH}" )

    if ( NOT ARCH )
        set ( ARCH unknown )
    endif ( NOT ARCH )

    message ( "Detected architecture: ${ARCH}" )

    set ( ${output_var} "${ARCH}" PARENT_SCOPE )
endfunction()