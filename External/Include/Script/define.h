#pragma once

enum class PLAYER_STATE
{
	IDLE,
	WALK,
	RUN,
	DAMEGED,
	ATTACK,
	PARRY,
	DEATH,
	END,
};

enum class MONSTER_STATE
{
	IDLE,
	STAGGER,
	ATTACK,
	DEATH,
	WALK,
	END,
};