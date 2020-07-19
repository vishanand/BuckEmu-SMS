// Visual Display Processor (VDP)
#ifndef VDP_H
#define VDP_H
#include <cstdint>

class SMS;

class VDP {
    public:
        VDP(SMS& smsP);

        void write_control(uint8_t data);
        void write_data(uint8_t data);
        uint8_t read_control();
        uint8_t read_data();

        void rasterize_line(int line, uint32_t * pixels);

        uint8_t v_counter;
        uint8_t h_counter;

    private:
        uint8_t VRAM[0x4000]; // Video RAM
        uint8_t CRAM[0x20]; // Color RAM

        uint8_t VDP_reg[0xF]; // VDP registers
        uint8_t read_buf; // Read Buffer
        uint8_t code_reg; // Code register
        uint16_t addr_reg; // Address register
                
        /* flag set when first byte written, cleared when second byte written,
           cleared when control port read, cleared when data port read/write  */
        bool cmd_flag;

        bool INT_flag; // Frame Interrupt Pending (set on first line after end of active display)
        bool OVR_flag; // Sprite Overflow (more than 8 sprites in line)
        bool COL_flag; // Sprite Collision    

        uint32_t temp_tile_row[8]; //store 8 pixels temporarily

        uint32_t color_conv[0x40]; // correspondence between SMS and SDL colors (6 bit to 24 bit)
        uint8_t interpolate_color(uint8_t color); // convert from 2 bit to 8 bit color
        inline void rasterize_tile(int line, int col);

        SMS& sms; // reference to SMS object
};

#endif