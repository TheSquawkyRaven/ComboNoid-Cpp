#include "Combo.h"
#include "../Game.h"
#include "Gameplay.h"

Combo::Combo(Game* game, Gameplay* gameplay) : game(game), gameplay(gameplay)
{
}

void Combo::Init()
{

}

void Combo::OnDestroy()
{
	
}

void Combo::UpdateCombo()
{
	if (combo != lastCombo)
	{
		lastCombo = combo;
		if (combo == 0)
		{
			printf("Combo Ended\n");
			return;
		}
		printf("Combo: %d\n", combo);
	}
}

void Combo::PaddleHit()
{
	combo++;
	UpdateCombo();
}

void Combo::PaddleMiss()
{
	combo--;
	if (combo < 0)
	{
		combo = 0;
	}
	UpdateCombo();
}

void Combo::PaddleMissBall()
{
	combo--;
	if (combo < 0)
	{
		combo = 0;
	}
	UpdateCombo();
}

void Combo::BallLost()
{
	combo = 0;
	UpdateCombo();
}
