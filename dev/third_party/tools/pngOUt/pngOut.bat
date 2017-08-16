@echo off 
set work_path=%1
c:
cd %work_path%

for /R %%s in (./*.png) do ( 

C:\thNew20140612\godlike\art\icon\pngquant -ext .png -force %%s 
echo %%s 
) 
pause 