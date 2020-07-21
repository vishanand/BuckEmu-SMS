#include <cstdio>
#include <string>
#include <SDL2/SDL.h>

#include "sms.hpp"

#define WIDTH    256
#define HEIGHT   192
#define SCALE    2
#define FPS      60

void oldMain();

int main(){
    // Uncomment below to run zexdoc tests without VDP
    /* oldMain();
    return 0; */

    SMS sms;

    std::printf("BuckEmu SMS: a Sega Master System Emulator!\n");

    // load ROM into memory
    std::string filename = "ROM/pauseTest.sms";
    if (!sms.cart.openROM(filename)){
        std::printf("Error opening ROM file '%s'\n", filename.c_str());
        return 1;
    }

    std::printf("\nStarting emulation at 0x00!\n");

    // set up SDL
    bool quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow("BuckEmu SMS",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * SCALE, HEIGHT * SCALE, 0);

    // set up texture rendering
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    uint32_t * pixels = new uint32_t[WIDTH * HEIGHT];
    memset(pixels, 0xFF, WIDTH * HEIGHT * sizeof(Uint32));

    // set up frame limiter
        
    int16_t line = 0;    

    // start timer
    int startTime = SDL_GetTicks();

    int cycles = 0;
    int cycCnt = 0;
    int frame = 0;

    // main loop
    while (!quit)
    {   
        // loop enough cycles for a line
        while (cycCnt < 228){
            cycles = sms.z80.runInstruction();
            cycCnt += cycles;
            
            if (cycles >= 69){
                std::printf("\nHit unimplemented opcode:\n");
                uint16_t pc = sms.z80.getPC() - 1;
                uint8_t opcode = sms.mem.getByte(pc);
                std::printf("PC: 0x%X \tOpcode: 0x%X \tCycles: %d\n", pc, opcode, cycles);    
                quit = true;
            }            

        }

        cycCnt -= 228;
        line++;
        
        sms.vdp.rasterize_line(line, pixels);

        /* if (frame % FPS == 0){ // every 60 frames (one second)
            frame = 0;     
        } */
        if (line == 262){ // every frame
            SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

            if (SDL_PollEvent(&event)){
                switch (event.type)
                {
                    case SDL_QUIT:
                        quit = true;
                        break;

                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym){
                            case SDLK_RETURN:
                                sms.z80.NMI();
                                break;
                            case SDLK_q:
                                quit = true;
                                break;
                        }                        
                        break;
                }
            } 

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            
            while (SDL_GetTicks() - startTime < 1000/FPS){
                SDL_Delay(1);
            }

            startTime = SDL_GetTicks();
            frame++;
            line = 0;
        }
    }

    // clean up SDL
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}

// run ZEXDOC without VDP and without timing limits
void oldMain(){
    SMS sms;

    std::printf("BuckEmu SMS: a Sega Master System Emulator!\n");

    // load ROM into memory
    std::string filename = "ROM/zexdoc_sdsc.sms";
    if (!sms.cart.openROM(filename)){
        std::printf("Error opening ROM file '%s'\n", filename.c_str());
        return;
    }

    // run instructions until we hit an unimplemented instruction
    int cycles = 0;
    long instCnt = 0;
    long cycCnt = 0;
    std::printf("\nStarting emulation at 0x00!\n");
    while (cycles < 69){
        uint16_t pc = sms.z80.getPC();
        uint32_t opcode = sms.mem.getByte(pc);
        if (opcode == 0xED || opcode == 0xDD || opcode == 0xFD || opcode == 0xCB){
            opcode = (opcode << 8) | sms.mem.getByte(pc+1);
            
            if ((opcode & 0xFF) == 0xCB){
                opcode = (opcode << 8) | sms.mem.getByte(pc+2);
                opcode = (opcode << 8) | sms.mem.getByte(pc+3);
            }
        }
        cycles = sms.z80.runInstruction();
        cycCnt += cycles;
        instCnt++;

        if (cycles >= 69){            
            std::printf("\n\nRan %ld instructions, %ld cycles, hit unimplemented opcode:\n", instCnt, cycCnt);
            std::printf("PC: 0x%X \tOpcode: 0x%X \tCycles: %d\n", pc, opcode, cycles);
        }
    }
}
