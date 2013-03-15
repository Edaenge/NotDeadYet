#pragma once

static const float V_WALK_SPEED		= 3.25f;
static const float V_RUN_SPEED		= 5.36f;
static const float V_CROUCH_SPEED	= 0.5f;

/*! Animation states*/
static const enum PlayerState
{
	STATE_IDLE,
	STATE_WALKING,
	STATE_RUNNING,
	STATE_CROUCHING,
	STATE_ATTACK,
	STATE_EQUIP_WEAPON,
	STATE_UNEQUIP_WEAPON,
	STATE_IDLE_WEAPON,
	STATE_BUSY,
	STATE_DEAD,
	STATE_NONE
};