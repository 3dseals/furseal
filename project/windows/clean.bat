::
::  Copyright (c) 2011 netsurfers
::


%~d0
cd %~p0\..\..

del /s /q *.o *.ncb *.user log
del /s /q /ah *.suo Thumbs.db

cd documents\
rmdir /s /q .\reference\html
cd ..

cd project\windows\furseal
rmdir /s /q .\debug
rmdir /s /q .\release
rmdir /s /q furseal\Debug
rmdir /s /q furseal\Release
del /q  ..\..\..\library\*
cd ..\..\..

cd sample\project\windows\sample
rmdir /s /q .\debug
rmdir /s /q .\release
rmdir /s /q sample01_hello\Debug
rmdir /s /q sample01_hello\Release
del /q ..\icon\*.aps
del /q  ..\..\..\bin\*
cd ..\..\..\..
