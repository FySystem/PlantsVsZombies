#pragma once

namespace GameOffset
{
	//ģ�����ַƫ��
	constexpr uintptr_t ModuleBase = 0x2a9ec0;

	//����ַƫ��
	constexpr uintptr_t GameBase = 0x768;

	//��ʬ����ַ
	constexpr uintptr_t ZombieBase = 0x90;

	namespace Offset
	{
		//����ƫ��
		constexpr uintptr_t Sum = 0x5560;

		//����ƫ��
		constexpr uintptr_t ZombiePositionX = 0xB0C;

		//Ѫ��ƫ��
		constexpr uintptr_t ZomBieHealth = 0xBA8;

		//�ܽ�ʬ����
		constexpr uintptr_t NumOfZombie = 0xA0;

		//��ʬ���ƫ�� 
		constexpr uintptr_t zombieOffset = 0x15c;
	}
}

class Zombie
{
public:
	Zombie(const Vector::vec2& n_location, const Vector::Health<int>& n_health)
		:location(n_location), health(n_health)
	{}

	//����
	Vector::vec2 location;

	//Ѫ��
	Vector::Health<int> health;
};

class Infomation : public Singleton<Infomation>
{
public:
	void Execute();

	//��ʬ�б�
	std::vector<std::shared_ptr<Zombie>> ZombieList;
private:
	void upData();
};

