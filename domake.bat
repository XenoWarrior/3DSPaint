@ECHO off

:CLEAN_UP
RMDIR /S /Q build
RMDIR /S /Q -p
DEL /Q HelloWorld.3dsx
DEL /Q HelloWorld.elf
DEL /Q HelloWorld.smdh

:MAKE
make

:OUTPUT
ECHO HomeBrew Launcher 3DSX file built.

:BANNER
bannertool makebanner -i banner.png -a banner.wav -o banner.bin

:OUTPUT
ECHO Banner BIN file built.

arm-none-eabi-strip HelloWorld.elf
makerom -f cia -o HelloWorld.cia -rsf HelloWorld.rsf -target t -exefslogo -elf HelloWorld.elf -icon HelloWorld.smdh -banner banner.bin

:OUTPUT
ECHO 3DS Home Menu bubble built.

arm-none-eabi-strip HelloWorld.elf
makerom -f cci -o HelloWorld.3ds -rsf HelloWorld.rsf -target d -exefslogo -elf HelloWorld.elf -icon HelloWorld.smdh -banner banner.bin

:OUTPUT
ECHO 3DS GateWay 3DS file built.

@ECHO Press any key to send to 3DS console.
pause

java -jar sockfile.jar 192.168.0.21 HelloWorld.cia