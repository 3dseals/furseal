del /s /q *.sdf
rmdir /s /q .\Debug
rmdir /s /q .\Release

for /d %i in (.\*) do @rmdir /s /q %i\Release
for /d %i in (.\*) do @rmdir /s /q %i\Debug
