// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//상태머신 기억 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = FSM)
		EEnemyState animState;
	//공격 상태 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		bool bAttackPlay = false;

	//공격 애니메이션 끝나는 이벤트 함수
	UFUNCTION(BlueprintCallable, Category = FSMEvent)
		void OnEndAttackAnimation();

	//피격 애니메이션 재생 함수
	//구현부가 없어서 원래 실패해야하는데 BlueprintImplementableEvent로 인해 블루프린트에서 구현해준다 판단해 빌드가 된다.
	UFUNCTION(BlueprintImplementableEvent, Category = FSMEvent)
		void PlayDamageAnim(FName sectionName);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
		bool bDieDone = false;

};
