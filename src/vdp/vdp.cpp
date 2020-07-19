#include <cstdio>
#include "vdp.hpp"
#include "../sms.hpp"

// macros to manipulate bit n of byte x
#define SetBit(x,n)     ((x) |= (1<<(n)))
#define ClearBit(x,n)   ((x) &= ~(1<<(n)))
#define FlipBit(x,n)    ((x) ^= (1<<(n)))
#define CheckBit(x,n)   ((x) & (1<<(n)))

VDP::VDP(SMS& smsP) : sms(smsP) {
    // fill out color conversion table
    for (int i=0; i<64; i++){
        uint8_t blue = interpolate_color((i & 0x30) >> 4);
        uint8_t green = interpolate_color((i & 0xC) >> 2);
        uint8_t red = interpolate_color(i & 0x3);

        color_conv[i] = (red << 16) | (green << 8) | blue;
    }
}

uint8_t VDP::interpolate_color(uint8_t color){
    // convert 2 bit to 8 bit color
    switch (color){
        case 0:
            return 0;
        case 1:
            return 85;
        case 2:
            return 170;
        default:
            return 255;
    }
}

void VDP::write_control(uint8_t data){
    if (cmd_flag){
        code_reg = data >> 6;
        addr_reg = (addr_reg & 0xFF) | ((data & 0x3F) << 8);

        if (code_reg == 0){
            // write to VRAM
            read_buf = VRAM[addr_reg++];
            addr_reg &= 0x3FFF;
        } else if (code_reg == 2){
            // write to VDP reg
            VDP_reg[data & 0xF] = addr_reg & 0xFF;
        }
    } else {
        addr_reg = (addr_reg & 0x3F00) | data;
    }

    cmd_flag ^= true;
}

uint8_t VDP::read_control(){
    cmd_flag = false;

    uint8_t temp = (INT_flag << 7) | (OVR_flag << 6) | (COL_flag << 5);
    INT_flag = 0;
    OVR_flag = 0;
    COL_flag = 0;

    return temp;
}

void VDP::write_data(uint8_t data){
    cmd_flag = false;

    if (code_reg < 3)
        VRAM[addr_reg] = data;
    else if (code_reg == 3)
        CRAM[addr_reg & 0x1F] = data;
    addr_reg++;
    addr_reg &= 0x3FFF;
}

uint8_t VDP::read_data(){
    cmd_flag = false;
    uint8_t temp = read_buf;

    read_buf = VRAM[addr_reg++];
    addr_reg &= 0x3FFF;

    return temp;    
}

inline void VDP::rasterize_tile(int line, int col){    
    uint8_t row = line / 8;
    uint16_t base_addr = ((VDP_reg[2] >> 1) & 0x7) << 11;
    uint16_t vram_addr = base_addr | ((row & 0x1F) << 6) | ((col & 0x1F) << 1);    
    uint8_t high_byte = VRAM[vram_addr+1];
    uint8_t low_byte = VRAM[vram_addr];
    uint16_t pattern_indx = ((high_byte & 1) << 8) | low_byte;
    bool priority = CheckBit(high_byte, 4);
    bool palette = CheckBit(high_byte, 3);
    bool v_flip = CheckBit(high_byte, 2);
    bool h_flip = CheckBit(high_byte, 1);

    // TODO flips, palettes, priority, etc
    int ln = (line % 8) * 4;
    uint8_t b0 = VRAM[(pattern_indx << 5) + ln];
    uint8_t b1 = VRAM[(pattern_indx << 5) + ln + 1];
    uint8_t b2 = VRAM[(pattern_indx << 5) + ln + 2];
    uint8_t b3 = VRAM[(pattern_indx << 5) + ln + 3];

    if (h_flip){
        for (int i=0; i<8; i++){
            uint8_t raw_color = ((b0 & 1) << 0) | ((b1 & 1) << 1) | ((b2 & 1) << 2) | ((b3 & 1) << 3);
            if (palette){
                raw_color = CRAM[raw_color + 16];
            } else {
                raw_color = CRAM[raw_color];
            }
            temp_tile_row[i] = color_conv[raw_color];

            b0 >>= 1;
            b1 >>= 1;
            b2 >>= 1;
            b3 >>= 1;
        }
    } else {
        for (int i=0; i<8; i++){
            uint8_t raw_color = ((b0 & 128) >> 7) | ((b1 & 128) >> 6) | ((b2 & 128) >> 5) | ((b3 & 128) >> 4);
            if (palette){
                raw_color = CRAM[raw_color + 16];
            } else {
                raw_color = CRAM[raw_color];
            }
            temp_tile_row[i] = color_conv[raw_color];

            b0 <<= 1;
            b1 <<= 1;
            b2 <<= 1;
            b3 <<= 1;
        }
    }
}

void VDP::rasterize_line(int line, uint32_t * pixels){
    
    // rasterize background
    uint8_t backdrop = (VDP_reg[7] & 0xF) | 0x10;

    uint8_t h_scroll = VDP_reg[8];
    uint8_t v_scroll = VDP_reg[9];

    if (line < 2 && CheckBit(VDP_reg[0], 6))
        h_scroll = 0; // disable h-scroll for row 0+1

    uint8_t col = (h_scroll >> 3);
    uint8_t fine_scroll = h_scroll & 0x7;
    uint8_t row = line / 8;    

    if (fine_scroll > 0){
        rasterize_tile(line, col);
        for (int i=0; i<fine_scroll; i++){
            pixels[line * 256 + (i % 256)] = color_conv[backdrop];
        }
        for (int i=fine_scroll; i<8; i++){  // draw partial first column        
            pixels[line * 256 + (i % 256)] = temp_tile_row[i];
        }
        col++;
    }

    for (; col < 32; col++){
        rasterize_tile(line, col);
        for (int i=0; i<8; i++){
            pixels[line * 256 + ((i + col*8) % 256)] = temp_tile_row[i];
        }
    }

    // DRAW: pixels[y * 256 + (x % 256)] = c;

    if (line == 0xC1){
        if (CheckBit(VDP_reg[1], 5)){
            sms.z80.interrupt(0xFF);
            INT_flag = true;
        }
    }
}