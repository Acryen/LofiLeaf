// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableActor.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameJam23/Characters/PlayerCharacter.h"
#include "GameJam23/PlayerControllers/CustomPlayerController.h"


ACollectableActor::ACollectableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollisionComponent->SetupAttachment(GetRootComponent());
	SphereCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ACollectableActor::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginoverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);

	QuestStruct.FurnitureToCreate = FurnitureToCreate;
	QuestStruct.StonesToCollect = StonesToCollect;
	QuestStruct.WoodToCollect = WoodToCollect;
	QuestStruct.WoolToCollect = WoolToCollect;
}

void ACollectableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacter) return;

	FRotator LookAtDirection = (PlayerCharacter->GetCameraComponent()->GetComponentLocation() - WidgetComponent->GetComponentLocation()).ToOrientationRotator();
	WidgetComponent->SetWorldRotation(LookAtDirection);
}

void ACollectableActor::OnSphereBeginoverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	ACustomPlayerController* CustomPlayerController = PlayerCharacter->GetCustomPlayerController();
	if (!CustomPlayerController) return;
	CustomPlayerController->TogglePressFText();
	CustomPlayerController->bCanInteractWithQuestSheet = true;
	CustomPlayerController->CollectableActor = this;

	WidgetComponent->SetVisibility(true);
}

void ACollectableActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;

	ACustomPlayerController* CustomPlayerController = PlayerCharacter->GetCustomPlayerController();
	if (!CustomPlayerController) return;
	CustomPlayerController->TogglePressFText();
	CustomPlayerController->bCanInteractWithQuestSheet = false;
	CustomPlayerController->CollectableActor = nullptr;

	WidgetComponent->SetVisibility(false);
}