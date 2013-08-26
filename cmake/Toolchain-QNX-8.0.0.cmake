#
# (C) Copyright 2009 Johns Hopkins University (JHU), All Rights
# Reserved.
# (C) Copyright 2013 Laszlo Papp <lpapp@kde.org>
#
# --- begin cisst license - do not edit ---
# 
# This software is provided "as is" under an open source license, with
# no warranty.  The complete license can be found in license.txt and
# http://www.cisst.org/cisst/license.txt.
# 
# --- end cisst license ---

SET(CMAKE_SYSTEM_NAME QNX)
SET(CMAKE_SYSTEM_VERSION 8.0.0)
# for device
#SET(CMAKE_SYSTEM_PROCESSOR armv7)
# for simulator
SET(CMAKE_SYSTEM_PROCESSOR x86)
SET(TOOLCHAIN QNX)

#SET(CMAKE_IMPORT_LIBRARY_SUFFIX ".a")

SET(CMAKE_SHARED_LIBRARY_PREFIX "lib")
SET(CMAKE_SHARED_LIBRARY_SUFFIX ".so")
SET(CMAKE_STATIC_LIBRARY_PREFIX "lib")
SET(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

IF(CMAKE_HOST_WIN32)
  SET(HOST_EXECUTABLE_SUFFIX ".exe")
ENDIF(CMAKE_HOST_WIN32)

FIND_PATH(QNX_HOST
  NAME usr/bin/qcc${HOST_EXECUTABLE_SUFFIX}
  PATHS $ENV{QNX_HOST} C:/QNX641/host/win32/x86
  NO_CMAKE_PATH
  NO_CMAKE_ENVIRONMENT_PATH
)
  
FIND_PATH(QNX_TARGET
  NAME usr/include/qnx_errno.h
  PATHS $ENV{QNX_TARGET} C:/QNX641/target/qnx6
  NO_CMAKE_PATH
  NO_CMAKE_ENVIRONMENT_PATH
)

IF(CMAKE_HOST_WIN32)
  FIND_PATH(QNX_CONFIGURATION
    NAME bin/qnxactivate.exe
    PATHS $ENV{QNX_CONFIGURATION}
    "C:/Program Files/QNX Software Systems/qconfig"
    NO_CMAKE_PATH
    NO_CMAKE_ENVIRONMENT_PATH
 )
ENDIF(CMAKE_HOST_WIN32)

SET(ENV{QNX_HOST} ${QNX_HOST})
SET(ENV{QNX_TARGET} ${QNX_TARGET})
IF(CMAKE_HOST_WIN32)
  SET(ENV{QNX_CONFIGURATION} ${QNX_CONFIGURATION})
  SET(ENV{PATH} "$ENV{PATH};${QNX_HOST}/usr/bin")
ENDIF(CMAKE_HOST_WIN32)

SET(CMAKE_MAKE_PROGRAM "${QNX_HOST}/usr/bin/make${HOST_EXECUTABLE_SUFFIX}"    CACHE PATH "QNX Make Program")
SET(CMAKE_SH           "${QNX_HOST}/usr/bin/sh${HOST_EXECUTABLE_SUFFIX}"      CACHE PATH "QNX shell Program")
SET(CMAKE_AR           "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ar${HOST_EXECUTABLE_SUFFIX}"      CACHE PATH "QNX ar Program")
SET(CMAKE_RANLIB       "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ranlib${HOST_EXECUTABLE_SUFFIX}"      CACHE PATH "QNX ranlib Program")
SET(CMAKE_NM           "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-nm${HOST_EXECUTABLE_SUFFIX}"      CACHE PATH "QNX nm Program")
SET(CMAKE_OBJCOPY      "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objcopy${HOST_EXECUTABLE_SUFFIX}" CACHE PATH "QNX objcopy Program")
SET(CMAKE_OBJDUMP      "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-objdump${HOST_EXECUTABLE_SUFFIX}" CACHE PATH "QNX objdump Program")
SET(CMAKE_LINKER       "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-ld"     CACHE PATH "QNX Linker Program")
SET(CMAKE_STRIP        "${QNX_HOST}/usr/bin/nto${CMAKE_SYSTEM_PROCESSOR}-strip${HOST_EXECUTABLE_SUFFIX}"   CACHE PATH "QNX Strip Program")

# I think this is qcc's bug but we need -fPIC for libtomcrypt
SET(DEFAULT_COMPILER_FLAGS "-fPIC")

# well only x86 or armv7
SET(QCC_TARET_ARG1 ${CMAKE_SYSTEM_PROCESSOR})
IF (CMAKE_SYSTEM_PROCESSOR STREQUAL "armv7")
  SET(QCC_TARET_ARG1 "${QCC_TARET_ARG1}le")
ENDIF()

SET(CMAKE_C_COMPILER ${QNX_HOST}/usr/bin/qcc${HOST_EXECUTABLE_SUFFIX})
SET(CMAKE_C_COMPILER_ARG1 " -Vgcc_nto${QCC_TARET_ARG1} -lang-c")
#SET(CMAKE_C_FLAGS "${DEFAULT_COMPILER_FLAGS}")
SET(CMAKE_C_FLAGS_DEBUG "-g")
SET(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

SET(CMAKE_CXX_COMPILER ${QNX_HOST}/usr/bin/qcc${HOST_EXECUTABLE_SUFFIX})
SET(CMAKE_CXX_COMPILER_ARG1 "-Vgcc_nto${QCC_TARET_ARG1} -lang-c++")
#SET(CMAKE_CXX_FLAGS "${DEFAULT_COMPILER_FLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

SET(CMAKE_ASM_COMPILER ${QNX_HOST}/usr/bin/qcc${HOST_EXECUTABLE_SUFFIX})
SET(CMAKE_ASM_COMPILER_ARG1 "-Vgcc_nto${QCC_TARET_ARG1} -lang-c")
#SET(CMAKE_ASM_FLAGS "${DEFAULT_COMPILER_FLAGS}")
SET(CMAKE_ASM_FLAGS_DEBUG "-g")
SET(CMAKE_ASM_FLAGS_MINSIZEREL "-Os -DNDEBUG")
SET(CMAKE_ASM_FLAGS_RELEASE "-O3 -DNDEBUG")
SET(CMAKE_ASM_FLAGS_RELWITHDEBINFO "-O2 -g")

SET(CMAKE_FIND_ROOT_PATH ${QNX_TARGET}) 
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# for Zlib and so
SET(CMAKE_APPBUNDLE_PATH "${QNX_TARGET}/lib:${QNX_TARGET}/usr/lib:${QNX_TARGET}/usr/include")
# for some reason FIND_PACKAGE doesn't find zlib properly so add this
SET(ZLIB_LIBRARY "{QNX_TARGET}/usr/lib/zlib.a")
# pthread is supported on QNX
SET(CMAKE_USE_PTHREADS_INIT TRUE)