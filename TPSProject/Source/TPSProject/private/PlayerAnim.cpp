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
		//�̵��ӵ� �ʿ�
		FVector velocity = player->GetVelocity();
		//�÷��̾��� ���� ���� �ʿ�
		FVector forwardVector = player->GetActorForwardVector();
		//���ǵ忡 ��(����) �Ҵ�
		speed = FVector::DotProduct(forwardVector, velocity);
		//�¿� �ӵ� �Ҵ�
		FVector rightVector = player->GetActorRightVector();
		direction = FVector::DotProduct(rightVector, velocity);
	


		//�÷��̾ ���� ���߿� �ִ��� ����
		auto movement = player->GetCharacterMovement();
		bIsInAir = movement->IsFalling();
	}
}

void UPlayerAnim::PlayAttackAnim()
{
	Montage_Play(attackAnimMontage);
}
