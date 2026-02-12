#include "Item/CPP_WeaponBase.h"
#include "MGR/CPP_DamageMGR.h"
#include "Kismet/GameplayStatics.h"

ACPP_WeaponBase::ACPP_WeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;


	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(DefaultSceneRoot);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(StaticMesh);

	WeaponTraceBegin = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponTraceBegin"));
	WeaponTraceBegin->SetupAttachment(StaticMesh);

	WeaponTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponTraceEnd"));
	WeaponTraceEnd->SetupAttachment(StaticMesh);

}

void ACPP_WeaponBase::BeginPlay()
{
	Super::BeginPlay();

	m_CurrentDamage = FindComponentByClass<UCPP_DamageMGR>();
	if (!m_CurrentDamage)
	{
		UE_LOG(LogTemp, Error, TEXT("DamageMGR not found on %s"), *GetName());
	}
}

void ACPP_WeaponBase::Attack(bool isAttack)
{
	UE_LOG(LogTemp, Error, TEXT("isAttack : %s"),isAttack ? TEXT("true") : TEXT("false"));
	Capsule->SetGenerateOverlapEvents(isAttack);
	m_HitActor.Empty();
}

void ACPP_WeaponBase::OnAttack(const FHitResult& HitInfo, AActor* HitActor)
{
	if (m_HitActor.Contains(HitActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("duplication"));
	}
	else
	{
		m_HitActor.Add(HitActor);
		m_TotalDamage = IsValid(m_CurrentDamage) ? m_CurrentDamage->GetFinalDamage() : 0.f;
	
		FVector HitDirection = GetActorForwardVector();
		AController* InstigatorController = GetInstigatorController();
		AActor* DamageCauser = this;

		UGameplayStatics::ApplyPointDamage(
			HitActor,
			m_TotalDamage,
			HitDirection,
			HitInfo,
			InstigatorController,
			DamageCauser,
			UDamageType::StaticClass()
		);
		UE_LOG(LogTemp, Warning, TEXT("m_TotalDamage : %f"), m_TotalDamage);
	}
}

void ACPP_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
