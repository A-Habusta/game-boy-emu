# Adrian Habusta

### Quick rundown of the project
This project is an emulator for the original Game Boy console. Usage of the project is:

`semester_project <boot_rom_file> <rom_file> [<sram_file>]` 

Boot rom is included in the repository, but the rom file is not. The rom file is the game you want to play. The sram 
file is optional, and is used to save the game, if the game supports it.

#### Boot ROM
A boot rom file is available in the bootrom directory. The file is assembled from this file:
https://github.com/LIJI32/SameBoy/blob/master/BootROMs/dmg_boot.asm

### Dependencies
This project depends on SDL2. Refer to your package manager on how to install it. On Windows, you can use vcpkg to 
download it.

### Build
To build the project, run `cmake .` and then `make` in the root directory of the project.
This will create a binary called `semester_project`. Use the command above to run the project.

### Controls
The controls are mapped to the keyboard as follows:

| Gameboy Button | Keyboard Key |
|----------------|--------------|
| A              | Z            |
| B              | X            |
| Start          | Enter        |
| Select         | Space        |
| Up             | Up Arrow     |
| Down           | Down Arrow   |
| Left           | Left Arrow   |
| Right          | Right Arrow  |
