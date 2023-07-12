// Fill out your copyright notice in the Description page of Project Settings.


#include "FurnitureActor.h"


AFurnitureActor::AFurnitureActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

void AFurnitureActor::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMeshBeginOverlap);
	MeshComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnMeshEndOverlap);
}

void AFurnitureActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFurnitureActor::OnMeshBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOverlapping = true;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("BeginOverlap"));
}

void AFurnitureActor::OnMeshEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOverlapping = false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EndOverlap"));
}