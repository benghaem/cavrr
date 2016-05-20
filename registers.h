//RFILE  -- General AVR Register file
//32 locations each 8 bits wide
typedef struct
{
    unsigned char reg[32];
} RFILE;

//read out a full 8 bit register
unsigned char RFILE_read_reg(RFILE* rfile, int r);

//write a full 8 bit register
void RFILE_write_reg(RFILE* rfile, int r, unsigned char val);

//IOFILE -- IO AVR Registers
//64 locations each 8 bits wide
typedef struct
{
    unsigned char reg[64];
} IOFILE;

//read out a full 8 bit IO register
unsigned char IOFILE_read_reg(IOFILE* io, int r);

//read out a single bit from an IO Register
//iofile, register, bit
unsigned char IOFILE_read_reg_bit(IOFILE* io, int r, int b);

