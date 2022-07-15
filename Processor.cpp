#include "Processor.hpp"
#include "../Utils/Utils.hpp"

std::vector<uint8_t> C_Processor::GetDataFromMemory(uint16_t nAddress, int iSize)
{
	/* Validation */
	if (nAddress > 2048 || nAddress < 0 || iSize <= 0 || iSize > 2048)
		throw "GetDataFromMemory | Invalid";
	std::vector<uint8_t> vecResult;

	/* Copy all bytes */
	for (int iCurrentByte = 0; iCurrentByte != iSize; iCurrentByte++)
		vecResult.push_back(this->m_aStaticMemory[nAddress + iCurrentByte]);

	return vecResult;
}

void C_Processor::SetDataToMemory(uint16_t nAddress, std::vector<uint8_t> vecData)
{
	/* Copy bytes to RAM*/
	for (int iCurrentByte = 0; iCurrentByte != vecData.size(); iCurrentByte++)
		this->m_aStaticMemory[iCurrentByte + nAddress] = vecData.at(iCurrentByte);
}

uint16_t C_Processor::GetDataFromMnemonic(AddressingMode_t iMode, uint16_t nMnemonic = 0)
{
	int16_t nResult = 0;

	switch (iMode)
	{
	/* Getting data directly from the mnemonic*/
	case Implied:
		nResult = nMnemonic;
		break;
		/* Getting data from the Accumulator*/
	case Accumulator:
		nResult = this->m_nAccumulatorRegister;
		break;
	/* Getting data from the Static RAM */
	case Absolute:
		nResult = GetDataFromMemory(nMnemonic).at(0);
		break;
		/* Getting data from zero page ( first 256 bytes ) */
	case ZeroPage:
		if (nMnemonic > 256)
			throw "Mnemonic > 256";
		nResult = GetDataFromMemory(nMnemonic).at(0);
		break;
		/* Is absolute addresation, and address = mnemonic + x register */
	case AbsoluteX:
		nResult = GetDataFromMemory(nMnemonic + this->m_nIndexXRegister).at(0);
		break;
	/* Is absolute addresation, and address = mnemonic + y register */
	case AbsoluteY:
		nResult = GetDataFromMemory(nMnemonic + this->m_nIndexXRegister).at(0);
		break;
	/* is zero page addresation and address = mnemonic + low bits x register*/
	case ZeroPageX:
		uint8_t iZeroMnemonic = nMnemonic;
		Utils::AddLowBitsToOtherNumber(this->m_nIndexXRegister, iZeroMnemonic);
		nResult = GetDataFromMemory(iZeroMnemonic).at(0);
		break;
		/* is zero page addresation and address = mnemonic + low bits y register*/
	case ZeroPageY:
		uint8_t iZeroMnemonic = nMnemonic;
		Utils::AddLowBitsToOtherNumber(this->m_nIndexYRegister, iZeroMnemonic);
		nResult = GetDataFromMemory(iZeroMnemonic).at(0);
		break;
	}
	return nResult;
}

void C_Processor::SetDataFromMnemonic(AddressingMode_t iMode, uint16_t nMnemonic, std::vector<uint8_t> vecData)
{
	switch (iMode)
	{
	/* Getting data directly from the mnemonic*/
	case Implied:
		SetDataToMemory(nMnemonic, vecData);
		break;
		/* Getting data from the Accumulator*/
	case Accumulator:
		SetDataToMemory(Accumulator, vecData);
		break;
	/* Getting data from the Static RAM */
	case Absolute:
		SetDataToMemory(nMnemonic, vecData);
		break;
		/* Getting data from zero page ( first 256 bytes ) */
	case ZeroPage:
		if (nMnemonic > 256)
			throw "Mnemonic > 256";
		SetDataToMemory(nMnemonic, vecData);

		break;

		/* Is absolute addresation, and address = mnemonic + x register */
	case AbsoluteX:
		SetDataToMemory(nMnemonic + this->m_nIndexXRegister, vecData);
		break;
	/* Is absolute addresation, and address = mnemonic + y register */
	case AbsoluteY:
		SetDataToMemory(nMnemonic + this->m_nIndexYRegister, vecData);
		break;
	/* is zero page addresation and address = mnemonic + low bits x register*/
	case ZeroPageX:
		uint8_t iZeroMnemonic = nMnemonic;
		Utils::AddLowBitsToOtherNumber(this->m_nIndexXRegister, iZeroMnemonic);
		SetDataToMemory(iZeroMnemonic, vecData);
		break;
		/* is zero page addresation and address = mnemonic + low bits y register*/
	case ZeroPageY:
		uint8_t iZeroMnemonic = nMnemonic;
		Utils::AddLowBitsToOtherNumber(this->m_nIndexYRegister, iZeroMnemonic);
		SetDataToMemory(iZeroMnemonic, vecData);
		break;
	}
}

void C_Processor::LoadAccumulatorWithMemory(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Reset flags*/
	this->m_cFlagManager.SetFlag(ZeroResult, false);

	/* check if data zero */
	if (!iData || iData == 0)
		this->m_cFlagManager.SetFlag(ZeroResult, true);

	/* Update negative flag*/

	bool bNegative = Utils::GetBit(iData, 7);

	this->m_cFlagManager.SetFlag(NegativeResult, bNegative);

	/* Load Memory to Accumulator*/
	this->m_nAccumulatorRegister = iData;
}

void C_Processor::LoadIndexXWithMemory(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Load Memory to Index X*/
	this->m_nIndexXRegister = iData;
}

void C_Processor::LoadIndexYWithMemory(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Load Memory to Index Y*/
	this->m_nIndexYRegister = iData;
}

void C_Processor::StoreIndexYInMemory(AddressingMode_t iMode, uint16_t nAddress)
{
	SetDataFromMnemonic(iMode, nAddress, std::vector<uint8_t>{this->m_nIndexYRegister});
}

void C_Processor::StoreIndexXInMemory(AddressingMode_t iMode, uint16_t nAddress)
{
	SetDataFromMnemonic(iMode, nAddress, std::vector<uint8_t>{this->m_nIndexXRegister});
}

void C_Processor::StoreAccumulatorInMemory(AddressingMode_t iMode, uint16_t nAddress)
{
	SetDataFromMnemonic(iMode, nAddress, std::vector<uint8_t>{this->m_nAccumulatorRegister});
}

void C_Processor::TransferAccumulatorToIndexX()
{
	this->m_nIndexXRegister = this->m_nAccumulatorRegister;
}

void C_Processor::TransferAccumulatorToIndexY()
{
	this->m_nIndexYRegister = this->m_nAccumulatorRegister;
}

void C_Processor::TransferIndexXToAccumulator()
{
	this->m_nAccumulatorRegister = this->m_nIndexXRegister;
}

void C_Processor::TransferIndexYToAccumulator()
{
	this->m_nAccumulatorRegister = this->m_nIndexYRegister;
}

void C_Processor::TransferStackPointerToIndexX()
{
	this->m_nIndexXRegister = this->m_nStackPointer;
}

void C_Processor::TransferIndexXToStackPointer()
{
	this->m_nStackPointer = this->m_nIndexXRegister;
}

void C_Processor::AndWithAccumulator(AddressingMode_t iMode, uint16_t nAddress)
{
	uint8_t iValue = GetDataFromMnemonic(iMode, nAddress);

	this->m_nAccumulatorRegister &= iValue;
}

void C_Processor::XorWithAccumulator(AddressingMode_t iMode, uint16_t nAddress)
{
	uint16_t iValue = GetDataFromMnemonic(iMode, nAddress);

	this->m_nAccumulatorRegister |= iValue;
}

void C_Processor::OrWithAccumulator(AddressingMode_t iMode, uint16_t nAddress)
{
	uint16_t iValue = GetDataFromMnemonic(iMode, nAddress);

	this->m_nAccumulatorRegister ^= iValue;
}

void C_Processor::IncrementIndexX()
{
	this->m_nIndexXRegister++;
}
void C_Processor::DecrementIndexX()
{
	this->m_nIndexXRegister--;
}

void C_Processor::IncrementIndexY()
{
	this->m_nIndexYRegister++;
}

void C_Processor::DecrementIndexY()
{
	this->m_nIndexYRegister--;
}

void C_Processor::IncrementMemoryByOne(AddressingMode_t iMode, uint16_t nAddress)
{

	std::vector<uint8_t> aBytesToInsert;

	/* push bytes to vector*/
	aBytesToInsert.push_back(GetDataFromMnemonic(iMode, nAddress) + 1);
	/* Writes bytes to RAM*/
	SetDataFromMnemonic(iMode, nAddress, aBytesToInsert);
}

void C_Processor::DecrementMemoryByOne(AddressingMode_t iMode, uint16_t nAddress)
{
	std::vector<uint8_t> aBytesToInsert;

	/* push bytes to vector*/
	aBytesToInsert.push_back(GetDataFromMnemonic(iMode, nAddress) - 1);
	/* Writes bytes to RAM*/
	SetDataFromMnemonic(iMode, nAddress, aBytesToInsert);
}

void C_Processor::CompareMemoryWithAccumulator(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Reset flags */
	this->m_cFlagManager.SetFlag(Carry, false);
	this->m_cFlagManager.SetFlag(ZeroResult, false);

	/* Run instruction*/
	auto iResult = this->m_nAccumulatorRegister - iData;

	/* Set flags*/

	this->m_cFlagManager.SetFlag(ZeroResult, iResult == 0 ? true : false);
	this->m_cFlagManager.SetFlag(ZeroResult, iResult > 0 ? true : false);

	/* Update negative flag*/

	bool bNegative = Utils::GetBit(iData, 7);

	this->m_cFlagManager.SetFlag(NegativeResult, bNegative);
}

void C_Processor::CompareMemoryWithIndexX(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Reset flags */
	this->m_cFlagManager.SetFlag(Carry, false);
	this->m_cFlagManager.SetFlag(ZeroResult, false);

	/* Run instruction*/
	auto iResult = this->m_nIndexXRegister - iData;

	/* Set flags*/

	this->m_cFlagManager.SetFlag(ZeroResult, iResult == 0 ? true : false);
	this->m_cFlagManager.SetFlag(ZeroResult, iResult > 0 ? true : false);

	/* Update negative flag*/

	bool bNegative = Utils::GetBit(iData, 7);

	this->m_cFlagManager.SetFlag(NegativeResult, bNegative);
}

void C_Processor::CompareMemoryWithIndexY(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Reset flags */
	this->m_cFlagManager.SetFlag(Carry, false);
	this->m_cFlagManager.SetFlag(ZeroResult, false);

	/* Run instruction*/
	auto iResult = this->m_nIndexYRegister - iData;

	/* Set flags*/

	this->m_cFlagManager.SetFlag(ZeroResult, iResult == 0 ? true : false);
	this->m_cFlagManager.SetFlag(Carry, iResult > 0 ? true : false);

	/* Update negative flag*/

	bool bNegative = Utils::GetBit(iData, 7);

	this->m_cFlagManager.SetFlag(NegativeResult, bNegative);
}

void C_Processor::ShiftOneBitRight(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Reset flags */
	this->m_cFlagManager.SetFlag(NegativeResult, false);
	this->m_cFlagManager.SetFlag(ZeroResult, false);

	/* Shift one byte right*/
	auto iResult = iData >> 1;

	/* Get first bit*/
	auto bFirstBit = Utils::GetBit(iData, 0);

	if (iResult == 0)
		this->m_cFlagManager.SetFlag(ZeroResult, true);

	/* Backup first byte*/
	this->m_cFlagManager.SetFlag(Carry, bFirstBit);

	std::vector<uint8_t> aBytesToWrite;

	aBytesToWrite.push_back(iResult);

	/* Write to memory */
	SetDataFromMnemonic(iMode, nAddress, aBytesToWrite);
}

void C_Processor::ShiftOneBitLeft(AddressingMode_t iMode, uint16_t nAddress)
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic(iMode, nAddress);

	/* Reset flags */
	this->m_cFlagManager.SetFlag(NegativeResult, false);
	this->m_cFlagManager.SetFlag(ZeroResult, false);

	/* Shift one bit left*/
	auto iResult = iData << 1;

	/* Get last bit*/
	auto bLastBit = Utils::GetBit(iData, 7);

	if (iResult == 0)
		this->m_cFlagManager.SetFlag(ZeroResult, true);

	/* Backup last bit*/
	this->m_cFlagManager.SetFlag(Carry, bLastBit);

	this->m_cFlagManager.SetFlag(NegativeResult, Utils::GetBit(iResult, 7));

	std::vector<uint8_t> aBytesToWrite;

	aBytesToWrite.push_back(iResult);

	/* Write to memory */
	SetDataFromMnemonic(iMode, nAddress, aBytesToWrite);
}

void C_Processor::RotateOneBitRight(AddressingMode_t iMode, uint16_t nAddress)
{
	// soon, im lazy..
}

void C_Processor::RotateOneBitLeft(AddressingMode_t iMode, uint16_t nAddress)
{
}