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
##### Linux
To build the project, run `cmake .` and then `make` in the root directory of the project.
This will create a binary called `semester_project`. Use the command above to run the project.
##### Windows
You will need to have `cmake` and `vcpkg` installed. To build the project you will also need to pass `vcpkg.cmake` to `cmake`. To learn how to do this, follow this link: https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration. Building can also be done in Visual Studio.

##### Documentation
To manually build the documentation, go to the `doc` folder and run `make`. You need to have the `pdflatex` command available on your system, and necessary LaTeX packages available. A compiled version is available in the project root.


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

### Missing features
The following features are missing from the emulator:
 - Sound
 - Serial communication
 - Cartridge memory controllers 
   - Only games with simple ROM work, check if game ROM has bit 0x0147 set to 0x00
   - Confirmed working games are Tetris and Dr. Mario
 - PPU variable length pixel transfer
 - Exact T-Cycle timing
