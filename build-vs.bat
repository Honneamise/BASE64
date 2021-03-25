
@ECHO OFF

SET msvc_ver=2019

SET msvc=C:\Program Files (x86)\Microsoft Visual Studio\%msvc_ver%\Community\VC\Auxiliary\Build\vcvars32.bat

SET compile=cl.exe /analyze /MTd /W3 /TC SRC/*.c /link /SUBSYSTEM:CONSOLE /MACHINE:X86 /OUT:debug.exe

SET clean=del *.obj *.xml

"%msvc%" && cmd /k "%compile% && %clean% && pause && exit"
