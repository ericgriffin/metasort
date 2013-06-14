cd /d %~dp0
del .\stage\lib\* /Q /F
call bootstrap.bat
bjam.exe link=static threading=multi address-model=64 runtime-link=static release stage
bjam.exe link=static threading=multi address-model=64 runtime-link=static debug stage
