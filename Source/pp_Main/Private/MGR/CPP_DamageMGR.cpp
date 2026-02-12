#include "MGR/CPP_DamageMGR.h"

UCPP_DamageMGR::UCPP_DamageMGR()
{
	PrimaryComponentTick.bCanEverTick = true;
	m_StatisticsScale = 1.f;
	m_EquipScale = 1.f;
	m_BuffScale = 1.f;
}


void UCPP_DamageMGR::BeginPlay()
{
	Super::BeginPlay();
}


void UCPP_DamageMGR::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCPP_DamageMGR::CalculateDamage(float Damage)
{
	m_Damage = Damage;
}

float UCPP_DamageMGR::GetFinalDamage() const
{
	const float Stat  = FMath::Max(0.f, m_StatisticsScale);
	const float Equip = FMath::Max(0.f, m_EquipScale);
	const float Buff  = FMath::Max(0.f, m_BuffScale);
	return m_Damage * Stat * Equip * Buff;
}

void UCPP_DamageMGR::SetScales(float InStat, float InEquip, float InBuff)
{
	m_StatisticsScale = InStat;
	m_EquipScale = InEquip;
	m_BuffScale = InBuff;
}
