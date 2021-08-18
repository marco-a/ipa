====================
= IPA MARCO AGNOLI =
====================
>> Ordnerstruktur

./src/
	Jeglicher Sourcecode.

./src/lib/
	Sourcecode für die Libraries.

./Makefile
	Datei für make.exe

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

- Externer Quarz muss angewählt werden
- Externe JTAG Schnittstelle muss deaktiviert werden

Fusebits werden _NICHT_ automatisch geschrieben