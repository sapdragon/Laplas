#include <cstdint>
#include  <vector>
#include "FlagsManager.hpp"

#define STATIC_MEMORY_PAGES 8
#define MEMORY_PAGE_SIZE 256

enum AddressingMode_t
{
	Implied = 2,
	Accumulator = 8,
	Immediate,
	Absolute = 3,
	ZeroPage = 1,
	AbsoluteX = 6,
	AbsoluteY = 7,
	ZeroPageX = 5,
	ZeroPageY = 13,
	InDirectX = 0,
	InDirectY = 4,
	AbsoluteInDirect,
	Relative = 12,
};

struct MemoryPage_t
{
	uint8_t m_aMemory[256];
};

class C_ProcessorContext
{
	public:
	/*       Static RAM    */
    uint8_t m_aStaticMemory[ MEMORY_PAGE_SIZE *STATIC_MEMORY_PAGES];
	
	/*     Registers */
	uint8_t m_nAccumulatorRegister;
	uint8_t m_nIndexXRegister;
	uint8_t m_nIndexYRegister;
	
	/*  Pointers */
	uint8_t m_nStackPointer;
	uint16_t m_nInstructionPointer;
	
     uint8_t m_nProcessorStatus;
};

class C_Processor : private C_ProcessorContext
{
public:
	/* Registers read/write */
	void LoadAccumulatorWithMemory( AddressingMode_t iMode, uint16_t nAddress);
	void LoadIndexXWithMemory( AddressingMode_t iMode, uint16_t nAddress);
	void LoadIndexYWithMemory( AddressingMode_t iMode, uint16_t nAddress);
	   
	void StoreIndexYInMemory( AddressingMode_t iMode, uint16_t nAddress);
	void StoreIndexXInMemory( AddressingMode_t iMode, uint16_t nAddress);
	void StoreAccumulatorInMemory( AddressingMode_t iMode, uint16_t nAddress);
	   
	void TransferAccumulatorToIndexX();
	void TransferAccumulatorToIndexY();
	void TransferIndexXToAccumulator();
	void TransferIndexYToAccumulator();
	   
	void TransferStackPointerToIndexX();
	void TransferIndexXToStackPointer();
	  
	  /* Arithmetic bits */
	void AndWithAccumulator( AddressingMode_t iMode, uint16_t nAddress);
	void XorWithAccumulator( AddressingMode_t iMode, uint16_t nAddress);
	void OrWithAccumulator( AddressingMode_t iMode, uint16_t nAddress);	  

	/* Increment / Decrement index register*/
	void IncrementIndexX();
	void DecrementIndexX();

	void IncrementIndexY();
	void DecrementIndexY();
	  
	/* Increment / Decrement value in memory*/
	void IncrementMemoryByOne(AddressingMode_t iMode, uint16_t nAddress);
	void DecrementMemoryByOne(AddressingMode_t iMode, uint16_t nAddress);
	  
	/* Compare instructuons */
	void CompareMemoryWithAccumulator(AddressingMode_t iMode, uint16_t nAddress); 
	void CompareMemoryWithIndexX(AddressingMode_t iMode, uint16_t nAddress); 
	void CompareMemoryWithIndexY(AddressingMode_t iMode, uint16_t nAddress);
	  
	/* Rotate bits*/
	void ShiftOneBitRight(AddressingMode_t iMode, uint16_t nAddress);	 
	void ShiftOneBitLeft(AddressingMode_t iMode, uint16_t nAddress);
	 	 
	void RotateOneBitRight(AddressingMode_t iMode, uint16_t nAddress);	 
	void RotateOneBitLeft(AddressingMode_t iMode, uint16_t nAddress);

private:
	/* Why need SEC, CLC, CLV etc..*/
	C_FlagManager m_cFlagManager;
	
	/* Get the specified number of bytes from memory at the address */
    std::vector<uint8_t> GetDataFromMemory(uint16_t nAddress, int iSize = 1);
	
	/* Set the specified number of bytes to memory at the address */
    void SetDataToMemory(uint16_t nAddress, std::vector<uint8_t> vecData);
	
	/* Get data depending on the addressing mode  */
	uint16_t GetDataFromMnemonic(AddressingMode_t iMode, uint16_t nMnemonic);
	void SetDataFromMnemonic(AddressingMode_t iMode, uint16_t nMnemonic, std::vector<uint8_t> vecData);
};