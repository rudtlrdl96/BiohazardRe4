// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item/ItemData.h"
#include "Interface\BInterface_WeaponPutOut.h"
#include "Interface\BInterface_WeaponPutAway.h"
#include "Interface\BInterface_WeaponShoot.h"
#include "Interface\BInterface_WeaponHandSocketSwap.h"
#include "Interface\BInterface_WeaponReload.h"
#include "Interface\BInterface_KnifeAttack.h"
#include "BLeon.generated.h"

struct FInputActionInstance;
class UBFsm;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class ABLeonWeapon;
class UCapsuleComponent;

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UENUM(BlueprintType)
enum class ELeonState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walk			UMETA(DisplayName = "Walk"),
	Jog				UMETA(DisplayName = "Jog"),
	Aim				UMETA(DisplayName = "Aim"),
	KnifeAttack		UMETA(DisplayName = "KnifeAttack"),
	KickAttack		UMETA(DisplayName = "KickAttack"),
	Damage			UMETA(DisplayName = "Damage")
};

UENUM(BlueprintType)
enum class ELeonAim : uint8
{
	Start	UMETA(DisplayName = "Start"),
	Loop	UMETA(DisplayName = "Loop"),
	End		UMETA(DisplayName = "End"),
};

UENUM(BlueprintType)
enum class ELeonHealth : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Danger	UMETA(DisplayName = "Danger"),
};

UENUM(BlueprintType)
enum class ELeonWeaponSwap : uint8
{
	None	UMETA(DisplayName = "None"),
	PutOut	UMETA(DisplayName = "PutOut"),
	PutAway	UMETA(DisplayName = "PutAway"),
};

UENUM(BlueprintType)
enum class ELeonKnifeAttackState: uint8
{
	EnterAttack	UMETA(DisplayName = "EnterAttack"),
	LeftAttack	UMETA(DisplayName = "LeftAttack"),
	RightAttack UMETA(DisplayName = "RightAttack"),
	ResetAttack UMETA(DisplayName = "ResetAttack"),
};

UENUM(BlueprintType)
enum class ELeonWeaponAnim : uint8
{
	Empty	UMETA(DisplayName = "Empty"),
	Pistol	UMETA(DisplayName = "Pistol"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Rifle   UMETA(DisplayName = "Rifle"),
	Knife   UMETA(DisplayName = "Knife"),
	Grenade UMETA(DisplayName = "Grenade"),
};

UENUM(BlueprintType)
enum class ELeonDirection : uint8
{
	F	UMETA(DisplayName = "F"),
	FR  UMETA(DisplayName = "FR"),
	R   UMETA(DisplayName = "R"),
	BR  UMETA(DisplayName = "BR"),
	B   UMETA(DisplayName = "B"),
	BL  UMETA(DisplayName = "BL"),
	L   UMETA(DisplayName = "L"),
	FL  UMETA(DisplayName = "FL"),
};

UENUM(BlueprintType)
enum class ELeonDamageDirection : uint8
{
	LT	UMETA(DisplayName = "LT"),
	LU  UMETA(DisplayName = "LU"),
	RT  UMETA(DisplayName = "RT"),
	RU  UMETA(DisplayName = "RU"),
	FT  UMETA(DisplayName = "FU"),
	FU  UMETA(DisplayName = "FU"),
	B   UMETA(DisplayName = "B"),
};

UCLASS()
class BIOHAZARDRE4_API ABLeon : public ACharacter, 
	public IBInterface_WeaponPutAway, public IBInterface_WeaponPutOut, public IBInterface_WeaponShoot, 
	public IBInterface_WeaponHandSocketSwap, public IBInterface_WeaponReload, public IBInterface_KnifeAttack
{
	GENERATED_BODY()

private:
	static const FVector StandSocketOffset;
	static const FVector GunAimSocketOffset;
	static const FVector GreanadeAimSocketOffset;

	static const float StandSpringArmLength;
	static const float GunAimSpringArmLength;
	static const float GreanadeAimSpringArmLength;

public:
	// Sets default values for this character's properties
	ABLeon();
	~ABLeon();

	// Called every frame
	virtual void Tick(float _DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 플레이어가 사용하는 무기를 교체합니다
	void ChangeUseWeapon(EItemCode _WeaponCode);

	// 플레이어의 정면 벡터와 무브 벡터의 각도 차이를 반환합니다
	UFUNCTION(BlueprintCallable)
	double GetMoveAngle() const;

	// 플레이어의 정면 벡터와 입력 벡터의 각도 차이를 반환합니다
	UFUNCTION(BlueprintCallable)
	double GetInputAngle() const;

	// 플레이어가 사용하는 무기 아이템 코드를 반환합니다
	inline EItemCode GetUseWeaponCode() const
	{
		return UseWeaponCode;
	}

	// 현재 플레이어가 앉은 상태인지 반환합니다
	UFUNCTION(BlueprintCallable)
	inline bool IsCrouch() const
	{
		return bIsCrouch;
	}

	// 현재 플레이어의 FSM 상태를 반환합니다
	UFUNCTION(BlueprintCallable)
	ELeonState GetCurrentFSMState() const;	

	// 현재 플레이어의 무기 애니메이션 상태를 반환합니다
	UFUNCTION(BlueprintCallable)
	inline ELeonWeaponAnim GetWeaponAnimationState() const
	{
		return GetUseWeaponAnimation(UseWeaponCode);
	}

	// 현재 플레이어의 체력 애니메이션 상태를 반환합니다
	UFUNCTION(BlueprintCallable)
	inline ELeonHealth GetHealthState() const
	{
		return LeonHealth;
	}	

	// 현재 플레이어의 조준 상태를 반환합니다
	UFUNCTION(BlueprintCallable)
	inline ELeonAim GetAimState() const
	{
		return LeonAim;
	}

	// 현재 플레이어의 나이프 공격 상태를 반환합니다
	UFUNCTION(BlueprintCallable)
	inline ELeonKnifeAttackState GetKnifeAttackState() const
	{
		return KnifeAttackState;
	}

	// 현재 플레이어가 달리는 상태인지 반환합니다
	UFUNCTION(BlueprintCallable)
	inline bool IsJog() const
	{
		return bIsJog;
	}	

	// 현재 플레이어의 이동 방향을 반환합니다
	UFUNCTION(BlueprintCallable)
	inline FVector GetMoveDirection() const
	{
		return MoveDir;
	}

	// 현재 플레이어의 입력 방향을 반환합니다
	UFUNCTION(BlueprintCallable)
	inline FVector GetInputDirection() const
	{
		return MoveInput;
	}	

	// 현재 플레이어의  입력 방향을 월드 기준으로 반환합니다
	UFUNCTION(BlueprintCallable)
	inline FVector GetWorldInputDirection() const
	{
		FVector Result = MoveInput;
		VPlayerCameraToWorld(Result);
		return Result;
	}

	// 현재 플레이어의 카메라 회전 입력을 반환합니다
	UFUNCTION(BlueprintCallable)
	inline FVector GetLookDirection() const
	{
		return LookInput;
	}	
	
	// 플레이어의 무기 변경 상태를 반환합니다
	UFUNCTION(BlueprintCallable)
	inline ELeonWeaponSwap GetWeaponSwapState() const
	{
		return LeonWeaponSwap;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetCameraDirection() const;

	// 현재 플레이어의 입력 방향을 Enum으로 반환합니다
	UFUNCTION(BlueprintCallable)
	ELeonDirection GetLeonDirection() const;


	// 플레이어가 무기들 들고 있는지 반환합니다
	UFUNCTION(BlueprintCallable)
	inline bool HasWeapon() const
	{
		return nullptr != CurrentWeapon;
	}

	// 플레이어가 총 발사 반동상태인지 반환합니다
	UFUNCTION(BlueprintCallable)
	inline bool IsGunRecoil() const
	{
		return bIsGunRecoil;
	}

	// 
	UFUNCTION(BlueprintCallable)
	inline bool IsGunReload() const
	{
		return bIsGunReload;
	}

	UFUNCTION(BlueprintCallable)
	FTransform GetWeaponLeftSocketTransform() const;


	virtual void WeaponPutOutStart() override;
	virtual void WeaponPutOutEnd() override;
	virtual void WeaponPutAwayStart() override;
	virtual void WeaponPutAwayEnd() override;

	virtual void WeaponShootStart() override;
	virtual void WeaponShootEnd() override;

	virtual void AttachLeftHandSocket() override;
	virtual void AttachRightHandSocket() override;

	virtual void ReloadStart() override;
	virtual void Reload() override;
	virtual void ReloadEnd() override;
	virtual bool AbleReload() const override;
	
	virtual void KnifeComboStart() override;
	virtual void KnifeComboEnd() override;

	virtual void KnifeAttackStart() override;
	virtual void KnifeAttackEnd() override;

	virtual void KnifeCollisionActive() override;
	virtual void KnifeCollisionDisable() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsMove : 1 = false;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsJog : 1 = false;	
	
	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsAim : 1 = false;	
	
	UPROPERTY(EditAnywhere, Category = Animation)
	uint32 bIsCombat : 1 = false;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsJogTrigger : 1 = false;	
	
	UPROPERTY(VisibleAnywhere, Category = Animation)
	uint32 bIsCrouch : 1 = false;		

	UPROPERTY(VisibleAnywhere, Category = Animation)
	ELeonState LeonFSMState = ELeonState::Idle;
	
	UPROPERTY(EditAnywhere, Category = Animation)
	ELeonWeaponAnim LeonWeaponState = ELeonWeaponAnim::Empty;

	UPROPERTY(EditAnywhere, Category = Animation)
	EItemCode PutOutWeapon = EItemCode::Empty;
			
	UPROPERTY(EditAnywhere, Category = Animation)
	ELeonHealth LeonHealth = ELeonHealth::Normal;	
	
	UPROPERTY(VisibleAnywhere, Category = Animation)
	ELeonAim LeonAim = ELeonAim::Start;	
	
	UPROPERTY(VisibleAnywhere, Category = Animation)
	ELeonWeaponSwap  LeonWeaponSwap = ELeonWeaponSwap::None;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	EItemCode  UseWeaponCode = EItemCode::Empty;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	float AimUpdateTime = 0.0f;	

	UPROPERTY(VisibleAnywhere, Category = Animation)
	float CombatTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 10.0f;	
	UPROPERTY(EditAnywhere, Category = Input)
	float MaxTurnSpeed = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LookAction = nullptr;	

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* ShootAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* AimAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* JogAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* CrouchAction = nullptr;	
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* InteractionAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* QuickSlotActions[8];

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* WeaponPutAwayAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* WeaponReloadAction = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* PlayerCamera = nullptr;	
	
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	UCapsuleComponent* KnifeAttackCollision = nullptr;

	FVector MoveDir = FVector::ZeroVector;
	FVector MoveInput = FVector::ZeroVector;
	FVector LookInput = FVector::ZeroVector;
	double JogTurnAngle = 0.0;

	UBFsm* FsmComp = nullptr;

	ABLeonWeapon* CurrentWeapon = nullptr;

	FName LerpSocketStart = "";
	FName LerpSocketEnd = "";

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FAlphaBlend AbsolutAlphaBlend;	
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FAlphaBlend PutOutTurnAlphaBlend;	
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FAlphaBlend PutAwayLocationAlphaBlend;

	FAlphaBlend SocketLocationBlend;
	FAlphaBlend SocketRotationBlend;

	float SocketLocationLerpTime = 0.0f;
	float SocketRotationLerpTime = 0.0f;

	float SocketLocationLerpSpeed = 1.0f;
	float SocketRotationLerpSpeed = 1.0f;

	float SocketSwapLocationSpeed = 50.0f;
	float SocketSwapRotationSpeed = 50.0f;

	uint32 bIsLerpSocket : 1 = false;
	uint32 bIsLerpWeaponChange : 1 = false;

	uint32 bIsWeaponShootTrigger: 1 = false;
	uint32 bIsGunRecoil: 1 = false;
	uint32 bIsGunReload : 1 = false;

	ELeonKnifeAttackState KnifeAttackState = ELeonKnifeAttackState::EnterAttack;

	uint32 bAbleComboInput : 1 = false;
	uint32 bAbleNextCombo : 1 = false;
	uint32 bIsComboEnd : 1 = false;

	double CurrentHP = 1000.0;
	double MaxHP = 1000.0;

	void PlayMove(const FInputActionInstance& _MoveAction);
	void PlayIdle(const FInputActionInstance& _MoveAction);

	virtual void PlayLook(const FInputActionInstance& _LookAction);
	virtual void PlayLook(const FVector2D& _LookAction);

	void StopLook();

	void SpringArmUpdate(float _DeltaTime);
	void UseWeaponUpdate(float _DeltaTime);
	void WeaponSocketUpdate(float _DeltaTime);
	void SocketSwapUpdate(float _DeltaTime);

	void VPlayerCameraToWorld(FVector& _Result) const;

	void JogLookAt(float _DeltaTime);
	
	bool AbleKnifeAttack() const;
	bool AbleShoot() const;
	void KnifeComboCheck();
	void Attack();

	void Aim(float _DeltaTime);
	void ReloadActive();

	bool AbleWeaponSwap() const;
	void UseQuickSlot(const uint32 _Index);

	void ActiveJog();
	void DisableJog();
	void ActiveAim();
	void DisableAim();
	void TryCrouch();
	void TryInteraction();
	bool AbleAim() const;

	void CreateSprintArm();
	void CreateFSM();
	void CreateCollision();

	ABLeonWeapon* CreateWeapon(EItemCode _WeaponCode);
	void DeleteCurrentWeapon();

	ELeonWeaponAnim GetUseWeaponAnimation(EItemCode _WeaponCode) const;

	/* FSM */
	void IdleEnter();
	void IdleUpdate(float _DeltaTime);
	void IdleExit();

	void WalkEnter();
	void WalkUpdate(float _DeltaTime);
	void WalkExit();

	void JogEnter();
	void JogUpdate(float _DeltaTime);
	void JogExit();

	void AimEnter();
	void AimUpdate(float _DeltaTime);
	void AimExit();

	void KnifeAttackEnter();
	void KnifeAttackUpdate(float _DeltaTime);
	void KnifeAttackExit();

	void DamageEnter();
	void DamageUpdate(float _DeltaTime);
	void DamageExit();
};