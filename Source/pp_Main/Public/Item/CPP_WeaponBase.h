#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "CPP_WeaponBase.generated.h"

class UCPP_DamageMGR;

UCLASS()
class ACPP_WeaponBase : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TSet<AActor*> m_HitActor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* WeaponTraceBegin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USceneComponent* WeaponTraceEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	UCPP_DamageMGR* m_CurrentDamage;

	float m_TotalDamage;

public:	
	ACPP_WeaponBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Attack(bool isAttack);
	UFUNCTION(BlueprintCallable)
	void OnAttack(const FHitResult& HitInfo, AActor* HitActor);
};
