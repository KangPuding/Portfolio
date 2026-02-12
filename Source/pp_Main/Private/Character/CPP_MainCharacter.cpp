#include "Character/CPP_MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Character/CPP_GameStateBase.h"
#include "Item/CPP_Slot.h"
#include "Item/CPP_Item_Base.h"

ACPP_MainCharacter::ACPP_MainCharacter()
{
	m_CurrentWeaponType = E_WeaponTypes::E_NONE;
	m_CurrentWeapon = nullptr;

	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;

	UCharacterMovementComponent* MovementCompnonet = this->GetCharacterMovement();
	MovementCompnonet->bOrientRotationToMovement = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	m_BaseMaxHp = 1500.f;
	m_BaseMaxMp = 1000.f;
	m_BaseMaxSm = 1000.f;
	m_BaseDefense = 0.f;

	m_EquipBonusHp = 0.f;
	m_EquipBonusMp = 0.f;
	m_EquipBonusSm = 0.f;
	m_EquipBonusDef = 0.f;

	m_MaxHp = m_BaseMaxHp;
	m_MaxMp = m_BaseMaxMp;
	m_MaxSm = m_BaseMaxSm;
	m_Defense = m_BaseDefense;

	m_CurrentHp = m_MaxHp;
	m_CurrentMp = m_MaxMp;
	m_CurrentSm = m_MaxSm;

}

void ACPP_MainCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindEquipSlots();
	RecalcEquipStats();
}

void ACPP_MainCharacter::BindEquipSlots()
{
	ACPP_GameStateBase* GS = Cast<ACPP_GameStateBase>(UGameplayStatics::GetGameState(this));
	if (!GS)
		return;

	const int32 EquipSlotCount = GS->EquipSlotCount; 
	for (int32 i = 0; i < EquipSlotCount; ++i)
	{
		UCPP_Slot* Slot = GS->getEquipInventorySlot(i);
		if (!Slot)
			continue;

		Slot->OnSlotUpdated.RemoveDynamic(this, &ACPP_MainCharacter::OnEquipSlotUpdated);
		Slot->OnSlotUpdated.AddDynamic(this, &ACPP_MainCharacter::OnEquipSlotUpdated);
	}

	UE_LOG(LogTemp, Warning, TEXT("[BindEquipSlots] EquipSlotCount=%d"), GS->EquipSlotCount);
	for (int32 i = 0; i < GS->EquipSlotCount; ++i)
	{
		UCPP_Slot* Slot = GS->getEquipInventorySlot(i);
		UE_LOG(LogTemp, Warning, TEXT("[BindEquipSlots] i=%d Slot=%s"), i, *GetNameSafe(Slot));
	}
}

void ACPP_MainCharacter::OnEquipSlotUpdated()
{
	UE_LOG(LogTemp, Warning, TEXT("[OnEquipSlotUpdated] called"));
	RecalcEquipStats();
}

void ACPP_MainCharacter::RecalcEquipStats()
{
	ACPP_GameStateBase* GS = Cast<ACPP_GameStateBase>(UGameplayStatics::GetGameState(this));
	if (!GS)
		return;

	m_EquipBonusHp = 0.f;
	m_EquipBonusMp = 0.f;
	m_EquipBonusSm = 0.f;
	m_EquipBonusDef = 0.f;

	const int32 EquipSlotCount = GS->EquipSlotCount;
	for (int32 i = 0; i < EquipSlotCount; ++i)
	{
		UCPP_Slot* Slot = GS->getEquipInventorySlot(i);
		if (!Slot || !Slot->m_ItemClass || Slot->m_ItemCount <= 0)
			continue;

		const ACPP_Item_Base* CDO = Slot->m_ItemClass->GetDefaultObject<ACPP_Item_Base>();
		if (!CDO)
			continue;

		m_EquipBonusHp += CDO->GetBonusMaxHp();
		m_EquipBonusMp += CDO->GetBonusMaxMp();
		m_EquipBonusSm += CDO->GetBonusMaxSm();
		m_EquipBonusDef += CDO->GetBonusDefense();
	}

	m_MaxHp = m_BaseMaxHp + m_EquipBonusHp;
	m_MaxMp = m_BaseMaxMp + m_EquipBonusMp;
	m_MaxSm = m_BaseMaxSm + m_EquipBonusSm;
	m_Defense = m_BaseDefense + m_EquipBonusDef;

	m_CurrentHp = FMath::Clamp(m_CurrentHp, 0.f, m_MaxHp);
	m_CurrentMp = FMath::Clamp(m_CurrentMp, 0.f, m_MaxMp);
	m_CurrentSm = FMath::Clamp(m_CurrentSm, 0.f, m_MaxSm);

	UE_LOG(LogTemp, Warning, TEXT("[RecalcEquipStats] MaxHp=%.1f (Base=%.1f + Bonus=%.1f) | MaxMp=%.1f | MaxSm=%.1f | Def=%.1f"),
		m_MaxHp, m_BaseMaxHp, m_EquipBonusHp, m_MaxMp, m_MaxSm, m_Defense);
}

int32 ACPP_MainCharacter::ItemSelect()
{
	const int32 nCount = m_ItemArray.Num();
	if (nCount <= 0)
		return INDEX_NONE;

	const int32 nIndex = m_ItemArray.Find(m_SelectItem);
	const int32 nLast = nCount - 1;

	if (nIndex < 0)
		return nLast;

	const int32 nPrev = (nIndex + nCount - 1) % nCount;
	return nPrev;
}


void ACPP_MainCharacter::OnWeaponChanged(E_WeaponTypes NewWeapon, ACPP_WeaponBase* CurrentWeapon)
{
	m_CurrentWeaponType = NewWeapon;
	m_CurrentWeapon = CurrentWeapon;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	if (CurrentWeapon == nullptr)
	{
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->bOrientRotationToMovement = true;
	}
	else
	{
		MoveComp->bUseControllerDesiredRotation = true;
		MoveComp->bOrientRotationToMovement = false;
	}
}
