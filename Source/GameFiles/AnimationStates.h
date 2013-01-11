#pragma once

static const float V_WALK_SPEED		= 6.0f;
static const float V_RUN_SPEED		= 10.0f;
static const float V_CROUCH_SPEED	= 2.0f;

/*! Animation states*/
static const enum PlayerState
{
	STATE_IDLE,
	STATE_WALKING,
	STATE_RUNNING,
	STATE_CROUCHING
};