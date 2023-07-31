// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"


UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	//USpringArmComponent ������Ʈ �Ӽ� �߰�
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* springArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* tpsCamComp;

	// �¿� ȸ�� ó��
	void Turn(float value);

	// ���� ȸ�� ó��
	void LookUp(float value);

	// �̵��ӵ�
	UPROPERTY(VisibleAnywhere, Category = PlayerSetting)
		float walkSpeed = 600;
	//�̵�����
	FVector direction;

	// �¿� �̵� �Է� �̺�Ʈ ó�� �Լ�
	void InputHorizontal(float value);
	//���� �̵� �Է� �̺�Ʈ ó�� �Լ�
	void InputVertical(float value);
	
	// ���� �Է� �̺�Ʈ ó�� �Լ�
	void InputJump();

	// �÷��̾� �̵�ó��
	void Move();

	//�� ���̷�Ż�޽�
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class USkeletalMeshComponent* gunMeshComp;

	//�Ѿ� ����
	UPROPERTY(EditDefaultsOnly, Category = BulletFactory)
		UClass* bulletFactory;
		//TSubclassOf<class ABullet> bulletFactory;

	//�Ѿ� �߻� ó�� �Լ�
	void InputFire();

	//�������۰� ����ƽ�޽� �߰�
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
		class UStaticMeshComponent* sniperGunComp;

	//��ź�� ��� ������ ����
	bool bUsingGrenadeGun = true;
	//��ź������ ����
	void ChangeToGrenadeGun();
	//�������۰����� ����
	void ChangeToSniperGun();

	//�������� ����
	void SniperAim();
	//�������� ���� ������ ����
	bool bSniperAim = false;

	//�������� UI���� ����
	UPROPERTY(EditDefaultsOnly, Category = SniperUIFactory)
		TSubclassOf<class UUserWidget>sniperUIFactory;
	//�������� UI ���� �ν��Ͻ�
	class UUserWidget* _sniperUI;
};