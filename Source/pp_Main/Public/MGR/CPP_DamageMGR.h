#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CPP_DamageMGR.generated.h"

class UCPP_WeaponBase;
class ACPP_MainCharacter;

UCLASS(Blueprintable)
class UCPP_DamageMGR : public UActorComponent
{
	GENERATED_BODY()

private:
	float m_StatisticsScale;
	float m_EquipScale;
	float m_BuffScale;


public:	
	UCPP_DamageMGR();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float m_Damage;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void CalculateDamage(float Damage);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetFinalDamage() const;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetScales(float InStat, float InEquip, float InBuff);
};
