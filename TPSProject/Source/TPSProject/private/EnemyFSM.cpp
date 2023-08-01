// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <Components/CapsuleComponent.h>
// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	//���忡�� ATPSPlayer Ÿ�� ã�ƿ���
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass());
	//ATPSPlayer Ÿ������ ĳ����
	target = Cast<ATPSPlayer>(actor);
	//���� ��ü ��������
	me = Cast<AEnemy>(GetOwner());

	// ...
	
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}

	// ...
}

void UEnemyFSM::IdleState()
{
	// �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// ���� ��� �ð��� ���ð��� �ʰ��ߴٸ�
	if (currentTime > idleDelayTime)
	{
		//�̵� ���·� ��ȯ
		mState = EEnemyState::Move;
		//��� �ð� �ʱ�ȭ
		currentTime = 0;
	}

}

void UEnemyFSM::MoveState()
{
	// Ÿ�� �������� �ʿ���
	FVector destination = target->GetActorLocation();
	// ������ �ʿ���
	FVector dir = destination - me->GetActorLocation();
	// �������� �̵�
	me->AddMovementInput(dir.GetSafeNormal());

	//Ÿ��� ��������� ���� ���·� ��ȯ�ϰ� �ʹ�.
	//���� �Ÿ��� ���ݹ��� �ȿ� ������
	if (dir.Size() < attackRange)
	{
		//���� ���·� ��ȯ
		mState = EEnemyState::Attack;
	}
}

void UEnemyFSM::AttackState()
{
	//�ð��� �귯���Ѵ�.
	currentTime += GetWorld()->DeltaTimeSeconds;
	//���ݽð��� ��
	if (currentTime > attackDelayTime)
	{
		//�����ϰ� �ʹ�
		
		//��� �ð� �ʱ�ȭ
		currentTime = 0;
	}

	//��ǥ : Ÿ��� ���� ������ ����� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	//Ÿ����� �Ÿ��� �ʿ��ϴ�.
	float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());
	//Ÿ����� �Ÿ��� ���� ������ ������ϱ�
	if (distance > attackRange)
		//���¸� �̵����� ��ȯ
		mState = EEnemyState::Move;
}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	// ��� �ð��� ���ð��� �ʰ��ߴٸ�
	if (currentTime > damageDelayTime)
	{
		//��� ���·� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Idle;
		currentTime = 0;
	}
}

void UEnemyFSM::DieState()
{
	//��� �Ʒ��� ���������ʹ�.
	//��ӿ P = P0 + vt
	FVector P0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * dieSpeed * GetWorld()->DeltaTimeSeconds;
	FVector P = P0 + vt;
	me->SetActorLocation(P);

	if (P.Z < -200.0f)
	{
		me->Destroy();
	}

}

void UEnemyFSM::OnDamageProcess()
{
	hp--;
	if (hp > 0)
	{
		//���¸� �ǰ����� ��ȯ
		mState = EEnemyState::Damage;
	}

	else
	{
		//���¸� �������� ��ȯ
		mState = EEnemyState::Die;
		//ĸ�� �浹ü ��Ȱ��ȭ
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
