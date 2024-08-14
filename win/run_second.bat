@echo off
setlocal

rem Define variables
set "NETWORK=info"
set "IP_ADDRESS=172.18.0.22"
set "PORT=7000"
set "CONTAINERNAME=chatuser2"

set "IMAGENAME=localchat"

rem Run Docker container
docker run -it --rm --name %CONTAINERNAME% --network %NETWORK% --ip %IP_ADDRESS% --env IP_ADDRESS=%IP_ADDRESS% --env PORT=%PORT% %IMAGENAME%

endlocal