
    Welcome to PSPXTI

Original Author of XTiger

  Tiger is a TI-92 emulator for Linux/x86.  It was originally written by
  Jonas Minnberg and was closed source.  There were two versions: the SVGA
  version and an Xlib version.  Jonas Minnberg has not worked on XTiger
  for quite a while and he gave Misha Nasledov the code and permission 
  to GPL it.

Author of the PSP port version 

  Ludovic.Jacomme (ludovic.jacomme@gmail.com) also known as Zx-81

  Special thanks to WebCousin for giving me its TI92, and to SubZero
  for testing earlier versions of this emulator on 1.5 FW.

  The IR keyboard support is based on the work of Harald Fielker 
  (author of the PSP IR Keyboard Library), and the Virtual keyboard 
  is based on the Danzel code.

  If you like Eboot music you may find the complete album here :
  http://www.jamendo.com/en/album/22408


1. INTRODUCTION
   ------------

  This is a port on PSP of the Texas Instrument 92 emulator for 
  X Window Systems : Xtiger.

  This package is under GPL Copyright, read COPYING file for
  more information about it.


2. INSTALLATION
   ------------

  Unzip the zip file, and copy the content of the directory fw4x or fw15
  (depending of the version of your firmware) on the psp/game, psp/game150,
  or psp/game4xx if you use custom firmware 4.01-M33.

  It has been developped on linux for Firmware 4.01-m33 and i hope it works
  also for other firmwares.

  A rom is available with this package, but you can legally use it only 
  if you own already a TI-92.

  TI92+ roms for hardware version 2 are not compatible with 
  this emulator !

  For any comments or questions on this version, please visit 
  http://zx81.zx81.free.fr or http://zx81.dcemu.co.uk 


3. CONTROL
   ------------

3.1 Standard control :
   ------------

In the TI92 emulator window, there are three different mapping (standard, left
trigger, and right Trigger mappings).  You can toggle between while playing
inside the emulator using the two trigger keys.

    -------------------------------------
    PSP        TI-92          (standard)
  
    Square      Backspace
    Circle      Space 
    Triangle    Escape 
    Cross       Enter1
    Up          Up
    Down        Down
    Left        Left
    Right       Right

    -------------------------------------
    PSP        TI-92    (left)

    Square      Apps   
    Circle      Diamond 
    Triangle    Hand   
    Cross       2nd   
    Up          F1
    Down        F2  
    Left        F3   
    Right       F4   

    -------------------------------------
    PSP        TI-92    (right)

    Square      (     
    Circle      )    
    Triangle    Diamond
    Cross       ,     
    Up          Up
    Down        Down
    Left        Left
    Right       Right

Press Select  to enter in emulator main menu.
Press Start   open/close the On-Screen keyboard

  In the main menu

    RTrigger   Reset the emulator

    Triangle   Go Up directory
    Cross      Valid
    Circle     Valid
    Triangle   Go Back to the emulator window

The On-Screen Keyboard of "Danzel" and "Jeff Chen"

Use the stick to choose one of the 9 squares, and use Cross, Triangle, Circle
or Square to choose one of the 4 letters of the highlighted square.

Use LTrigger and RTrigger to see other 9 squares figures.

3.2 IR Keyboard control :
   ------------

You can use an IR-keyboard with PSP XTI. Personally i use a Targus Universal
Wireless keyboard, which is one of the best. 

You can edit the file pspirkeyb.ini and modify it according to your IR 
keyboard model :

# 1  - Compaq/HP foldable keyboard
# 2  - Snap'n'Type
# 5  - HP Slim keyboard
# 8  - Belkin IR (not IrDA)
# 9  - Flexis FX-100 keyboard
# 10 - Benq G250 gamepad
# 12 - Micro Innovations Foldaway keyboard
# 13 - Micro Innovations Datapad
# 14 - Compaq MicroKeyboard
# 15 - Targus Universal Wireless keyboard
# 18 - Freedom keyboard
# 19 - NOVAETS - KIS2 - IR Keyboard
# 20 - Palm Universal Wireless Keyboard
# 21 - Hama Palm IR keyboard

You can use the arrows of your keyboard to navigate through the menus. The
ENTER key is use to validate, and the Escape key to cancel.

Inside the TI-92 window, you can press TAB to return to the emulator menu. 

The key CTRL-A -> CTRL-G are mapped to F1 -> F7.
The key CTRL-O -> Diamond
The key Ctrl-P -> 2nd
The key Ctrl-K -> Hand
The key Ctrl-L -> Home

In the emulator menu :

  IR-keyboard   PSP

  Cursor        Digital Pad

  Tab           Start
  Ctrl-W        Start

  Escape        Select
  Ctrl-Q        Select

  Ctrl-E        Triangle
  Ctrl-X        Cross
  Ctrl-S        Square
  Ctrl-F        Circle
  Ctrl-Z        L-trigger
  Ctrl-C        R-trigger


4. LOADING TI-92 APPLICATION FILES
   ------------

  You can save any TI 92 files (with .92* file extension) on your 
  PSP memory stick. Then, while inside the TI-92 emulator, just press
  simultaneously SELECT + L1 + R1, and then using the file selector 
  choose one application to load in the RAM of your emulator.

  It may happen that the file is not properly loaded, and so you have 
  to try again.

  To manage applications already loaded in your TI-92, you can use 
  the VLINK menu (activate button 2ND and press the '-' key).

  You can find many applications on http://www.ticalc.org/ and on the 
  official TI ftp site ftp://ftp.ti.com/pub/graph-ti/calc-apps/92/

  If you don't have enough memory to load all the program files you want
  yoy may then use the "Save slot" menu to save differents states of 
  the emulator (save slot). You will then be able to load them, one at
  a time for different usages.

5. LOADING KEY MAPPING FILES
   ------------

If you want to modify the default keyboard mapping between PSP Keys and TI-92
keys you can write your own mapping file. Using notepad for example you can
edit a file with the .kbd extension and put it in the kbd directory.

For the exact syntax of those mapping files, have a look on sample files
already presents in the kbd directory (default.kbd etc ...). (or use the save
keyboard file menu to generate a default.kbd file).

After writting such keyboard mapping file, you can load them using the
keyboard menu inside the emulator.

You can now use the Keyboard menu and edit, load and save your default
keyboard mapping file inside the emulator. 

6. COMMENTS
   ------------

  You can write your own comments using the "Comment" menu. 
  All your comments would be saved in a comment.txt text file.


7. COMPILATION
   ------------

  It has been developped under Linux using gcc with PSPSDK. 
  To rebuild the homebrew run the Makefile in the src archive.


