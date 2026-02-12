#include "Component/CPP_CompWeaponSwap.h"
#include "Character/CPP_MainCharacter.h"
#include "Item/CPP_WeaponBase.h"
#include "Component/CPP_CompMontage.h"
#include "Components/ChildActorComponent.h"

UCPP_CompWeaponSwap::UCPP_CompWeaponSwap()
{
	PrimaryComponentTick.bCanEverTick = true;
	m_WeaponIndex = 0;
	m_MainCharacter = nullptr;
}

void UCPP_CompWeaponSwap::BeginPlay()
{
	Super::BeginPlay();

	m_MainCharacter = Cast<ACPP_MainCharacter>(GetOwner());
	m_CompMontage = GetOwner()->FindComponentByClass<UCPP_CompMontage>();

	if (!m_MainCharacter)
	{
		return;
	}

	if (m_Weapons.Num() == 0 || E_WeaponType.Num() == 0)
	{
		return;
	}

	if (m_Weapons.Num() != E_WeaponType.Num())
	{
		return;
	}

	if (!m_CompMontage)
	{
		UE_LOG(LogTemp, Error, TEXT("UCPP_CompMontage not found!"));
	}

	if (m_WeaponIndex < 0 || m_WeaponIndex >= m_Weapons.Num())
	{
		m_WeaponIndex = 0;
	}

	ChangeWeapon();
}

void UCPP_CompWeaponSwap::NextWeapon()
{
	if (m_MainCharacter && m_MainCharacter->is_Aiming)
		return;

	if (m_CompMontage && m_CompMontage->IsMontagePlaying())
		return;

	const int32 WeaponCount = m_Weapons.Num();
	if (WeaponCount <= 0) return;

	m_WeaponIndex = (m_WeaponIndex + 1) % WeaponCount;

	ChangeWeapon();
}

void UCPP_CompWeaponSwap::PrevWeapon()
{
	if (m_MainCharacter && m_MainCharacter->is_Aiming)
		return;

	if (m_CompMontage && m_CompMontage->IsMontagePlaying())
		return;

	const int32 WeaponCount = m_Weapons.Num();
	if (WeaponCount <= 0) return;

	m_WeaponIndex = (m_WeaponIndex + WeaponCount - 1) % WeaponCount;

	ChangeWeapon();
}

void UCPP_CompWeaponSwap::ChangeWeapon()
{
	const bool bOwnerOK = (m_MainCharacter != nullptr);
	const int32 WeaponCnt = m_Weapons.Num();
	const int32 TypeCnt = E_WeaponType.Num();
	const bool bCountsOK = (WeaponCnt > 0) && (TypeCnt > 0) && (WeaponCnt == TypeCnt);
	const bool bIndexOK = (m_WeaponIndex >= 0) && (m_WeaponIndex < WeaponCnt);

	if (!(bOwnerOK && bCountsOK && bIndexOK))
		return;

	if (m_WeaponIndex == 0)
	{
		Vibility();
		m_MainCharacter->OnWeaponChanged(E_WeaponType[m_WeaponIndex], nullptr);
		return;
	}

	TSubclassOf<ACPP_WeaponBase> DesiredClass = m_Weapons[m_WeaponIndex];
	if (!DesiredClass) // UClass 자체가 비어있으면 바로 종료 (IsA() 크래시 방지)
		return;

	// 모든 무기 숨기기
	Vibility();

	ACPP_WeaponBase* CurrentWeapon = nullptr;

	TArray<UChildActorComponent*> ChildActorComponents;
	m_MainCharacter->GetComponents<UChildActorComponent>(ChildActorComponents);

	for (int32 i = 0; i < ChildActorComponents.Num(); i = i + 1)
	{
		UChildActorComponent* ChildComp = ChildActorComponents[i];
		
		AActor* ChildActor = nullptr;
		if (ChildComp)
		{
			ChildActor = ChildComp->GetChildActor();
		}

		const bool bMatch = (ChildActor != nullptr) && ChildActor->IsA(DesiredClass);

		if (ChildComp)
		{
			ChildComp->SetVisibility(bMatch);
			ChildComp->SetComponentTickEnabled(bMatch);
		}

		if (bMatch)
		{
			CurrentWeapon = Cast<ACPP_WeaponBase>(ChildActor);
		}
	}

	m_MainCharacter->OnWeaponChanged(E_WeaponType[m_WeaponIndex], CurrentWeapon);
}

void UCPP_CompWeaponSwap::Vibility()
{
	if (!m_MainCharacter)
		return;

	TArray<UChildActorComponent*> ChildActorComponents;
	m_MainCharacter->GetComponents<UChildActorComponent>(ChildActorComponents);

	for (int32 i = 0; i < ChildActorComponents.Num(); i = i + 1)
	{
		UChildActorComponent* ChildComp = ChildActorComponents[i];
		
		AActor* ChildActor = nullptr;
		if (ChildComp != nullptr)
		{
			ChildActor = ChildComp->GetChildActor();
		}

		const bool bWeaponActor = (ChildActor != nullptr) && ChildActor->IsA(ACPP_WeaponBase::StaticClass());
		if (bWeaponActor)
		{
			ChildComp->SetVisibility(false);
			ChildComp->SetComponentTickEnabled(false);
		}
	}
}

void UCPP_CompWeaponSwap::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
