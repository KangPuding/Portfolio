#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enum/E_WeaponType.h"
#include "Enum/E_DodgeDirection.h"
#include "MetaData/CPP_MetaActionData.h"
#include "Character/CPP_MainCharacter.h"
#include "CPP_CompMontage.generated.h"

class UCPP_MetaActionData;
class USkeletalMeshComponent;

USTRUCT(BlueprintType)
struct FS_DodgeMontageSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ForwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* BackwardMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* LeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* RightMontage;

	FS_DodgeMontageSet()
		: ForwardMontage(nullptr)
		, BackwardMontage(nullptr)
		, LeftMontage(nullptr)
		, RightMontage(nullptr)
	{
	}
};

USTRUCT(BlueprintType)
struct FS_SKILLSET
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FirstSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* SecondSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ThirdSkill;

	FS_SKILLSET()
		: FirstSkill(nullptr)
		, SecondSkill(nullptr)
		, ThirdSkill(nullptr)
	{
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UCPP_CompMontage : public UActorComponent
{
	GENERATED_BODY()

public:
	UCPP_CompMontage();
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	bool m_isPlayMontage;

	UPROPERTY()
	UAnimMontage* m_InputMontage;

	UPROPERTY()
	UAnimMontage* m_PlayMontage;

	UPROPERTY()
	UCPP_MetaActionData* m_InputMetaMontage;

	UPROPERTY()
	UCPP_MetaActionData* m_PlayMetaMontage;

	UPROPERTY()
	FTimerHandle m_InputHandleMontage;

	UPROPERTY()
	ACPP_MainCharacter* m_MainCharacter;

	UPROPERTY()
	USkeletalMeshComponent* m_Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation|Policy", meta = (AllowPrivateAccess = "true"))
	bool m_isRuntimeCancelWindow;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<UAnimMontage*> AttackMontages;

private:
	bool enablePlay();
	void eraseMontage();
	void playMontage();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	int32 GetActionPriority(E_ACTIONTYPE actionType);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void e_setInputMontage(UAnimMontage* Montage);

	bool IsMontagePlaying() const { return m_isPlayMontage; }

	UFUNCTION(BlueprintCallable)
	void e_PlayMontage(UAnimMontage* Montage);

	UFUNCTION(BlueprintCallable)
	bool PlayCurrentWeaponMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<E_WeaponTypes, FS_DodgeMontageSet> DodgeMontages;

	void PlayDodgeMontage(E_WeaponTypes WeaponType, E_DodgeDirection Direction);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	TMap<E_WeaponTypes, FS_SKILLSET> SkillMontage;

	UFUNCTION(BlueprintCallable)
	void PlaySkillByIndex(int32 SkillIndex);

	UFUNCTION(BlueprintCallable)
	void PlaySkill1();
	UFUNCTION(BlueprintCallable)
	void PlaySkill2();
	UFUNCTION(BlueprintCallable)
	void PlaySkill3();

	UFUNCTION(BlueprintPure, Category = "Animation|Skill")
	bool IsSkillMontagePlaying() const
	{
		return m_isPlayMontage;
	}

	UFUNCTION(BlueprintCallable, Category = "Animation|Policy")
	void OpenCancelWindow();

	UFUNCTION(BlueprintCallable, Category = "Animation|Policy")
	void CloseCancelWindow();

	UFUNCTION(BlueprintPure, Category = "Animation|Policy")
	bool IsCancelWindowOpen() const { return m_isRuntimeCancelWindow; }
};
