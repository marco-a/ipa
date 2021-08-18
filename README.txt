====================
= IPA MARCO AGNOLI =
====================
>> Ordnerstruktur

./src/
	Jeglicher Sourcecode.

./src/lib/
	Sourcecode f�r die Libraries.

./Makefile
	Datei f�r make.exe

./PROGRAM.hex
	Intel HEX-Datei welche auf den Mikrokontroller geladen werden kann.


Tabbreite	:	4 Leerzeichen
Coding Style	:	wiki/General/LehrlingsProgrammierrichtlinien
Lineending	:	CR, \n (UNIX)
Mikrokontroller	:	ATMega16A @ 5V

Kompilieren mit	:	make all

Hochladen der Datei mit avrdude oder anderem Programm.

!!WICHTIG!!:

Fuse bits:

- Externer Quarz muss angew�hlt werden
- Externe JTAG Schnittstelle muss deaktiviert werden

Fusebits werden _NICHT_ automatisch geschrieben