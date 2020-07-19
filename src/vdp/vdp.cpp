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

        color_conv[i] = (blue << 16) | (green << 8) | red;
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
            VDP_reg[(addr_reg & 0x0F00) >> 8] = addr_reg & 0xFF;
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
        VRAM[addr_reg++] = data;
    else
        CRAM[(addr_reg++) & 0x20] = data;
    addr_reg &= 0x3FFF;
}

uint8_t VDP::read_data(){
    cmd_flag = false;
    uint8_t temp = read_buf;

    read_buf = VRAM[addr_reg++];
    addr_reg &= 0x3FFF;

    return temp;    
}

void VDP::rasterize_line(int line, uint32_t * pixels){
    
    // rasterize background
    uint8_t backdrop = (VDP_reg[7] & 0xF) | 0x10;

    uint8_t h_scroll = VDP_reg[8];
    uint8_t v_scroll = VDP_reg[9];

    uint16_t base_addr = ((VDP_reg[2] > 1) & 0x7) << 11;
    base_addr = 0x3800;

    if (line < 2 && CheckBit(VDP_reg[0], 6))
        h_scroll = 0; // disable h-scroll for row 0+1

    uint8_t col = (h_scroll >> 3);
    uint8_t fine_scroll = h_scroll & 0x7;
    uint8_t row = line / 8;

    uint8_t high_byte = VRAM[base_addr + 28*2*row + col + 1];
    uint8_t low_byte = VRAM[base_addr + 28*2*row + col];
    uint16_t tile = high_byte << 8 | low_byte;

    uint16_t pattern_indx = (tile & 0x200) * 0x20;

    // TODO: Finish this function

    if (fine_scroll > 0){ // draw partial first column
        for (int i=fine_scroll; i<8; i++){            
            pixels[line * 256 + (i % 256)] = 0;
        }
        col++;
    }
    
    // DRAW: pixels[y * 256 + (x % 256)] = c;
}