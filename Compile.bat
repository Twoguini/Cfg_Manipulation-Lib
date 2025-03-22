del testCFGFILE.dll, cfgFileTest_M.exe

gcc -shared -o testCFGFILE.dll cfgFile_M.c

gcc -o cfgFileTest_M.exe cfgFileTest_M.c -L. -ltestCFGFILE -I. -static-libgcc -static-libstdc++
