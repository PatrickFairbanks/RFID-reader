//--------------------------------------------------------------------------------------
// File: MLX.h
// Authors: Andrew Coulthard
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------

#include <IOConfig.h>
#include <XPD.h>

#include "time_funcs.h"

typedef struct MLX_Pin_Set 
{
	GlobalPin const DIN;
	GlobalPin const DOUT;
	GlobalPin const DSYNC;
	GlobalPin const CK;
	GlobalPin const MODE;
	GlobalPin const RTB;	
	
	static const int periodTicks;
} MLX_Pin_Set;