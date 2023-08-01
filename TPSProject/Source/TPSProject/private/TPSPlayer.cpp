// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "Bullet.h"
#include "EnemyFSM.h"

// SpringArm 컴포넌트를 사용할 수 있게
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
// 생성자
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// CreateDefaultSubobject 팩토리 함수를 이용 USpringArmComponent 인스턴스 생성 springArmComp 변수에 저장
	springArmComp=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0, 70, 90));
	springArmComp->TargetArmLength = 400;
	springArmComp->bUsePawnControlRotation = false;


	// Camera 컴포넌트 붙이기
	tpsCamComp=CreateDefaultSubobject<UCameraComponent>(TEXT("TpsCamComp"));
	tpsCamComp->SetupAttachment(springArmComp);
	tpsCamComp->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = true;

	JumpMaxCount = 2;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComponent"));
	gunMeshComp->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<USkeletalMesh>TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (TempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	//스나이퍼 컴포넌트 등록
	sniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGunComp"));
	//부모 컴포넌트를 Mesh 컴포넌트로 설정
	sniperGunComp->SetupAttachment(GetMesh());
	//스태틱메시 데이터 로드
	ConstructorHelpers::FObjectFinder<UStaticMesh>TempSniperMesh(TEXT("/Script/Engine.StaticMesh'/Game/SniperGun/sniper1.sniper1'"));
	//데이터 로드에 성공했다면
	if (TempSniperMesh.Succeeded())
	{
		//스테틱메시 데이터 할당
		sniperGunComp->SetStaticMesh(TempSniperMesh.Object);
		//위치 조정
		sniperGunComp->SetRelativeLocation(FVector(-22, 55, 120));
		// 크기 조정
		sniperGunComp->SetRelativeScale3D(FVector(0.15f));
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	//스나이퍼 UI 위젯 인스턴스 생성
	_sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	//일반 조준 UI 크로스헤어 인스턴스 생성
	_crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);
	//일반 조준 UI 등록
	_crosshairUI->AddToViewport();

	ChangeToSniperGun();

	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move();
	

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::InputHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::InputVertical);
	
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed, this, &ATPSPlayer::InputJump);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::InputFire);

	//총 교체 이벤트처리 함수 바인딩
	PlayerInputComponent->BindAction(TEXT("GrenadeGun"), IE_Pressed, this, &ATPSPlayer::ChangeToGrenadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), IE_Pressed, this, &ATPSPlayer::ChangeToSniperGun);

	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Pressed, this, &ATPSPlayer::SniperAim);
	PlayerInputComponent->BindAction(TEXT("Sniper"), IE_Released, this, &ATPSPlayer::SniperAim);

}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::InputHorizontal(float value)
{
	direction.Y = value;
}

void ATPSPlayer::InputVertical(float value)
{
	direction.X = value;
}

void ATPSPlayer::InputJump()
{
	Jump();
}

void ATPSPlayer::Move()
{
	//컨트롤하고 있는 방향(상대좌표)에서의 이동으로 적용하기 위해
	direction = FTransform(GetControlRotation()).TransformVector(direction);


 //FVector P0 = GetActorLocation();
  //FVector vt = direction * walkSpeed * DeltaTime;
  //FVector P = P0 + vt;
 //SetActorLocation(P);
	

	// 위에 등속운동 공식 P(결과위치) = P0(현재 위치) + v(속도) * t(시간)을 제공해주는 컴포넌트를 제공
	// Character Movement
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

void ATPSPlayer::InputFire()
{
	//유탄총 사용시
	if (bUsingGrenadeGun)
	{
		FTransform firePosition = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition);
	}
	//스나이퍼 사용시
	else
	{
		//LineTrace의 시작 위치
		FVector startPos = tpsCamComp->GetComponentLocation();
		//LineTrace의 종료 위치
		FVector endPos = tpsCamComp->GetComponentLocation() + tpsCamComp->GetForwardVector() * 5000;
		//LinceTrace의 충돌 정보를 담을 변수
		FHitResult hitInfo;
		//충돌 옵션 설정 변수
		FCollisionQueryParams params;
		// 자기 자신(플레이어)는 충돌에서 제외
		params.AddIgnoredActor(this);
		//Channel 필터를 이용한 LineTrace 충돌 검출(충돌정보, 시작위치, 종료위치, 검출 채널, 충돌 옵션)
		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);
		if (bHit)
		{
			//총알 파편 효과 트랜스폼
			FTransform bulletTrans;
			//부딪힌 위치 할당
			bulletTrans.SetLocation(hitInfo.ImpactPoint);
			//총알 파편 효과 인스턴스 생성
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, bulletTrans);
			
			auto hitComp = hitInfo.GetComponent();
			//만약 컴포넌트에 물리가 적용되어 있다면
			if (hitComp && hitComp->IsSimulatingPhysics())
			{
				//날려버릴 힘과 방향이 필요
				FVector force = -hitInfo.ImpactNormal * hitComp->GetMass() * 50000;
				//그 방향으로 날려버리고 싶다.
				hitComp->AddForce(force);
			}
			auto enemy = hitInfo.GetActor()->GetDefaultSubobjectByName(TEXT("FSM"));
			if (enemy)
			{
				auto enemyFSM = Cast<UEnemyFSM>(enemy);
				enemyFSM->OnDamageProcess();
			}
		}

	}

}

// 유탄 총으로 변경
void ATPSPlayer::ChangeToGrenadeGun()
{
	//유탄총 사용 중으로 체크
	bUsingGrenadeGun = true;
	sniperGunComp->SetVisibility(false);
	gunMeshComp->SetVisibility(true);
}

// 스나이퍼건으로 변경
void ATPSPlayer::ChangeToSniperGun()
{
	bUsingGrenadeGun = false;
	sniperGunComp->SetVisibility(true);
	gunMeshComp->SetVisibility(false);
}

// 스나이퍼 조준
void ATPSPlayer::SniperAim()
{
	if (bUsingGrenadeGun)
	{
		return;
	}
	
	if (bSniperAim == false)
	{
		bSniperAim = true;
		_sniperUI->AddToViewport();
		tpsCamComp->SetFieldOfView(45.0f);
		//일반 조준 UI 제거
		_crosshairUI->RemoveFromParent();
	}

	else
	{
		bSniperAim = false;
		_sniperUI->RemoveFromParent();
		tpsCamComp->SetFieldOfView(90.0f);
		_crosshairUI->AddToViewport();
	}
}

