# OscilloscopeProgramming
Program dedicated to the operation of an oscilloscope

Run on x86
cl *.c /Zi /EHsc /nologo /link i386/ftd2xx.lib /out:controller.exe

Run on x64
cl *.c /Zi /EHsc /nologo /link amd64/ftd2xx.lib /out:controller.exe