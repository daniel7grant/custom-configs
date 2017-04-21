setlocal ENABLEDELAYEDEXPANSION
set filename=%1
set word=\localhost
set str=%filename:\wamp64\www=!word!%
set str=%filename:\wamp\www=!word!%
if %str%==%filename% exit
set filename=%str%
:loop
for	/F "tokens=2,* delims=^\^" %%a IN (%str%) do (
	set str="%%a\%%b"
)
if not "%str:~1,9%" == "localhost" goto loop
start http:\\%str%
exit
