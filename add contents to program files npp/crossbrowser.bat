2>NUL start chrome %1
2>NUL start firefox %1
2>NUL start opera %1
2>NUL start vivaldi %1
2>NUL start safari %1 
for /f "tokens=4-5 delims=. " %%i in ('ver') do set VERSION=%%i.%%j
if  "%version%" == "10.0" ( start microsoft-edge %1 ) else ( start iexplore %1 )
exit