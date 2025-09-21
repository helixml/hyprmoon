# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

if(EXISTS "/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitclone-lastrun.txt" AND EXISTS "/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitinfo.txt" AND
  "/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitinfo.txt")
  message(VERBOSE
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

# Even at VERBOSE level, we don't want to see the commands executed, but
# enabling them to be shown for DEBUG may be useful to help diagnose problems.
cmake_language(GET_MESSAGE_LOG_LEVEL active_log_level)
if(active_log_level MATCHES "DEBUG|TRACE")
  set(maybe_show_command COMMAND_ECHO STDOUT)
else()
  set(maybe_show_command "")
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/workspace/build-simple/_deps/simplewebserver-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/workspace/build-simple/_deps/simplewebserver-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --config "advice.detachedHead=false" "https://gitlab.com/eidheim/Simple-Web-Server.git" "simplewebserver-src"
    WORKING_DIRECTORY "/workspace/build-simple/_deps"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(NOTICE "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://gitlab.com/eidheim/Simple-Web-Server.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "4abe349" --
  WORKING_DIRECTORY "/workspace/build-simple/_deps/simplewebserver-src"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '4abe349'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/workspace/build-simple/_deps/simplewebserver-src"
    RESULT_VARIABLE error_code
    ${maybe_show_command}
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/workspace/build-simple/_deps/simplewebserver-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitinfo.txt" "/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  ${maybe_show_command}
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/workspace/build-simple/_deps/simplewebserver-subbuild/simplewebserver-populate-prefix/src/simplewebserver-populate-stamp/simplewebserver-populate-gitclone-lastrun.txt'")
endif()
