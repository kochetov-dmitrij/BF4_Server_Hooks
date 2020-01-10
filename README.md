# Low level modifications for BF4 server

### Features:
* Undestructible tower pillars on Shanghai map, which is impossible to do with any legal plugins.\
Changes 1250 HP of each pillar to 10^21 HP by direcly substituting values in memory
* Attack helis don't spawn until a certain amount of players on the server *
* Autokill for using enemy attack heli *

(*) means that this feature relies on someone elses code which I don't want to publish\
These features are in `more` branch and won't work without additional code

### Requrements:
* Battlefield 4 server build 179665/R63
* Access to the server proccess since you have to inject a dll
* Visual Studio
