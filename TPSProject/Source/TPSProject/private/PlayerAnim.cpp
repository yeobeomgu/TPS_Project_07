// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>


void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto ownerPawn = TryGetPawnOwner();
	auto player = Cast<ATPSPlayer>(ownerPawn);

	if (player)
	{
		//이동속도 필요
		FVector velocity = player->GetVelocity();
		//플레이어의 전방 벡터 필요
		FVector forwardVector = player->GetActorForwardVector();
		//스피드에 값(내적) 할당
		speed = FVector::DotProduct(forwardVector, velocity);
		//좌우 속도 할당
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	


		//플레이어가 현재 공중에 있는지 여부
		auto movement = player->GetCharacterMovement();
		bIsInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}
