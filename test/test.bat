del "%~dp0checkins\mypt\*.*?"
del "%~dp0checkins\mypt2\*.*?"
del "%~dp0checkins\mypt3\*.*?"
del "%~dp0checkins\mypt4\*.*?"
del "%~dp0checkins\mypt5\*.*?"

del "%~dp0checkouts\mypt\*.*?"
del "%~dp0checkouts\mypt2\*.*?"
del "%~dp0checkouts\mypt3\*.*?"
del "%~dp0checkouts\mypt4\*.*?"
del "%~dp0checkouts\mypt5\*.*?"

del "%~dp0repos\mypt\*.*?"
del "%~dp0repos\mypt2\*.*?"
del "%~dp0repos\mypt3\*.*?"
del "%~dp0repos\mypt4\*.*?"
del "%~dp0repos\mypt5\*.*?"

REM change bin\main.exe with your executable file
bin\main.exe create %~dp0mypt\ %~dp0repos\mypt\
bin\main.exe check-out %~dp0repos\mypt\ %~dp0checkouts\mypt\ manifest_1.txt
bin\main.exe check-out %~dp0repos\mypt\ %~dp0mypt\ manifest_1.txt
bin\main.exe check-in %~dp0checkouts\mypt\ %~dp0checkins\mypt\ manifest_1.txt

@echo off
@echo VCS>>%~dp0checkouts\mypt\test.txt

bin\main.exe check-in %~dp0checkouts\mypt\ %~dp0checkins\mypt\ manifest_1.txt
bin\main.exe check-out %~dp0checkins\mypt\ %~dp0checkouts\mypt\ manifest_2.txt

bin\main.exe merge %~dp0checkouts\mypt\manifest_2.txt %~dp0checkins\mypt
