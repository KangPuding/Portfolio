#include "Character/CPP_CharacterBase.h"

ACPP_CharacterBase::ACPP_CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPP_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

