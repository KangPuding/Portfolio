#include "Component/CPP_CompMontage.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Animation/AnimInstance.h"

UCPP_CompMontage::UCPP_CompMontage()
{
	PrimaryComponentTick.bCanEverTick = true;
	m_isPlayMontage = false;
	m_InputMontage = nullptr;
	m_PlayMontage = nullptr;
	m_InputMetaMontage = nullptr;
	m_PlayMetaMontage = nullptr;
	m_MainCharacter = nullptr;
	m_Mesh = nullptr;
	m_isRuntimeCancelWindow = false;
}

void UCPP_CompMontage::BeginPlay()
{
	Super::BeginPlay();
	m_MainCharacter = Cast<ACPP_MainCharacter>(GetOwner());
	if (!m_MainCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("MontageComp: Owner is not ACPP_MainCharacter!"));
		return;
	}
	m_Mesh = m_MainCharacter->GetMesh();
	if (!m_Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("MontageComp: Mesh not found!"));
	}
}

int32 UCPP_CompMontage::GetActionPriority(E_ACTIONTYPE actionType)
{
	if (actionType == E_ACTIONTYPE::Evasion)
		return 1;
	if (actionType == E_ACTIONTYPE::Combo)
		return 2;
	if (actionType == E_ACTIONTYPE::Skill)
		return 3;
	return 0;
}

void UCPP_CompMontage::OpenCancelWindow()
{
	m_isRuntimeCancelWindow = true;
	UE_LOG(LogTemp, Warning, TEXT("CancelWindow OPEN"));
}

void UCPP_CompMontage::CloseCancelWindow()
{
	m_isRuntimeCancelWindow = false;
	UE_LOG(LogTemp, Warning, TEXT("CancelWindow CLOSE"));
}

bool UCPP_CompMontage::enablePlay()
{
	if (!m_isPlayMontage)
		return true;

	if (!IsValid(m_InputMetaMontage))
		return false;

	if (!IsValid(m_PlayMetaMontage))
		return true;

	const int32 PlayPriority = GetActionPriority(m_PlayMetaMontage->e_ActionType);
	const int32 InputPriority = GetActionPriority(m_InputMetaMontage->e_ActionType);

	if (InputPriority < PlayPriority)
		return true;

	if (InputPriority == PlayPriority)
	{
		const bool bBothCombo =
			(m_PlayMetaMontage->e_ActionType == E_ACTIONTYPE::Combo) &&
			(m_InputMetaMontage->e_ActionType == E_ACTIONTYPE::Combo);

		if (bBothCombo && m_PlayMetaMontage->m_isCancelCombo && m_isRuntimeCancelWindow)
		{
			if (IsValid(m_PlayMetaMontage->m_ComboMontage))
			{
				e_setInputMontage(m_PlayMetaMontage->m_ComboMontage);
			}
			return true;
		}
		return false;
	}

	return false;
}

void UCPP_CompMontage::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	playMontage();
}

void UCPP_CompMontage::eraseMontage()
{
	m_InputMontage = nullptr;
	m_InputMetaMontage = nullptr;
}

void UCPP_CompMontage::playMontage()
{
	if (!IsValid(m_InputMontage))
		return;

	if (!enablePlay())
		return;

	if (!m_Mesh)
		return;

	UAnimInstance* AnimInstance = m_Mesh->GetAnimInstance();
	if (!AnimInstance)
		return;

	if (m_isPlayMontage && IsValid(m_PlayMontage) && IsValid(m_PlayMetaMontage))
	{
		if (m_PlayMetaMontage->m_isCancelCombo)
		{
			AnimInstance->Montage_Stop(0.0f, m_PlayMontage);
			UE_LOG(LogTemp, Warning, TEXT("playMontage: force stop %s (CancelCombo TRUE)"), *m_PlayMontage->GetName());
		}
	}

	UAnimMontage* MontageToPlay = m_InputMontage;
	UCPP_MetaActionData* MetaToPlay = m_InputMetaMontage;

	const float PlayLength = AnimInstance->Montage_Play(MontageToPlay, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("playMontage: %s len=%.3f"), *MontageToPlay->GetName(), PlayLength);

	if (PlayLength <= 0.f)
		return;

	m_PlayMontage = MontageToPlay;
	m_PlayMetaMontage = MetaToPlay;
	m_InputMontage = nullptr;
	m_InputMetaMontage = nullptr;
	m_isPlayMontage = true;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(m_InputHandleMontage);
	}

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UCPP_CompMontage::OnMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, m_PlayMontage);
}

void UCPP_CompMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage != m_PlayMontage)
		return;

	m_PlayMontage = nullptr;
	m_PlayMetaMontage = nullptr;
	m_isPlayMontage = false;
	m_isRuntimeCancelWindow = false;
}

void UCPP_CompMontage::e_setInputMontage(UAnimMontage* Montage)
{
	m_InputMontage = Montage;
	if (!m_InputMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("e_setInputMontage: Montage is null"));
		return;
	}

	m_InputMetaMontage = m_InputMontage->FindMetaDataByClass<UCPP_MetaActionData>();

	if (m_InputMetaMontage)
	{
		UE_LOG(LogTemp, Log, TEXT("MetaActionData found: %s"), *m_InputMetaMontage->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No MetaActionData found in montage: %s"), *m_InputMontage->GetName());
	}
}

void UCPP_CompMontage::e_PlayMontage(UAnimMontage* Montage)
{
	e_setInputMontage(Montage);

	playMontage();

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(m_InputHandleMontage, this, &UCPP_CompMontage::eraseMontage, 0.5f, false);
	}
}

bool UCPP_CompMontage::PlayCurrentWeaponMontage()
{
	if (!m_MainCharacter)
		return false;

	const int32 Index = static_cast<int32>(m_MainCharacter->m_CurrentWeaponType);
	if (Index <= 0)
		return false;

	if (Index >= AttackMontages.Num())
		return false;

	UAnimMontage* MontageToPlay = AttackMontages[Index];
	if (!MontageToPlay)
		return false;

	const bool bWasPlaying = m_isPlayMontage;

	e_PlayMontage(MontageToPlay);

	if (!bWasPlaying && m_isPlayMontage && m_PlayMontage == MontageToPlay)
	{
		return true;  
	}

	return false;
}

void UCPP_CompMontage::PlayDodgeMontage(E_WeaponTypes WeaponType, E_DodgeDirection Direction)
{
	if (!m_MainCharacter)
		return;

	USkeletalMeshComponent* MeshComp = m_MainCharacter->GetMesh();
	if (!MeshComp)
		return;

	FS_DodgeMontageSet* MontageSet = DodgeMontages.Find(WeaponType);
	if (!MontageSet)
		return;

	UAnimMontage* MontageToPlay = nullptr;

	if (Direction == E_DodgeDirection::E_FORWARD)
		MontageToPlay = MontageSet->ForwardMontage;
	else if (Direction == E_DodgeDirection::E_BACKWARD)
		MontageToPlay = MontageSet->BackwardMontage;
	else if (Direction == E_DodgeDirection::E_LEFT)
		MontageToPlay = MontageSet->LeftMontage;
	else if (Direction == E_DodgeDirection::E_RIGHT)
		MontageToPlay = MontageSet->RightMontage;

	if (!MontageToPlay)
		return;

	e_PlayMontage(MontageToPlay);
}

void UCPP_CompMontage::PlaySkillByIndex(int32 SkillIndex)
{
	if (!m_MainCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PlaySkillByIndex: MainCharacter is null"));
		return;
	}

	const E_WeaponTypes CurrentWeapon = m_MainCharacter->m_CurrentWeaponType;
	UE_LOG(LogTemp, Warning, TEXT("PlaySkillByIndex: CurrentWeaponType = %d"), static_cast<int32>(CurrentWeapon));

	FS_SKILLSET* SkillSet = SkillMontage.Find(CurrentWeapon);
	if (!SkillSet)
	{
		UE_LOG(LogTemp, Error, TEXT("PlaySkillByIndex: SkillSet not found for weapon %d"), static_cast<int32>(CurrentWeapon));
		return;
	}

	UAnimMontage* MontageToPlay = nullptr;

	if (SkillIndex == 1)
		MontageToPlay = SkillSet->FirstSkill;
	else if (SkillIndex == 2)
		MontageToPlay = SkillSet->SecondSkill;
	else if (SkillIndex == 3)
		MontageToPlay = SkillSet->ThirdSkill;
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlaySkillByIndex: Invalid skill index %d"), SkillIndex);
		return;
	}

	if (!MontageToPlay)
	{
		UE_LOG(LogTemp, Error, TEXT("PlaySkillByIndex: Montage is null for skill %d"), SkillIndex);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PlaySkillByIndex: Playing montage for skill %d"), SkillIndex);
	e_PlayMontage(MontageToPlay);
}

void UCPP_CompMontage::PlaySkill1()
{
	PlaySkillByIndex(1);
}

void UCPP_CompMontage::PlaySkill2()
{
	PlaySkillByIndex(2);
}

void UCPP_CompMontage::PlaySkill3()
{
	PlaySkillByIndex(3);
}
