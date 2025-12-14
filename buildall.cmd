@cls
@echo off

setlocal

@rem  from 2025-12-14 we use GCC (MinGW) on Windows
@rem  dowmloaded from winlibs.com
@rem  GCC is (almost) OS agnostic conpiler 
@rem  just be sure gcc is on the path
@rem  on windows (and on my machine) that is g:/mingw64/bin folder
@rem  we try and use os agnostic make
@rem  on Linux we use make on windows
@rem  Yes: mingw32-make.exe is GNU Make.
@rem  No: it does not behave exactly like GNU Make on Linux unless the surrounding environment matches.
@rem  we deliberately avoid CMake (this time)


pushd .
pushd .\A
call .\build-a.cmd
popd
popd

pushd .
pushd .\dbj-syserrmsg
call .\build-dbj-syserrmsg.cmd
popd
popd

pushd .
pushd .\dbj-shmem
call .\build-dbj-shmem.cmd
popd
popd

pushd .
pushd .\dbj-vector
call .\build-dbj-vector.cmd
popd
popd

pushd .
pushd .\host_console_app
call .\build-console-host.cmd
popd
popd

endlocal
