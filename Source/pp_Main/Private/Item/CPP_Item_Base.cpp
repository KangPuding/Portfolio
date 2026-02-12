#include "Item/CPP_Item_Base.h"
#include "Kismet/GameplayStatics.h"

ACPP_Item_Base::ACPP_Item_Base()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);
	DefaultSceneRoot->SetMobility(EComponentMobility::Movable);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(DefaultSceneRoot);
	StaticMesh->SetMobility(EComponentMobility::Movable);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(StaticMesh);

	m_OverlayMat = nullptr;
}

void ACPP_Item_Base::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->SetSimulatePhysics(true);
	
}

void ACPP_Item_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_Item_Base::HighLight(bool Enable)
{
	if (Enable)
		StaticMesh->SetOverlayMaterial(m_OverlayMat);
	else
		StaticMesh->SetOverlayMaterial(nullptr);
}


