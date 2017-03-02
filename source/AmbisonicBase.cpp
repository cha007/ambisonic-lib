#include "AmbisonicBase.h"

CAmbisonicBase::CAmbisonicBase(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc)
{
	m_nOrder = 0;
	m_b3D = 0;
	m_nChannelCount = 0;
}

CAmbisonicBase::~CAmbisonicBase()
{
	
}

AmbUInt CAmbisonicBase::GetOrder()
{
	return m_nOrder;
}

AmbUInt CAmbisonicBase::GetHeight()
{
	return m_b3D;
}

AmbUInt CAmbisonicBase::GetChannelCount()
{
	return m_nChannelCount;
}

AmbBool CAmbisonicBase::Create(AmbUInt nOrder, AmbBool b3D, AmbUInt nMisc)
{
	m_nOrder = nOrder;
	m_b3D = b3D;
	m_nChannelCount = OrderToComponents(m_nOrder, m_b3D);
    
    return true;
}