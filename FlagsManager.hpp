#pragma once
#include <variant>
#include "../Utils/Utils.hpp"
enum ProcessorStatuses_t
{
	Carry = 0,
	ZeroResult,
	InterruptDisable,
	DecimalMode,
	Break,
	OverFlow,
	NegativeResult,
};

class C_FlagManager 
{
	public:
	C_FlagManager ( uint8_t& iFlags)
	{
		m_iFlags = iFlags;
	}
	
	void SetFlag(ProcessorStatuses_t iFlag, bool bStatus)
	{
	    Utils::SetBit(this->m_iFlags, iFlag, bStatus);
	}
	bool GetFlagStatus(ProcessorStatuses_t iFlag)
	{
		return Utils::GetBit(this->m_iFlags, iFlag);
	}
	private:
	uint8_t& m_iFlags;
};