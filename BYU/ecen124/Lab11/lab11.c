/* lab11.c 
 * last modified 10/29/03 by Brian Pratt
 */


#include <stdio.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------
 * Misc. Messages
 * ---------------------------------------------------------------------
 */
#define USAGE_MSG             "usage: lab11 <debug_level> <file name>\n"
#define TODO_MSG              "TODO: implement %s\n"


/* ---------------------------------------------------------------------
 * Error Messages
 * ---------------------------------------------------------------------
 */
#define BAD_FILENAME_MSG      "ERROR: %s cannot be opened for reading\n"
#define MEM_OUT_OF_BOUNDS_MSG "ERROR: memory address %#x out range\n"
#define PC_OVERFLOW_MSG       "ERROR: PC out of memory range\n"
#define TRAP_ERROR_MSG        "ERROR: Unknown trap vector %#x\n"
#define REG_ADDRESS_MSG       "ERROR: register address R%d not in range\n"
#define UNDEFINED_OPCODE_MSG  "ERROR: Unkown opcode %#x\n"


/* ---------------------------------------------------------------------
 * Debug Messages
 * ---------------------------------------------------------------------
 */
#define MEM_WRITE_MSG         "memory[%#x] = %#x\n"
#define SET_PC_MSG            "pc = %#x\n"
#define SET_REG_MSG           "R%d = %#x\n"
#define SET_CCODES_MSG        "N = %d, Z = %d, P = %d\n"
#define ADD_MSG               "ADD\tR%d,R%d,R%d\n"
#define ADDI_MSG              "ADD\tR%d,R%d,%#x\n"
#define AND_MSG               "AND\tR%d,R%d,R%d\n"
#define ANDI_MSG              "AND\tR%d,R%d,%#x\n"
#define BR_MSG                "BR%s\t%#x\n"
#define LD_MSG                "LD\tR%d,%#x\n"
#define LDI_MSG               "LDI\tR%d,%#x\n"
#define LDR_MSG               "LDR\tR%d,R%d,%#x\n"
#define ST_MSG                "ST\tR%d,%#x\n" 
#define STI_MSG               "STI\tR%d,%#x\n"
#define STR_MSG               "STR\tR%d,R%d,%#x\n"
#define LEA_MSG               "LEA\tR%d,%#x\n"
#define NOT_MSG               "NOT\tR%d,R%d\n"
#define JSR_MSG               "JSR\t%#x\n" 
#define JSRR_MSG              "JSRR\tR%d\n"
#define JMP_MSG               "JMP\tR%d\n"
#define RET_MSG               "RET\n"


/* ---------------------------------------------------------------------
 * Bit Masks
 *
 * Bit masks are used to see if specific bits are set in a number.
 * For example, if I want to see bit[5] in the global variable
 * ir, then I would return (ir & 0x0020).  See the IMM_5_MASK below.
 *
 * ---------------------------------------------------------------------
 */
#define BIT_15_MASK     0x00008000
#define BITS_31_16_MASK 0xffff0000
#define BITS_15_0_MASK  0x0000ffff
#define BITS_15_12_MASK 0x0000f000
/* You will need to define other bit masks here */
#define BIT_4_MASK 0x00000010
#define BIT_5_MASK 0x00000020
#define BIT_8_MASK 0x00000100
#define BIT_9_MASK 0x00000200
#define BIT_10_MASK 0x00000400
#define BIT_11_MASK 0x00000800

#define BITS_2_0_MASK 0x00000007
#define BITS_4_0_MASK 0x0000001F
#define BITS_5_0_MASK 0x0000003F
#define BITS_7_0_MASK 0x000000FF
#define BITS_8_0_MASK 0x000001FF
#define BITS_10_0_MASK 0x000007FF

#define BITS_11_9_MASK 0x00000E00
#define BITS_8_6_MASK 0x000001C0

#define BITS_31_5_MASK 0xFFFFFFE0
#define BITS_31_6_MASK 0xFFFFFFC0
#define BITS_31_9_MASK 0xFFFFFE00
#define BITS_31_11_MASK 0xFFFFF800


/* ---------------------------------------------------------------------
 * Misc. Constants
 * ---------------------------------------------------------------------
 */
/* Definition of memory size */
#define MAX_MEMORY 32767

/* Define TRUE/FALSE */
#define TRUE 1
#define FALSE 0


/* ---------------------------------------------------------------------
 * LC-3 Instructions
 * ---------------------------------------------------------------------
 */
#define ADD  0x1
#define AND  0x5
#define BR   0x0
#define JMP  0xC     /* Note: JMP and RET have the same op-code       */
#define JSR  0x4     /* Note: JSR and JSRR have the same op-code      */
#define LD   0x2
#define LDI  0xA
#define LDR  0x6
#define LEA  0xE
#define NOT  0x9
#define ST   0x3
#define STI  0xB
#define STR  0x7
#define TRAP 0xF

/* Define traps */
#define GETC 0x20
#define OUT  0x21
#define PUTS 0x22
#define IN   0x23
#define HALT 0x25


/* ---------------------------------------------------------------------
 * Memory operations
 * ---------------------------------------------------------------------
 */
int memory[MAX_MEMORY];  /* This is the memory for LC-3               */

void setMemoryData( int address, int value );
int getMemoryData( int address );
void checkMemoryBounds( int address );


/* ---------------------------------------------------------------------
 * General purpose register operations
 * ---------------------------------------------------------------------
 */
int registers[8];        /* These are the general purpose registers   */

void setRegister( int reg_address, int value );
int getRegister( int reg_address );
void resetAllRegisters();
void checkRegisterAddressBounds( int reg_address );


/* ---------------------------------------------------------------------
 * Program Counter (PC) operations
 * ---------------------------------------------------------------------
 */
int pc;                  /* The program counter                       */

void setPC( int address );
int getPC();


/* ---------------------------------------------------------------------
 * Condition Code Register operations
 * ---------------------------------------------------------------------
 */
int n;                   /* The N condition code                      */
int z;                   /* The Z condition code                      */
int p;                   /* The P condition code                      */

void setConditionCodes( int reg_address );
int getN();
int getZ();
int getP();


/* ---------------------------------------------------------------------
 * Instruction Register (IR) operations
 * ---------------------------------------------------------------------
 */
int ir;                  /* The instruction register                  */

void setIR( int instruction );
int getIR();
int getDR();
int getSR();
int getSR2();
int getSR1();
int getOpcode();
int getIRP();
int getIRZ();
int getIRN();
int getImmediateBit();
int getBaseR();
int getImm5();
int getAddrBit();
int getOffset6();
int getTrapvect8();


/* ---------------------------------------------------------------------
 * Utility Functions
 * ---------------------------------------------------------------------
 */
void loadProgram( char* filename );
void fetchInstruction();
void executeInstruction();
int maskTo16Bits( int value );
int signExtend16Bit( int value );
int signExtendImm5( int value );
int signExtendOffset6(int value);
int signExtendOffset9(int value);
int signExtendOffset11(int value);
void dumpState();
void resetLC3();


/* ---------------------------------------------------------------------
 * Misc. Globals
 * ---------------------------------------------------------------------
 */
int debug = 0;           /* Assign 1 to see debugging messages        */
int halt;                /* If halt is nonzero, the machine is halted */


/* 
 * int main( int argc, char* argv[] )
 *
 */
int main( int argc, char* argv[] ) {
  /* Print a warning if an argument is not provided on the command line. */
  if ( argc != 3 ) {
    printf( USAGE_MSG );
    exit( -1 );
  }
  debug = atoi( argv[1] );   /* Set debug flag on command line         */
  resetLC3();                /* Reset LC3 to an initial state          */
  loadProgram( argv[2] );    /* Load the program                       */
  while ( !halt ) {          /* Keep going as long as halt flag is 0   */
    fetchInstruction();      /* Loads the ir and increments the pc     */
    executeInstruction();    /* Completely executes the instruction    */
    /* Check debug, if 1, dump LC3 state to see what is going on       */
    if (debug) {
      dumpState();           /* See below to see what it does          */
    }
  }
  printf("\n");
  if(debug)
	printf("\nhalted\n");    /* The LC-3 has halted                    */
  return( 0 );
}


/* ---------------------------------------------------------------------
 * Memory operations
 * ---------------------------------------------------------------------
 */
 
/* 
 * int getMemoryData(int address)
 * 
 */
int getMemoryData(int address) {
  checkMemoryBounds( address );
  return memory[address];
}

/* 
 * void setMemoryData( int address, int value )
 *
 * memory[address] = value; outputs operation data if debug asserted
 *
 */
void setMemoryData( int address, int value ) {
	//printf("%x - mem loc\n",address);
  checkMemoryBounds( address );
  memory[address] = maskTo16Bits( value );
  if ( debug )
    printf( MEM_WRITE_MSG, address, value );
}

/* 
 * void checkMemoryBounds(int address)
 *
 * Asserts that address is a valid memory address within the range
 * [0,MAX_MEMORY]. 
 *
 */
void checkMemoryBounds( int address ) {
  if ( address < 0 || address > MAX_MEMORY ) {
    printf( MEM_OUT_OF_BOUNDS_MSG, address );
    exit( -1 );
  }
}


/* ---------------------------------------------------------------------
 * General purpose register operations
 * ---------------------------------------------------------------------
 */

/* 
 * void setRegister(int reg_address, int value)
 * 
 * registers[reg_address] = value; sets conditions codes
 * on every register write too; outputs actions when debug
 * is asserted.
 * 
 */
void setRegister( int reg_address, int value ) {
  checkRegisterAddressBounds( reg_address );
  registers[reg_address] = maskTo16Bits( value );
	setConditionCodes(reg_address); //I like it here, but keep in mind mebby bad
  if ( debug )
    printf( SET_REG_MSG, reg_address, maskTo16Bits( value ) );
}

/* 
 * int getRegister( int reg_address )
 * 
 * Returns the value in register reg_address
 * 
 */
int getRegister( int reg_address ) {
  checkRegisterAddressBounds( reg_address );
  return registers[reg_address];
}

/*
 * void resetAllRegisters()
 *
 * Sets all registers to 0;
 *
 */
void resetAllRegisters() {
  int i = 0;
  while( i < 8 )
    setRegister( i++, 0 );
}

/* 
 * checkRegisterAddressBounds( int reg_address )
 *
 * exit( -1 ) if reg_address is not in the range [0,7]
 * 
 */
void checkRegisterAddressBounds( int reg_address ) {
  if ( reg_address < 0 || reg_address > 7 ) {
    printf( REG_ADDRESS_MSG, reg_address );
    exit( -1 );
  };
}


/* ---------------------------------------------------------------------
 * Program Counter (PC) operations
 * ---------------------------------------------------------------------
 */

/*
 * void setPC( int address );
 *
 * Sets the PC to a new address.  Use checkMemoryBounds to make sure
 * a valid address is passed in before assigning the PC.
 *
 */
void setPC( int address ) {
	pc = address;
}

/*
 * int getPC()
 *
 * Returns the current value of PC
 *
 */
int getPC() {
  return pc;
}


/* ---------------------------------------------------------------------
 * Condition Code Register operations
 * ---------------------------------------------------------------------
 */

/* 
 * void setConditionCodes( int reg_address )
 * 
 * Set condition codes negative (n), zero (z), and postive (p) depending
 * on the contents of the register specified by reg_address.
 * 
 * Globals: n, z, and p
 *
 */
void setConditionCodes( int reg_address ) {
	//printf("Reg: %d\n",reg_address);
  int value = signExtend16Bit( getRegister( reg_address ) );
	//printf("Val: %x\n",value);
  if(value > 0) { n = FALSE; z = FALSE; p = TRUE; }
	else if(value == 0) { n = FALSE; z = TRUE; p = FALSE; }
	else if(value < 0) { n = TRUE; z = FALSE; p = FALSE; } 
  return;
}

/*
 * int getN()
 *
 * Returns global variable n
 */
int getN() {
  return n;
}

/*
 * int getZ()
 *
 * Returns global variable z
 */
int getZ() {
  return z;
}

/*
 * int getP()
 *
 * Returns global variable p
 */
int getP() {
  return p;
}


/* ---------------------------------------------------------------------
 * Instruction Register (IR) operations
 * ---------------------------------------------------------------------
 */

/*
 * int setIR( int instruction )
 *
 * ir = instruction;
 *
 */
void setIR( int instruction ) {
  ir = instruction;
}

/*
 *
 * int getIR()
 * 
 * Returns the value of the global variable ir
 *
 */
int getIR() {
  return ir;
}

/* 
 * int getDR()
 * 
 * Returns a value in the range [0,7] indicating
 * the DR register defined by bits[11-9] in the global ir.
 *
 * Masks: BITS_11_9_MASK
 *
 * Gobals: ir
 *
 */
int getDR() {
  return (getIR() & BITS_11_9_MASK) >> 9;
}

/* 
 * int getSR()
 * 
 * Returns a value in the range [0,7] indicating
 * the SR register defined by bits[11-9] in the global ir.
 *
 * Masks: BITS_11_9_MASK
 *
 * Gobals: ir
 *
 */
int getSR() {
  return (getIR() & BITS_11_9_MASK) >> 9;
}

/* 
 * int getOpcode()
 *
 * Returns the value of bits[15-12] in the IR.  The bits are sifted
 * left so they range over [0,15].
 *
 */
int getOpcode() {
  /* op-code is top 4 bits, do logical AND of ir with  1111000000000000, 
   * and shift left 12 bits.
   */
  return ( ( getIR() & BITS_15_12_MASK ) >> 12);
}

/* 
 * int getSR1()
 * 
 * Returns a value in the range [0,7] indicating
 * the SR1 register defined by bits[8-6] in the global ir.
 *
 * Masks: BITS_8_6_MASK
 *
 * Globals: ir
 *
 */
int getSR1() {
	return (getIR() & BITS_8_6_MASK) >> 6;
}

/* 
 * int getSR2()
 *
 * Returns a value in the range [0,7] indicating
 * the SR2 register defined by bits[2-0] in the global ir.
 *
 * Masks: BITS_2_0_MASK
 *
 * Gobals: ir
 *
 * Help: ADD and AND instructions (chapter 5 text)
 *
 */
int getSR2() {
  return (getIR() & BITS_2_0_MASK);
}

/* 
 * int getBaseR()
 * 
 * Returns a value in the range [0,7] indicating
 * the base register defined by bits[8-6] in the global ir.
 *
 * Masks: BITS_8_6_MASK
 *
 * Globals: ir
 *
 */
int getBaseR() {
  return (getIR() & BITS_8_6_MASK) >> 6;
}

/* 
 * int getImmediateBit()
 *
 * Returns a nonzero value if bit[5] is set in the IR otherwise
 * it returns zero.  bit[5] is the bit that indicates an immediate
 * value in direct addressing modes.
 *
 * Masks: BIT_5_MASK 0x0020
 * 
 * Help: See the ADD (page 526) and AND (page 527) instruction 
 * specifications
 *
 */
int getImmediateBit() {
  return (getIR() & BIT_5_MASK) >> 5;
}

/* 
 * int getAddrBit()
 * 
 * Returns nonzero if bit [11] (the addressing mode bit) 
 * is set in the global IR
 *
 * Help: JSR/JSRR instruction (pgs. 232-234 text)
 */
int getAddrBit() {
  return (getIR() & BIT_11_MASK) >> 11;
}

/* 
 * int getImm5()
 *
 * Returns the value of bits[4-0] in the instruction register
 *
 * Masks: BITS_4_0_MASK
 *
 * Help: ADD and AND instructions (chapter 5 text)
 *
 */
int getImm5() {
  return (getIR() & BITS_4_0_MASK);
}

/* 
 * int getOffset6()
 *
 * Returns the value of bits[5-0] in the IR
 *
 * Mask: BITS_5_0_MASK
 *
 * Help: LDR, STR instructions (chapter 5 text)
 */
int getOffset6() {
  return (getIR() & BITS_5_0_MASK);
}

/* 
 * int getOffset9()
 *
 * Returns the value of bits[8-0] in the IR
 *
 * Mask: BITS_8_0_MASK
 *
 * Help: BR, LD, LDI, LEA, ST, STI instructions
 */
int getOffset9() {
  return (getIR() & BITS_8_0_MASK);
}

/* 
 * int getOffset11()
 *
 * Returns the value of bits[10-0] in the IR
 *
 * Mask: BITS_10_0_MASK
 *
 * Help: JSR instruction
 */
int getOffset11() {
  return (getIR() & BITS_10_0_MASK);
}

/* 
 * int getTrapvect8()
 *
 * Returns the value of bits[7-0] in the IR
 * 
 * Masks: BITS_7_0_MASK
 *
 * Help: TRAP (page 137, chapter 9 text)
 *
 */
int getTrapvect8() {
  return (getIR() & BITS_7_0_MASK);
}

/* 
 * int getIRN()
 * 
 * Returns a nonzero value if bit[11] is set in the ir
 *
 * Mask: BIT_11_MASK
 *
 * Help: page 120 of text.
 *
 */
int getIRN() {
  return (getIR() & BIT_11_MASK) >> 11;
}

/* 
 * int getIRZ()
 * 
 * Returns a nonzero value if bit[10] is set in the ir
 *
 * Mask: BIT_10_MASK
 *
 * Help: page 120 of text.
 *
 */
int getIRZ() {
  return (getIR() & BIT_10_MASK) >> 10;
}

/* 
 * int getIRP()
 * 
 * Returns a nonzero value if bit[9] is set in the ir
 *
 * Mask: BIT_9_MASK
 *
 * Help: page 120 of text.
 *
 */
int getIRP() {
  return (getIR() & BIT_9_MASK) >> 9;
}


/* ---------------------------------------------------------------------
 * Utility Functions
 * ---------------------------------------------------------------------
 */

/* This resets LC3 to its initial state. */
void resetLC3() {
  ir = 0;               /* Instruction register starts out with 0      */
  pc = 0;               /* Always fetch first instruction from address 0 */
  resetAllRegisters();  /* Reset all general-purpose registers         */
  n = z = p = 0;        /* Set all condition codes to 0                */
  halt = 0;             /* You are not halted after reset              */
}

/* 
 * void loadProgram( char* filename )
 *
 * Opens the file named filename and loads the values sequentially
 * into memory starting at the specified location.
 *
 * Assumptions: according to the mco file format, the first line
 * is the memory address where the program is loaded. You
 * can ASSUME that this value is set to be within your memory
 * bounds, but you need to set the PC appropriately.  You
 * can also ASSUME that your input file will be a valid hex mco format.
 *
 * Error checking: if filename cannot be opened for reading, then
 * output BAD_FILENAME_MSG and exit( -1 )
 * 
 * Functions: setPC and setMemoryData
 *
 */
void loadProgram( char* filename ) {
	///*
	FILE* infile = fopen(filename, "r");
	if(!infile) { //is null
		printf(BAD_FILENAME_MSG,filename);
		exit(-1);
	}
	
	//now how to read from a file??? - done
	int line;
	if(fscanf(infile,"%x",&line) == EOF) {
		printf("File %s is empty.",filename);
		exit(-1);
	}
	int mem = line;
	setPC(mem);
	while(fscanf(infile,"%x",&line) != EOF) {
		//printf("%d,%x\n",line,line);
		setMemoryData(mem,line);
		mem++;
	}
	//*/
}

/* 
 * void fetchInstruction()
 *
 * This function goes to memory at the address in PC and puts the memory
 * contents in the IR register afterwhich it increments the value of the PC.
 *
 * Functions: getPC, setIR, getMemoryData, and setPC
 *
*/
void fetchInstruction() {
  setIR(getMemoryData(getPC()));
	setPC(getPC()+1);
}

/* 
 * int signExtendImm5(int value)
 *
 * This function copies bit[4] of value into bits[31-5] 
 * of value and returns the resulting value.
 * This function will not work for anything larger than 32-bits due
 * to the mask definition.
 *
 * Masks: BIT_4_MASK and BITS_31_5_MASK
 *       
 * Help: ADD and AND instructions
 *
 */
int signExtendImm5( int value ) {
  return ((value & BIT_4_MASK)?BITS_31_5_MASK | value:value);
}

/* 
 * int signExtendOffset6(int value)
 *
 * This function copies bit[5] of value into bits[31-6] 
 * of value and returns the resulting value.
 * This function will not work for anything larger than 32-bits due
 * to the mask definition.
 *
 * Masks: BIT_5_MASK and BITS_31_6_MASK
 *       
 * Help: LDR and STR instructions
 *
 */
int signExtendOffset6( int value ) {
  return ((value & BIT_5_MASK)?BITS_31_6_MASK | value:value);
}

/* 
 * int signExtendOffset9(int value)
 *
 * Masks: BIT_8_MASK and BITS_31_9_MASK
 *       
 * Help: BR, LD, LDI, LEA, ST and STI instructions
 *
 */
int signExtendOffset9( int value ) {
  return ((value & BIT_8_MASK)?BITS_31_9_MASK | value:value);
}

/* 
 * int signExtendOffset11(int value)
 *
 * Masks: BIT_10_MASK and BITS_31_11_MASK
 *       
 * Help: JSR instruction
 *
 */
int signExtendOffset11( int value ) {
  return ((value & BIT_10_MASK)?BITS_31_11_MASK | value:value);
}

/* 
 * int signExtend16Bit( int value )
 * 
 * Sign extends the bit 15 to make a valid 32-bit number; this
 * function is useful when dealing with the 16-bit internal LC-3 number
 * and the 32-bit default integer size in IA-32.
 * 
 */
int signExtend16Bit( int value ) {
  return ( ( value & BIT_15_MASK ) ? BITS_31_16_MASK | value : value );
}

/* 
 * int maskTo16Bits( int value )
 *
 * The LC-3 word is 16-bits wide; this function truncates 32-bit numbers
 * to 16-bit numbers to match the LC-3 word definition.
 * 
 */
int maskTo16Bits( int value ) {
  return( BITS_15_0_MASK & value );
}

void dumpState() {
  int i;
  /* Makes it easier to see where printout starts                      */
  printf("+++++++++++++++++++++ start state dump +++++++++++++++++\n");
  for (i=0; i<8; i++) {  /* Print out all of the registers             */
    printf("R%d: %#x\n", i, getRegister(i));
  }
  /* Print out the NZP condition codes                                 */
  printf("N: %d, Z: %d, P: %d\n", n, z, p);
  printf("PC: %#x\n", pc);       /* Print out the PC                    */
  printf("IR: %#x\n", ir);       /* Print out the IR                    */
  /* Makes it easier to see where the printout stops                   */
  printf("--------------------- end state dump -------------------\n");
}

/*
 * void add_instruction() 
 *
 * Executes the ADD instruction for both register and immediate addressing
 * modes
 *
 */
void add_instruction() {
  int DR = getDR();
  if ( !getImmediateBit() ) {
    if ( debug )
      printf( ADD_MSG, DR, getSR1(), getSR2() );
    setRegister( DR, 
		 getRegister( getSR1() ) + getRegister( getSR2() ) );
  } 
  else {
    if ( debug )
      printf( ADDI_MSG, DR, getSR1(), 
	      maskTo16Bits( signExtendImm5( getImm5() ) ) );
    setRegister( DR, 
		 getRegister( getSR1() ) + signExtendImm5( getImm5() ) );
  }
  //setConditionCodes( DR );
}

/*
 * void and_instruction() 
 *
 * Executes the AND instruction for both register and immediate addressing
 * modes
 * 
 * Basically a copy of the above function
 *
 */
void and_instruction() {
  int DR = getDR();
  if ( !getImmediateBit() ) {
    if ( debug )
      printf( AND_MSG, DR, getSR1(), getSR2() );
    setRegister( DR, 
		 getRegister( getSR1() ) & getRegister( getSR2() ) );
  } 
  else {
    if ( debug )
      printf( ANDI_MSG, DR, getSR1(), 
	      maskTo16Bits( signExtendImm5( getImm5() ) ) );
    setRegister( DR, 
		 getRegister( getSR1() ) & signExtendImm5( getImm5() ) );
  }
  //setConditionCodes( DR );
}

/*
 * void executeInstruction()
 *
 * This is the heart of the simulator. It consists of a switch
 * statement that contains the code for executing each instruction.
 * The add instruction is provided as an example of how to code
 * up all of the instructions.
 * Help: instruction processing (pgs. 103-107 text)
 * Help:  list of instructions (page 119, back cover of text)
 */
void executeInstruction() {
  int tmp;
  int string_address;
	int addy;
  switch( getOpcode() ) {
  case ADD:
    add_instruction();
    break;
  case AND:
    and_instruction();
    break;
  case BR:
		addy = getPC() + signExtendOffset9(getOffset9());
	///*
		if(debug) {
			char codes[4]; //might work
			strcpy(codes,"");
			if(getIRN()) strcat(codes,"n");
			if(getIRZ()) strcat(codes,"z");
			if(getIRP()) strcat(codes,"p");
			printf(BR_MSG,codes,addy);
		}
	//*/
    if((getIRN() && getN()) || (getIRZ() && getZ()) || (getIRP() && getP()))
			setPC(addy);
    break;
  case JMP: /* Note: This case should handle JMP and RET */
    /* Functions: getBaseR, setPC and getRegister
     */
    if(debug) {
			int reg = getBaseR();
			if(reg != 7)
				printf(JMP_MSG,reg);
			else
				printf(RET_MSG);
		}
		setPC(getRegister(getBaseR()));
    break;
  case JSR:     /* Note: This case should handle JSR and JSRR */
		setRegister(7,getPC()); //subroutine, so back it up
    if(getAddrBit()) { //JSR
			int addy = getPC() + signExtendOffset11(getOffset11());
			if(debug)
				printf(JSR_MSG,addy);
			setPC(addy);
		}
		else { //JSRR
			addy = getRegister(getBaseR());
			if(debug)
				printf(JSRR_MSG,getBaseR());
			setPC(addy);
		}
    break;
  case LD:
    addy = getPC() + signExtendOffset9(getOffset9());
		if(debug)
			printf(LD_MSG,getDR(),addy);
		setRegister(getDR(),getMemoryData(addy));
    break;
  case LDI:
	  addy = getMemoryData(getPC() + signExtendOffset9(getOffset9()));
		if(debug)
			printf(LDI_MSG,getDR(),addy);
		setRegister(getDR(),getMemoryData(addy));
    break;
  case LDR:
    addy = getRegister(getBaseR()) + signExtendOffset6(getOffset6());
		if(debug)
			printf(LDR_MSG,getDR(),getBaseR(),addy);
		setRegister(getDR(),getMemoryData(addy));
    break;
  case LEA:
		addy = getPC() + signExtendOffset9(getOffset9());
    if(debug)
			printf(LEA_MSG,getDR(),addy);
		setRegister(getDR(),addy);
    break;
  case NOT:
    if(debug)
			printf(NOT_MSG,getDR(),getSR1());
		setRegister(getDR(),~getRegister(getSR1()));
    break;
  case ST:
    if ( debug )
      printf( ST_MSG, getSR(), getPC() + signExtendOffset9( getOffset9() ) );
		setMemoryData( getPC() + signExtendOffset9( getOffset9() ),  
									getRegister( getSR() ) );
    break;
  case STI:
		addy = getMemoryData(getPC()+signExtendOffset9(getOffset9()));
    if(debug)
			printf(STI_MSG, getSR(), addy);
		setMemoryData(addy,getSR());
		break;
  case STR:
		addy = getRegister(getBaseR()) + signExtendOffset6(getOffset6());
		if(debug)
			printf(STR_MSG,getSR(),getBaseR(),addy);
		setMemoryData(addy,getRegister(getSR()));
    break;
  case TRAP:
    switch(getTrapvect8()) {
    case GETC:
	  /* Note: This does not function quite like GETC.
	   *       See the lab web page for details.
	   */
      if ( debug )
				printf( "GETC\n" );
      setRegister(0, getchar());
      setRegister(7, pc);
      break;
    case OUT:
      if ( debug )
		printf( "OUT\n" );
      putchar(getRegister(0));
      setRegister(7, pc);
      break;
    case PUTS:
      if ( debug )
		printf( "PUTS\n" );
      string_address = getRegister(0);
      while((tmp=getMemoryData(string_address++)))
		putchar(tmp);
      setRegister(7, pc);
      break;
    case IN:
	  /* Note: This does not function quite like IN.
	   *       See the lab web page for details.
	   */
      if ( debug )
		printf( "IN\n" );
      putchar(':');
      setRegister(0, getchar());
      setRegister(7, pc);
      break;
    case HALT:
      if ( debug )
		printf( "HALT\n" );
      setRegister(7, pc);
      halt = 1;  /* Halt after this instruction */
      break;
    default:
      printf( TRAP_ERROR_MSG, getTrapvect8() );
    }
    break;
  default: 
	printf( UNDEFINED_OPCODE_MSG, getOpcode() );
  }
} 
