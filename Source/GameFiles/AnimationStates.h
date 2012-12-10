#pragma once

static const float V_WALK_SPEED		= 10.0f;
static const float V_RUN_SPEED		= 15.0f;
static const float V_CROUCH_SPEED	= 5.0f;

/*! Animation states*/
enum PlayerState
{
	STATE_IDLE,
	STATE_WALKING,
	STATE_RUNNING,
	STATE_CROUCHING
};