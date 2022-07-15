#pragma once
#include <vector>
#include <variant>
namespace Utils
{
	inline bool GetBit( uint16_t iData, int iBitNumber )
	{
		return ( ( bool ) ( ( 1 << iBitNumber ) & iData ) );
	}
	inline void SetBit( uint16_t& iData, bool bBitValue, int iBitNumber )
	{
		if ( bBitValue )
			iData |= ( 1 << iBitNumber );
		else
			iData &= ~( 1 << iBitNumber );

	}

	inline void SetBit( uint8_t& iData, bool bBitValue, int iBitNumber )
	{
		if ( bBitValue )
			iData |= ( 1 << iBitNumber );
		else
			iData &= ~( 1 << iBitNumber );

	}
	inline void InvertBit( uint8_t& iData, int iBitNumber )
	{
		iData ^= ~( 1 << iBitNumber );
	}

	inline void CopyLowBitsToOtherNumber( uint8_t& iFirstNumber, uint8_t& iSecondNumber )
	{
		for ( int iBitNumber = 0; iBitNumber < 4; iBitNumber++ )
			SetBit( iSecondNumber, GetBit( iFirstNumber, iBitNumber ), iBitNumber );
	}

	inline void CopyLowBitsToOtherNumber( uint16_t& iFirstNumber, uint16_t& iSecondNumber )
	{
		for ( int iBitNumber = 0; iBitNumber < 8; iBitNumber++ )
			SetBit( iSecondNumber, GetBit( iFirstNumber, iBitNumber ), iBitNumber );
	}

	inline void AddLowBitsToOtherNumber( uint16_t& iFirstNumber, uint16_t& iSecondNumber )
	{
		int16_t iTempValue = iSecondNumber;

		iTempValue += iFirstNumber;


		for ( int iBitNumber = 0; iBitNumber < 8; iBitNumber++ )
			SetBit( iSecondNumber, GetBit( iTempValue, iBitNumber ), iBitNumber );
	}

	inline void AddLowBitsToOtherNumber( uint8_t& iFirstNumber, uint8_t& iSecondNumber )
	{
		int16_t iTempValue = iSecondNumber;

		iTempValue += iFirstNumber;


		for ( int iBitNumber = 0; iBitNumber < 4; iBitNumber++ )
			SetBit( iSecondNumber, GetBit( iTempValue, iBitNumber ), iBitNumber );
	}
}