# OscilloscopeProgramming
Program dedicated to the operation of an oscilloscope

Instructions to run the sample code provided:

Run on x86
cl *.c /Zi /EHsc /nologo /link i386/ftd2xx.lib /out:controller.exe

Run on x64
cl *.c /Zi /EHsc /nologo /link amd64/ftd2xx.lib /out:controller.exe


## Instructions to run ReadFile(reqfRead) and WriteFile(reqfWrite) via driver:

```bash
> g++ -std=c++17 driver.cpp ReadFile.cpp WriteFile.cpp FtdiDevice.h

> ./a.out file1.txt <some_file_name>.txt
```