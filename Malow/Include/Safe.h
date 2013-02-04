#pragma once

template<typename T>
inline void SAFE_DELETE( T& object )
{
	if ( object )
	{
		delete object;
		object = 0;
	}
};