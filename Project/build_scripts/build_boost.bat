cd /d %~dp0
del .\stage\lib\* /Q /F
call bootstrap.bat
bjam.exe link=static threading=multi runtime-link=static release stage
bjam.exe link=static threading=multi runtime-link=static debug stage
