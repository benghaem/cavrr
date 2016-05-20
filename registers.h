
//RFILE  -- General AVR Register file
//32 locations each 8 bits wide
typedef struct
{
    unsigned char r[32];
} RFILE;


//read out a full 8 bit register
unsigned char RFILE_read_reg(int r);


//IOFILE -- IO AVR Registers
//64 locations each 8 bits wide
typedef struct
{
    unsigned char r[64];
} IOFILE

unsigned char IOFILE_read_reg(int r);

unsigned char IOFILE_read_reg_bit(int r, int b);

IOFILE_read_reg_bit(&f, SREG, SREG_H)
