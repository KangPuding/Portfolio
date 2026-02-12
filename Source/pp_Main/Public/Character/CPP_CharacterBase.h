#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_CharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS(Abstract, NotBlueprintable)
class ACPP_CharacterBase : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
public:
	ACPP_CharacterBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
