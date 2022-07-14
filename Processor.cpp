#include "Processor.hpp"

std::vector<uint8_t> C_Processor::GetDataFromMemory( uint16_t nAddress, int iSize )
{
	/* Validation */
	if ( nAddress > 2048 || nAddress < 0 || iSize <= 0 || iSize > 2048 )
		throw "GetDataFromMemory | Invalid";
	std::vector<uint8_t> vecResult;


	/* Copy all bytes */
	for ( int iCurrentByte = 0; iCurrentByte != iSize; iCurrentByte++ )
		vecResult.push_back( this->m_aStaticMemory[ nAddress + iCurrentByte ] );

	return vecResult;
}

void C_Processor::SetDataToMemory( uint16_t nAddress, std::vector<uint8_t> vecData )
{
	/* Copy bytes to RAM*/
	for ( int iCurrentByte = 0; iCurrentByte != vecData.size( ); iCurrentByte++ )
		this->m_aStaticMemory[ iCurrentByte + nAddress ] = vecData.at( iCurrentByte );

}

uint16_t C_Processor::GetDataFromMnemonic( AddressingMode_t iMode, uint16_t nMnemonic = 0 )
{
	int16_t nResult = 0;

	switch ( iMode )
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
		nResult = GetDataFromMemory( nMnemonic ).at( 0 );
		break;
		/* Getting data from zero page ( first 256 bytes ) */
		case ZeroPage:
		if ( nMnemonic > 256 )
			throw "Mnemonic > 256";
		nResult = GetDataFromMemory( nMnemonic ).at( 0 );

		break;

		/*  Other mods soon...*/
	}
	return nResult;
}


void C_Processor::SetDataFromMnemonic( AddressingMode_t iMode, uint16_t nMnemonic, std::vector<uint8_t> vecData )
{
	switch ( iMode )
	{
		/* Getting data directly from the mnemonic*/
		case Implied:
		SetDataToMemory( nMnemonic, vecData );
		break;
		/* Getting data from the Accumulator*/
		case Accumulator:
		SetDataToMemory( Accumulator, vecData );
		break;
		/* Getting data from the Static RAM */
		case Absolute:
		SetDataToMemory( nMnemonic, vecData );
		break;
		/* Getting data from zero page ( first 256 bytes ) */
		case ZeroPage:
		if ( nMnemonic > 256 )
			throw "Mnemonic > 256";
		SetDataToMemory( nMnemonic, vecData );

		break;

		/*  Other mods soon...*/
	}
}


void C_Processor::LoadAccumulatorWithMemory( AddressingMode_t iMode, uint16_t nAddress )
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic( iMode, nAddress );

	/* Load Memory to Accumulator*/
	this->m_nAccumulatorRegister = iData;
}

void C_Processor::LoadIndexXWithMemory( AddressingMode_t iMode, uint16_t nAddress )
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic( iMode, nAddress );

	/* Load Memory to Index X*/
	this->m_nIndexXRegister = iData;
}

void C_Processor::LoadIndexYWithMemory( AddressingMode_t iMode, uint16_t nAddress )
{
	/*  Get data from address*/
	auto iData = GetDataFromMnemonic( iMode, nAddress );

	/* Load Memory to Index Y*/
	this->m_nIndexYRegister = iData;
}

void C_Processor::StoreIndexYInMemory( AddressingMode_t iMode, uint16_t nAddress )
{
	SetDataFromMnemonic( iMode, nAddress, std::vector<uint8_t>{this->m_nIndexYRegister} );
}
void C_Processor::StoreIndexXInMemory( AddressingMode_t iMode, uint16_t nAddress )
{
	SetDataFromMnemonic( iMode, nAddress, std::vector<uint8_t>{this->m_nIndexXRegister} );
}
void C_Processor::StoreAccumulatorInMemory( AddressingMode_t iMode, uint16_t nAddress )
{
	SetDataFromMnemonic( iMode, nAddress, std::vector<uint8_t>{this->m_nAccumulatorRegister} );
}

void C_Processor::TransferAccumulatorToIndexX( )
{
	this->m_nIndexXRegister = this->m_nAccumulatorRegister;
}
void C_Processor::TransferAccumulatorToIndexY( )
{
	this->m_nIndexYRegister = this->m_nAccumulatorRegister;
}
void C_Processor::TransferIndexXToAccumulator( )
{
	this->m_nAccumulatorRegister = this->m_nIndexXRegister;
}
void C_Processor::TransferIndexYToAccumulator( )
{
	this->m_nAccumulatorRegister = this->m_nIndexYRegister;
}

void C_Processor::TransferStackPointerToIndexX( )
{
	this->m_nIndexXRegister = this->m_nStackPointer;
}
void C_Processor::TransferIndexXToStackPointer( )
{
	this->m_nStackPointer = this->m_nIndexXRegister;
}