#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Enum/E_WeaponType.h"
#include "Enum/E_DodgeDirection.h"
#include "CPP_MainCharacterController.generated.h"

class ACPP_CharacterBase;
class ACPP_MainCharacter;
class UInputMappingContext;
class UInputAction;
class UCPP_CompWeaponSwap;

struct FInputActionValue;

UCLASS()
class ACPP_MainCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// IA_Input
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RunAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* NextWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrevWeaponAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftMouseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DodgeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill1Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill2Action;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Skill3Action;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	ACPP_CharacterBase* m_CurrentCharacter;

	FVector2D LastMoveInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bool")
	bool m_isTarGetMode;

	UPROPERTY()
	bool m_isjumping;

public:
	E_DodgeDirection e_Direction;

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dodge(const FInputActionValue& Value);

	void OnNextWeapon();
	void OnPrevWeapon();

	void JumpStart();
	void JumpStop();

	void RunStart();
	void RunStop();

	void OnLeftMousePressed();

	void OnSkill_1();
	void OnSkill_2();
	void OnSkill_3();

	void HandleStaminaTick();  // 1 second
	void HandleMPTick();       // 10 second

protected:

	FTimerHandle StaminaTimerHandle;
	FTimerHandle MPTimerHandle;

	FTimerHandle Skill1CooldownHandle;
	FTimerHandle Skill2CooldownHandle;
	FTimerHandle Skill3CooldownHandle;


	// Time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float m_fStaminaTickInterval = 1.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float m_fMPTickInterval = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float m_fSkill1CooldownTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float m_fSkill2CooldownTime = 7.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float m_fSkill3CooldownTime = 10.0f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float m_fStaminaRunDrainPerTick = 2.0f; // SM (Run) : -2 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float m_fStaminaRegenPerTick = 50.0f;   // SM += 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float m_fMPRegenPerTick = 5.0f;	// MP += 5;

	// Cost
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float m_fAttackStaminaCost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float m_fJumpStaminaCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float m_fDodgeStaminaCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float m_fSkill1MPCost = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float m_fSkill2MPCost = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mana")
	float m_fSkill3MPCost = 20.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	bool m_isRunning = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
	bool m_isDodging = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	bool m_isSkillPlaying = false;

	bool m_isSkill1OnCooldown = false;
	bool m_isSkill2OnCooldown = false;
	bool m_isSkill3OnCooldown = false;

	void ResetSkill1Cooldown();
	void ResetSkill2Cooldown();
	void ResetSkill3Cooldown();
};
