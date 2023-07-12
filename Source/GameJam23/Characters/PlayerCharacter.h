// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
UCLASS()
class GAMEJAM23_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(const float Value);
	void MoveRight(const float Value);

private:
	/*
	 * Components
	 */	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;	

	/*
	 * Miscellaneous
	 */
	UPROPERTY()
	class ACustomPlayerController* CustomPlayerController;
	
public:
	FORCEINLINE ACustomPlayerController* GetCustomPlayerController() { return CustomPlayerController; }
	FORCEINLINE UCameraComponent* GetCameraComponent() { return FollowCamera; }
};
