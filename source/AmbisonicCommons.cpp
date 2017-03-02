#include "AmbisonicCommons.h"

AmbFloat DegreesToRadians(AmbFloat fDegrees)
{
	return fDegrees * (AmbFloat)M_PI / 180.f;
}

AmbFloat RadiansToDegrees(AmbFloat fRadians)
{
	return fRadians * 180.f / (AmbFloat)M_PI;
}

AmbUInt OrderToComponents(AmbUInt nOrder, AmbBool b3D)
{
	if(b3D)
		return (AmbUInt) powf(nOrder + 1.f, 2.f);
	else
		return nOrder * 2 + 1;
}

AmbUInt OrderToComponentPosition(AmbUInt nOrder, AmbBool b3D)
{
	AmbUInt nIndex = 0;

	if(b3D)
	{
		switch(nOrder)
		{
		case 0:	nIndex = 0;	break;
		case 1:	nIndex = 1;	break;
		case 2:	nIndex = 4;	break;
		case 3:	nIndex = 10;break;
		}
	}
	else
	{
		switch(nOrder)
		{
		case 0:	nIndex = 0;	break;
		case 1:	nIndex = 1;	break;
		case 2:	nIndex = 3;	break;
		case 3:	nIndex = 5;	break;
		}
	}

	return nIndex;
}

AmbUInt OrderToSpeakers(AmbUInt nOrder, AmbBool b3D)
{
	if(b3D)
		return (nOrder * 2 + 2) * 2;
	else
		return nOrder * 2 + 2;
}

char ComponentToChannelLabel(AmbUInt nComponent, AmbBool b3D)
{
    char cLabel = ' ';
    if(b3D)
    {
        switch(nComponent)
        {
        case 0:     cLabel = 'W';   break;
        case 1:     cLabel = 'X';   break;
        case 2:     cLabel = 'Y';   break;
        case 3:     cLabel = 'Z';   break;
        case 4:     cLabel = 'R';   break;
        case 5:     cLabel = 'S';   break;
        case 6:     cLabel = 'T';   break;
        case 7:     cLabel = 'U';   break;
        case 8:     cLabel = 'V';   break;
        case 9:     cLabel = 'K';   break;
        case 10:    cLabel = 'L';   break;
        case 11:    cLabel = 'M';   break;
        case 12:    cLabel = 'N';   break;
        case 13:    cLabel = 'O';   break;
        case 14:    cLabel = 'P';   break;
        case 15:    cLabel = 'Q';   break;
        };
    }
    else
    {
        switch(nComponent)
        {
        case 0:     cLabel = 'W';   break;
        case 1:     cLabel = 'X';   break;
        case 2:     cLabel = 'Y';   break;
        case 3:     cLabel = 'U';   break;
        case 4:     cLabel = 'V';   break;
        case 5:     cLabel = 'P';   break;
        case 6:     cLabel = 'Q';   break;
        };
    }

    return cLabel;
}