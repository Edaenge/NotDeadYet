@ECHO OFF

:: Copy Includes
IF EXIST External\NDYGFX\Include rmdir /q /s External\NDYGFX\Include
IF NOT EXIST External\NDYGFX\Include mkdir External\NDYGFX\Include
xcopy /s /e ..\NDYGFX\NDYGFX\Include External\NDYGFX\Include

:: Copy Lib
IF EXIST External\NDYGFX\Lib\NDYGFXD.lib del External\NDYGFX\Lib\NDYGFXD.lib
IF EXIST External\NDYGFX\Lib\NDYGFX.lib del External\NDYGFX\Lib\NDYGFX.lib
IF NOT EXIST External\NDYGFX\Lib\ mkdir External\NDYGFX\Lib
IF EXIST ..\NDYGFX\Build\Debug\NDYGFXD.lib copy ..\NDYGFX\Build\Debug\NDYGFXD.lib External\NDYGFX\Lib\NDYGFXD.lib
IF EXIST ..\NDYGFX\Build\Release\NDYGFX.lib copy ..\NDYGFX\Build\Release\NDYGFX.lib External\NDYGFX\Lib\NDYGFX.lib

:: Copy DLL
IF EXIST External\NDYGFX\Bin\BTHFBXD.dll del Bin\Debug\BTHFBXD.dll
IF EXIST External\NDYGFX\Bin\BTHFBX.dll del Bin\Debug\BTHFBX.dll
IF EXIST External\NDYGFX\Bin\NDYGFXD.dll del Bin\Debug\NDYGFXD.dll
IF EXIST External\NDYGFX\Bin\NDYGFX.dll del Bin\Debug\NDYGFX.dll
IF NOT EXIST External\NDYGFX\Bin\ mkdir External\NDYGFX\Bin\
IF EXIST ..\NDYGFX\Build\Debug\BTHFBXD.dll copy ..\NDYGFX\Build\Debug\BTHFBXD.dll External\NDYGFX\Bin\BTHFBXD.dll
IF EXIST ..\NDYGFX\Build\Release\BTHFBX.dll copy ..\NDYGFX\Build\Release\BTHFBX.dll External\NDYGFX\Bin\BTHFBX.dll
IF EXIST ..\NDYGFX\Build\Debug\NDYGFXD.dll copy ..\NDYGFX\Build\Debug\NDYGFXD.dll External\NDYGFX\Bin\NDYGFXD.dll
IF EXIST ..\NDYGFX\Build\Release\NDYGFX.dll copy ..\NDYGFX\Build\Release\NDYGFX.dll External\NDYGFX\Bin\NDYGFX.dll

:: Copy Shaders
IF EXIST WorkingDir\Shaders\ rmdir /q /s WorkingDir\Shaders
IF NOT EXIST WorkingDir\Shaders\ mkdir WorkingDir\Shaders
xcopy /s /e ..\NDYGFX\WorkingDir\Shaders WorkingDir\Shaders


ECHO All done!
pause