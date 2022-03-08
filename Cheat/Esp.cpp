#include "pch.h"
#include "framework.h"

void Esp::Execute()
{
	for (const auto& it : Infomation::get()->ZombieList)
	{
		if (it->location == 0)continue;
		if (it->health.hp <= 0)continue;
		Renderer::get()->drawEsp(Vector::vec4(
			it->location.x,
			it->location.y,
			100, 150
		));
	}
}