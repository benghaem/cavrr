
//SREG  -- AVR Status Register
typedef struct
{
    //Bit 7 -- I: Global Interrupt Enable
    int I = 0;

    //Bit 6 -- T: Bit Copy Storage
    int T = 0;

    //Bit 5 -- H: Half Carry Flag
    int H = 0;

    //Bit 4 -- S: Sign Bit, S = N XOR V
    int S = 0;

    //Bit 3 -- V: Two's Complement Overflow Flag
    int V = 0;

    //Bit 2 -- N: Negative Flag
    int N = 0;

    //Bit 1 -- Z: Zero Flag
    int Z = 0;

    //Bit 0 -- C: Carry Flag
    int C = 0;
} SREG;

//RFILE  -- AVR Register file
//32 locations each 16 bits wide
typedef struct
{
    unsigned short r[32];
} RFILE;

//read out a full 16 bit register
unsigned short RFILE_read_reg(int r);

unsigned char RFILE_read_half_reg(int r, int half)
