@echo off

:: IP address passed in as argument
set IPaddress=%1

:: Default name output if no 'Name:' is found in nslookup
set DNSname=Unknown

:: If a DNS name is found, store in variable
for /f "tokens=2 delims=:" %%f in ('nslookup %IPaddress% 2^>^&1 ^| find /i "Name"') do set DNSname=%%f

:: Stripping spaces from the token variable
set DNSname=%DNSname: =%

:: Output result
echo %DNSname%