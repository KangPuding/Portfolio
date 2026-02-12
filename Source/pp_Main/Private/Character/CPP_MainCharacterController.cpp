#include "Character/CPP_MainCharacterController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Character/CPP_CharacterBase.h"
#include "Character/CPP_MainCharacter.h"
#include "Component/CPP_CompWeaponSwap.h"
#include "Component/CPP_CompMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void ACPP_MainCharacterController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		StaminaTimerHandle,
		this,
		&ACPP_MainCharacterController::HandleStaminaTick,
		m_fStaminaTickInterval,
		true
	);

	GetWorldTimerManager().SetTimer(
		MPTimerHandle,
		this,
		&ACPP_MainCharacterController::HandleMPTick,
		m_fMPTickInterval,
		true
	);
}

void ACPP_MainCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(this->InputComponent);

	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPP_MainCharacterController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPP_MainCharacterController::Look);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACPP_MainCharacterController::JumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACPP_MainCharacterController::JumpStop);

		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ACPP_MainCharacterController::RunStart);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ACPP_MainCharacterController::RunStop);

		EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, this, &ACPP_MainCharacterController::OnNextWeapon);
		EnhancedInputComponent->BindAction(PrevWeaponAction, ETriggerEvent::Started, this, &ACPP_MainCharacterController::OnPrevWeapon);

		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ACPP_MainCharacterController::Dodge);

		EnhancedInputComponent->BindAction(Skill1Action, ETriggerEvent::Started, this, &ACPP_MainCharacterController::OnSkill_1);
		EnhancedInputComponent->BindAction(Skill2Action, ETriggerEvent::Started, this, &ACPP_MainCharacterController::OnSkill_2);
		EnhancedInputComponent->BindAction(Skill3Action, ETriggerEvent::Started, this, &ACPP_MainCharacterController::OnSkill_3);
	}

	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ACPP_MainCharacterController::OnLeftMousePressed);
}

void ACPP_MainCharacterController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	m_CurrentCharacter = Cast<ACPP_CharacterBase>(InPawn);

	UE_LOG(LogTemp, Log, TEXT("OnPossess: m_CurrentCharacter is %s"), m_CurrentCharacter ? TEXT("valid") : TEXT("nullptr"));

	if (UEnhancedInputLocalPlayerSubsystem* InputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		UE_LOG(LogTemp, Log, TEXT("DefaultMappingContext Added"));
		InputLocalPlayerSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InputLocalPlayerSubsystem is nullptr!!"));
	}
}

void ACPP_MainCharacterController::Move(const FInputActionValue& Value)
{
	if (!m_CurrentCharacter)
		return;

	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);

	if (MainChar->m_isLockInput)
		return;

	LastMoveInput = Value.Get<FVector2D>();

	const FVector2D Movement = Value.Get<FVector2D>();
	const FRotator Rotation = this->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	m_CurrentCharacter->AddMovementInput(ForwardDirection, Movement.Y);
	m_CurrentCharacter->AddMovementInput(RightDirection, Movement.X);
}

void ACPP_MainCharacterController::Look(const FInputActionValue& Value)
{
	if (!m_CurrentCharacter)
		return;

	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!m_isTarGetMode)
	{
		m_CurrentCharacter->AddControllerYawInput(LookAxisVector.X);
		m_CurrentCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACPP_MainCharacterController::Dodge(const FInputActionValue& Value)
{
	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!MainChar)
		return;

	if (m_isDodging)
		return;

	if (MainChar->m_CurrentSm < m_fDodgeStaminaCost)
		return;

	UCharacterMovementComponent* Move = MainChar->GetCharacterMovement();
	if (!Move)
		return;

	if (Move->IsFalling())
		return;

	const float ForwardValue = LastMoveInput.Y;
	const float RightValue = LastMoveInput.X;

	const auto Sign = [](float v) { return (v > 0.f) - (v < 0.f); };
	const int Horz = Sign(RightValue);
	const int Vert = Sign(ForwardValue);

	const bool UseForwardAxis = FMath::Abs(ForwardValue) >= FMath::Abs(RightValue);

	static const E_DodgeDirection Choose[2][3] = {
		{ E_DodgeDirection::E_LEFT,  E_DodgeDirection::E_NONE, E_DodgeDirection::E_RIGHT  },
		{ E_DodgeDirection::E_BACKWARD, E_DodgeDirection::E_NONE, E_DodgeDirection::E_FORWARD }
	};

	const int Idx = (UseForwardAxis ? Vert : Horz) + 1;
	const int AxisI = UseForwardAxis ? 1 : 0;
	const E_DodgeDirection Direction = Choose[AxisI][Idx];

	UCPP_CompMontage* MontageComp = MainChar->FindComponentByClass<UCPP_CompMontage>();
	if (MontageComp)
	{
		MontageComp->PlayDodgeMontage(MainChar->m_CurrentWeaponType, Direction);
		e_Direction = Direction;
	}

	MainChar->m_CurrentSm -= m_fDodgeStaminaCost;
	if (MainChar->m_CurrentSm < 0)
		MainChar->m_CurrentSm = 0;

	m_isDodging = true;

	const float DodgeFlagDuration = 0.6f;
	FTimerHandle TempHandle;
	GetWorldTimerManager().SetTimer(
		TempHandle,
		FTimerDelegate::CreateWeakLambda(this, [this]()
			{
				m_isDodging = false;
			}),
		DodgeFlagDuration,
		false
	);
}

void ACPP_MainCharacterController::JumpStart()
{
	if (!m_CurrentCharacter)
		return;

	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!MainChar)
		return;

	if (!MainChar->CanJump())
		return;

	if (UCPP_CompMontage* MontageComp = MainChar->FindComponentByClass<UCPP_CompMontage>())
	{
		if (MontageComp->IsMontagePlaying())
			return;
	}

	if (MainChar->m_CurrentSm < m_fJumpStaminaCost)
		return;

	m_isjumping = true;
	MainChar->Jump();

	MainChar->m_CurrentSm -= m_fJumpStaminaCost;
	if (MainChar->m_CurrentSm < 0)
		MainChar->m_CurrentSm = 0;
}

void ACPP_MainCharacterController::JumpStop()
{
	if (!m_CurrentCharacter)
		return;

	m_isjumping = false;
	m_CurrentCharacter->StopJumping();
}

void ACPP_MainCharacterController::RunStart()
{
	if (!m_CurrentCharacter)
		return;

	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!MainChar)
		return;

	if (MainChar->m_CurrentSm <= 0)
		return;

	if (m_isRunning)
		return;

	m_isRunning = true;
	m_CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2.0f;
}

void ACPP_MainCharacterController::RunStop()
{
	if (!m_CurrentCharacter)
		return;

	if (!m_isRunning)
		return;

	m_isRunning = false;
	m_CurrentCharacter->GetCharacterMovement()->MaxWalkSpeed /= 2.0f;
}

void ACPP_MainCharacterController::OnNextWeapon()
{
	if (m_CurrentCharacter == nullptr)
		return;

	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (MainChar && MainChar->is_Aiming)
		return;

	UCPP_CompWeaponSwap* WeaponComp = m_CurrentCharacter->FindComponentByClass<UCPP_CompWeaponSwap>();
	if (!WeaponComp)
		return;

	WeaponComp->NextWeapon();
}

void ACPP_MainCharacterController::OnPrevWeapon()
{
	if (m_CurrentCharacter == nullptr)
		return;

	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (MainChar && MainChar->is_Aiming)
		return;

	UCPP_CompWeaponSwap* WeaponComp = m_CurrentCharacter->FindComponentByClass<UCPP_CompWeaponSwap>();
	if (!WeaponComp)
		return;

	WeaponComp->PrevWeapon();
}

void ACPP_MainCharacterController::OnLeftMousePressed()
{
	if (!m_CurrentCharacter)
		return;

	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!MainChar)
		return;

	if (MainChar->m_CurrentWeaponType == E_WeaponTypes::E_NONE || MainChar->m_CurrentWeapon == nullptr)
		return;

	UCPP_CompMontage* MontageComp = m_CurrentCharacter->FindComponentByClass<UCPP_CompMontage>();
	if (!MontageComp || MainChar->is_Aiming)
		return;

	if (UCharacterMovementComponent* Move = MainChar->GetCharacterMovement())
	{
		if (Move->IsFalling() || m_isjumping)
			return;
	}
	else
	{
		if (m_isjumping)
			return;
	}

	if (MainChar->m_CurrentSm < m_fAttackStaminaCost)
		return;

	const bool bAttackAccepted = MontageComp->PlayCurrentWeaponMontage();

	if (!bAttackAccepted)
		return;

	MainChar->m_CurrentSm -= m_fAttackStaminaCost;
	if (MainChar->m_CurrentSm < 0)
		MainChar->m_CurrentSm = 0;
}

void ACPP_MainCharacterController::OnSkill_1()
{
	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!m_CurrentCharacter || !MainChar)
		return;

	if (MainChar->m_CurrentWeaponType == E_WeaponTypes::E_NONE || MainChar->m_CurrentWeapon == nullptr)
		return;

	UCPP_CompMontage* comp = m_CurrentCharacter->FindComponentByClass<UCPP_CompMontage>();
	if (!comp)
		return;

	if (UCharacterMovementComponent* Move = MainChar->GetCharacterMovement())
	{
		if (Move->IsFalling() || m_isjumping)
			return;
	}
	else
	{
		if (m_isjumping)
			return;
	}

	if (comp->IsSkillMontagePlaying())
		return;

	if (m_isSkill1OnCooldown)
		return;

	if (MainChar->m_CurrentMp < m_fSkill1MPCost)
		return;

	comp->PlaySkill1();

	MainChar->m_CurrentMp -= m_fSkill1MPCost;
	if (MainChar->m_CurrentMp < 0)
		MainChar->m_CurrentMp = 0;

	m_isSkill1OnCooldown = true;
	GetWorldTimerManager().ClearTimer(Skill1CooldownHandle);
	GetWorldTimerManager().SetTimer(
		Skill1CooldownHandle,
		this,
		&ACPP_MainCharacterController::ResetSkill1Cooldown,
		m_fSkill1CooldownTime,
		false
	);
}


void ACPP_MainCharacterController::OnSkill_2()
{
	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!m_CurrentCharacter || !MainChar)
		return;

	if (MainChar->m_CurrentWeaponType == E_WeaponTypes::E_NONE || MainChar->m_CurrentWeapon == nullptr)
		return;

	UCPP_CompMontage* comp = m_CurrentCharacter->FindComponentByClass<UCPP_CompMontage>();
	if (!comp)
		return;

	if (UCharacterMovementComponent* Move = MainChar->GetCharacterMovement())
	{
		if (Move->IsFalling() || m_isjumping)
			return;
	}
	else
	{
		if (m_isjumping)
			return;
	}

	if (comp->IsSkillMontagePlaying())
		return;

	if (m_isSkill2OnCooldown)
		return;

	if (MainChar->m_CurrentMp < m_fSkill2MPCost)
		return;

	comp->PlaySkill2();

	MainChar->m_CurrentMp -= m_fSkill2MPCost;
	if (MainChar->m_CurrentMp < 0)
		MainChar->m_CurrentMp = 0;

	m_isSkill2OnCooldown = true;
	GetWorldTimerManager().ClearTimer(Skill2CooldownHandle);
	GetWorldTimerManager().SetTimer(
		Skill2CooldownHandle,
		this,
		&ACPP_MainCharacterController::ResetSkill2Cooldown,
		m_fSkill2CooldownTime,
		false
	);
}


void ACPP_MainCharacterController::OnSkill_3()
{
	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!m_CurrentCharacter || !MainChar)
		return;

	if (MainChar->m_CurrentWeaponType == E_WeaponTypes::E_NONE || MainChar->m_CurrentWeapon == nullptr)
		return;

	UCPP_CompMontage* comp = m_CurrentCharacter->FindComponentByClass<UCPP_CompMontage>();
	if (!comp)
		return;

	if (UCharacterMovementComponent* Move = MainChar->GetCharacterMovement())
	{
		if (Move->IsFalling() || m_isjumping)
			return;
	}
	else
	{
		if (m_isjumping)
			return;
	}

	if (comp->IsSkillMontagePlaying())
		return;

	if (m_isSkill3OnCooldown)
		return;

	if (MainChar->m_CurrentMp < m_fSkill3MPCost)
		return;

	comp->PlaySkill3();

	MainChar->m_CurrentMp -= m_fSkill3MPCost;
	if (MainChar->m_CurrentMp < 0)
		MainChar->m_CurrentMp = 0;

	m_isSkill3OnCooldown = true;
	GetWorldTimerManager().ClearTimer(Skill3CooldownHandle);
	GetWorldTimerManager().SetTimer(
		Skill3CooldownHandle,
		this,
		&ACPP_MainCharacterController::ResetSkill3Cooldown,
		m_fSkill3CooldownTime,
		false
	);
}



void ACPP_MainCharacterController::HandleStaminaTick()
{
	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!MainChar)
		return;

	if (MainChar->m_CurrentSm < 0)
		MainChar->m_CurrentSm = 0;

	if (m_isRunning)
	{
		MainChar->m_CurrentSm -= m_fStaminaRunDrainPerTick;
		if (MainChar->m_CurrentSm < 0)
			MainChar->m_CurrentSm = 0;

		if (MainChar->m_CurrentSm <= 0)
		{
			RunStop();
		}

		return;
	}

	const bool bCanRegen = !m_isRunning && !m_isjumping && !m_isDodging;

	if (bCanRegen)
	{
		MainChar->m_CurrentSm += m_fStaminaRegenPerTick;

		if (MainChar->m_CurrentSm > MainChar->m_MaxSm)
			MainChar->m_CurrentSm = MainChar->m_MaxSm;
	}
}

void ACPP_MainCharacterController::HandleMPTick()
{
	ACPP_MainCharacter* MainChar = Cast<ACPP_MainCharacter>(m_CurrentCharacter);
	if (!MainChar)
		return;

	if (MainChar->m_CurrentMp < 0)
		MainChar->m_CurrentMp = 0;

	MainChar->m_CurrentMp += m_fMPRegenPerTick;

	if (MainChar->m_CurrentMp > MainChar->m_MaxMp)
		MainChar->m_CurrentMp = MainChar->m_MaxMp;
}

void ACPP_MainCharacterController::ResetSkill1Cooldown()
{
	m_isSkill1OnCooldown = false;
}

void ACPP_MainCharacterController::ResetSkill2Cooldown()
{
	m_isSkill2OnCooldown = false;
}

void ACPP_MainCharacterController::ResetSkill3Cooldown()
{
	m_isSkill3OnCooldown = false;
}

