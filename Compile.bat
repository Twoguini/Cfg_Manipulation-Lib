if not exist "./build" ( 
  md build 
  echo "Build File Created"
) else (
  cd ./build
  del testCFGFILE.dll, cfgFileTest_M.exe
  echo ".dll and .exe deleted"
  cd ..\
)

gcc -shared -o ./build/testCFGFILE.dll ./src/cfgFile_M.c

gcc -o ./build/cfgFileTest_M.exe ./src/cfgFileTest_M.c -L./build -ltestCFGFILE -I. -static-libgcc -static-libstdc++
