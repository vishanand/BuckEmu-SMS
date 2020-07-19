#include <cstdio>
#include <string>
#include <SDL2/SDL.h>

#include "sms.hpp"

#define WIDTH    256
#define HEIGHT   192
#define SCALE    2
#define FPS      60

int main(){
    SMS sms;

    std::printf("BuckEmu SMS: a Sega Master System Emulator!\n");

    // load ROM into memory
    std::string filename = "ROM/owdemo.sms";
    if (!sms.cart.openROM(filename)){
        std::printf("Error opening ROM file '%s'\n", filename.c_str());
        return 1;
    }

    std::printf("\nStarting emulation at 0x00!\n");

    // set up SDL
    bool quit = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("BuckEmu SMS",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH * SCALE, HEIGHT * SCALE, 0);

    // set up texture rendering
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
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

        // TODO: Run VDP for one line
        sms.vdp.rasterize_line(line, pixels);

        if (frame % FPS == 0){ // every 60 frames (one second)
            //memset(pixels, (rand() % 0xFF), WIDTH * HEIGHT * sizeof(Uint32));
            uint32_t colour = rand() % 0xFFFFFFFF;
            for (int i=20; i<256-20; i++){
                for (int j=20; j<192-20; j++){
                    pixels[j*WIDTH + i] = colour;
                }
            }
            frame = 0;            
        }
        if (line % HEIGHT == 0){ // every frame
            SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

            SDL_PollEvent(&event);
            switch (event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);            

            int wait_time = (SDL_GetTicks() - startTime);
            if (wait_time < 1000/FPS){
                SDL_Delay(1000/FPS - wait_time);
            }

            startTime = SDL_GetTicks();
            frame++;
            line = 0;
        } 
        pixels[(5 % HEIGHT) * WIDTH + (100 % WIDTH)] = rand() % 0xFFFFFFFF;                       
    }

    // clean up SDL
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}
