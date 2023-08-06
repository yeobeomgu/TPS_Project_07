// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	//플레이어의 이동속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		float speed = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		float direction = 0;

	//매프레임 갱신되는 함수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerAnim)
		bool bIsInAir = false;

	//재생할 공격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, Category = PlayerAnim)
		class UAnimMontage* attackAnimMontage;
	//공격 애니메이션 재생함수
	void PlayAttackAnim();

	
};
