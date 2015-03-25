//--------------------------------------------------------------------------------------
// File: InputStream.h
// Author: Andrew Coulthard
//
// This class essentially implements a FILO stack system for characters.
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------

#define STR_TERMINAL '\n'

typedef struct InputStream
{
	char stream[100]; 
	int index;
} InputStream;

void IS_Insert(char in, InputStream* iStream)
{
	iStream->stream[iStream->index] = in;
	iStream->index++;
}

/* Can be uncommented if we need these for something.
char IS_Peek(InputStream* iStream)
{
	if(iStream->index = 0)
		return iStream->stream[iStream->index];
	return iStream->stream[iStream->index - 1];
}

char IS_Remove(InputStream* iStream)
{
	if(iStream->index = 0)
	{
		return iStream->stream[iStream->index];
	}
	else
	{
		char out = iStream->stream[iStream->index - 1];
		iStream->stream[iStream->index - 1] = 0;
		iStream->index -= 1;
		return out;
	}
}
*/

void IS_Init(InputStream* iStream)
{
	for(int i = 0; i < 100; i++)
	{
		iStream->stream[i] = 0;
	}
	iStream->index = 0;
}

void IS_Clear(InputStream* iStream)
{
	IS_Init(iStream);
}

int IS_ContainsString(char* search, InputStream* iStream)
{
	int i = 0;
	while( 1 )
	{
		if( search[i] == STR_TERMINAL && iStream->stream[i] == STR_TERMINAL )
			return 1;
		if( search[i] != iStream->stream[i] )
			break;
		i++;
	}
	return 0;
}