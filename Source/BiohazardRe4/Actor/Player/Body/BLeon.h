// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item/ItemData.h"
#include "Data/BLeonStat.h"
#include "Interface/BIWeaponPutOut.h"
#include "Interface/BIWeaponPutAway.h"
#include "Interface/BIWeaponShoot.h"
#include "Interface/BIWeaponHandSocketSwap.h"
#include "Interface/BIWeaponReload.h"
#include "Interface/BIKnifeAttack.h"
#include "Interface/BIDamage.h"
#include "Interface/BIPlayerInteractionAnim.h"
#include "Actor/Generic/Interface/BInteraction.h"
#include "BLeon.generated.h"

struct FInputActionInstance;
class UBFsm;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class ABLeonWeapon;
class UCapsuleComponent;
class USceneComponent;
class ABDrawGrenadeAim;
class ABCollisionObserverSphere;
class ABCollisionObserverCapsule;
class ABInventoryActor;
class IBInteraction;
class UDamageType;

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
	Throwing		UMETA(DisplayName = "Throwing"),
	Damage			UMETA(DisplayName = "Damage"),
	Death			UMETA(DisplayName = "Death"),
	ObstacleJump	UMETA(DisplayName = "ObstacleJump"),
	Fall			UMETA(DisplayName = "Fall"),
	Parry			UMETA(DisplayName = "Parry"),
	OpenDoor		UMETA(DisplayName = "OpenDoor"),
	BreakBox		UMETA(DisplayName = "BreakBox"),
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
enum class ELeonKnifeAttackState : uint8
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
	FT  UMETA(DisplayName = "FT"),
	FU  UMETA(DisplayName = "FU"),
	B   UMETA(DisplayName = "B"),
};

UENUM(BlueprintType)
enum class ELeonDamageType : uint8
{
	Small		UMETA(DisplayName = "Small"),
	Medium		UMETA(DisplayName = "Medium"),
	Large		UMETA(DisplayName = "Large"),
	ExLarge		UMETA(DisplayName = "ExLarge"),
	Guard		UMETA(DisplayName = "Guard"),
	Explosion	UMETA(DisplayName = "Explosion"),
};

UENUM(BlueprintType)
enum class ELeonThrowingAnim : uint8
{
	Top			UMETA(DisplayName = "Top"),
	Bottom		UMETA(DisplayName = "Bottom"),
};

UENUM(BlueprintType)
enum class ELeonInteractionState : uint8
{
	Empty			UMETA(DisplayName = "Empty"),
	GroggyMonster	UMETA(DisplayName = "GroggyMonster"),
	Obstacle		UMETA(DisplayName = "Obstacle"),
	Cliff			UMETA(DisplayName = "Cliff"),
	Fence			UMETA(DisplayName = "Fence"),
	Door			UMETA(DisplayName = "Door"),
	DropItem		UMETA(DisplayName = "DropItem"),
};

UENUM(BlueprintType)
enum class ELeonJumpState : uint8
{
	MoveStart	UMETA(DisplayName = "MoveStart"),
	Jump		UMETA(DisplayName = "Jump"),
	End			UMETA(DisplayName = "End"),
};

UENUM(BlueprintType)
enum class ELeonFallState : uint8
{
	FallStart	UMETA(DisplayName = "FallStart"),
	FallLoop	UMETA(DisplayName = "FallLoop"),
	FallEnd		UMETA(DisplayName = "FallEnd"),
};

UENUM(BlueprintType)
enum class ELeonFallAnimation : uint8
{
	LowHeight	UMETA(DisplayName = "LowHeight"),
	HighHeight  UMETA(DisplayName = "HighHeight"),
};

UENUM(BlueprintType)
enum class EGunRecoilType : uint8
{
	Pistol	UMETA(DisplayName = "Pistol"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	Rifle	UMETA(DisplayName = "Rifle"),
};

USTRUCT()
struct FPlayerStat
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	float Attack = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float MaxHp = 1000.0f;
	UPROPERTY(VisibleAnywhere)
	float CurrentHp = 1000.0f;
};

UCLASS()
class BIOHAZARDRE4_API ABLeon : public ACharacter,
	public IBIWeaponPutAway, public IBIWeaponPutOut, public IBIWeaponShoot, public IBIWeaponHandSocketSwap, 
	public IBIWeaponReload, public IBIKnifeAttack, public IBIDamage, public IBInteraction, public IBIPlayerInteractionAnim
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

	// �÷��̾ ����ϴ� ���⸦ ��ü�մϴ�
	void ChangeUseWeapon(EItemCode _WeaponCode);

	// �÷��̾��� ���� ���Ϳ� ���� ������ ���� ���̸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	double GetMoveAngle() const;

	// �÷��̾��� ���� ���Ϳ� �Է� ������ ���� ���̸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	double GetInputAngle() const;

	// �÷��̾ ����ϴ� ���� ������ �ڵ带 ��ȯ�մϴ�
	inline EItemCode GetUseWeaponCode() const
	{
		return UseWeaponCode;
	}

	// ���� �÷��̾ ���� �������� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline bool IsCrouch() const
	{
		return bIsCrouch;
	}

	// ���� �÷��̾��� FSM ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	ELeonState GetCurrentFSMState() const;

	// ���� �÷��̾��� ���� �ִϸ��̼� ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonWeaponAnim GetWeaponAnimationState() const
	{
		return GetUseWeaponAnimation(UseWeaponCode);
	}

	// ���� �÷��̾��� ü�� �ִϸ��̼� ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonHealth GetHealthState() const
	{
		return LeonHealth;
	}

	// ���� �÷��̾��� ���� ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonAim GetAimState() const
	{
		return LeonAim;
	}	
	
	// ���� �÷��̾��� ���� ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonJumpState GetJumpState() const
	{
		return JumpState;
	}

	// ���� �÷��̾��� ������ ���� ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonKnifeAttackState GetKnifeAttackState() const
	{
		return KnifeAttackState;
	}

	// ���� �÷��̾ �޸��� �������� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline bool IsJog() const
	{
		return bIsJog;
	}

	// ���� �÷��̾��� �̵� ������ ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline FVector GetMoveDirection() const
	{
		return MoveDir;
	}

	// ���� �÷��̾��� �Է� ������ ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline FVector GetInputDirection() const
	{
		return MoveInput;
	}

	// ���� �÷��̾���  �Է� ������ ���� �������� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline FVector GetWorldInputDirection() const
	{
		FVector Result = MoveInput;
		VPlayerCameraToWorld(Result);
		return Result;
	}

	// ���� �÷��̾��� ī�޶� ȸ�� �Է��� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline FVector GetLookDirection() const
	{
		return LookInput;
	}

	// �÷��̾��� ���� ���� ���¸� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonWeaponSwap GetWeaponSwapState() const
	{
		return LeonWeaponSwap;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetCameraDirection() const;

	// ���� �÷��̾��� �Է� ������ Enum���� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	ELeonDirection GetLeonDirection() const;


	// �÷��̾ ����� ��� �ִ��� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline bool HasWeapon() const
	{
		return nullptr != CurrentWeapon;
	}

	// �÷��̾ �� �߻� �ݵ� �������� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline bool IsGunRecoil() const
	{
		return bIsGunRecoil;
	}

	// �÷����̰� �� ������ �������� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline bool IsGunReload() const
	{
		return bIsGunReload;
	}

	// ���������� ���� ������ Ÿ���� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonDamageType GetDamageType() const
	{
		return DamageType;
	}

	// ���������� ���� ������ ������ ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonDamageDirection GetDamageDirection() const
	{
		return DamageDirection;
	}

	// ������ ��ô �ִϸ��̼��� ��ȯ�մϴ�
	UFUNCTION(BlueprintCallable)
	inline ELeonThrowingAnim GetThrowingAnim() const
	{
		return ThrowingAnim;
	}

	UFUNCTION(BlueprintCallable)
	inline ABLeonWeapon* GetCurrentWeapon() const
	{
		return CurrentWeapon;
	}	

	UFUNCTION(BlueprintCallable)
	inline ELeonFallState GetFallState() const
	{
		return FallState;
	}	
	
	UFUNCTION(BlueprintCallable)
	inline ELeonFallAnimation GetFallAnim() const
	{
		return FallAnimation;
	}
	
	UFUNCTION(BlueprintCallable)
	inline bool IsPlayGetItemAnim() const
	{
		return bIsPlayGetItem;
	}

	UFUNCTION(BlueprintCallable)
	ABInventoryActor* GetInventoryActor() const;


	UFUNCTION(BlueprintCallable)
	FTransform GetWeaponLeftSocketTransform() const;


	virtual void WeaponPutOutStart() override;
	virtual void WeaponPutOutEnd() override;
	virtual void WeaponPutAwayStart() override;
	virtual void WeaponPutAwayEnd() override;

	virtual void WeaponShootStart() override;
	virtual void WeaponShootEnd() override;
	virtual void ThrowingWeapon() override;
	virtual void ThrowingEnd() override;
	virtual void KickAttackStart() override;
	virtual void KickAttackEnd() override;
	virtual void KickEnd() override;

	virtual void AttachLeftHandSocket() override;
	virtual void AttachRightHandSocket() override;

	virtual void ReloadStart() override;
	virtual void Reload() override;
	virtual void ReloadEnd() override;
	virtual bool AbleReload() const override;
	virtual void ShotgunReloadAnimStart() override;

	virtual void KnifeComboStart() override;
	virtual void KnifeComboEnd() override;

	virtual void KnifeAttackStart() override;
	virtual void KnifeAttackEnd() override;

	virtual void KnifeCollisionActive() override;
	virtual void KnifeCollisionDisable() override;

	virtual void DamageEnd() override;

	virtual bool AbleInteraction() const override;
	virtual EInteraction GetInteractionType() const override;
	virtual FVector GetUIPivot() const override;

	virtual void GetItemEnd() override;
	virtual void JumpObstacleEnd() override;
	virtual void FallGravityActive() override;
	virtual void FallTraceActive() override;
	virtual void FallLandingEnd() override;
	virtual void GravityActive() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//************** Animation ���� **************//

	uint32 bIsMove : 1 = false;
	uint32 bIsJog : 1 = false;
	uint32 bIsAim : 1 = false;
	uint32 bIsCombat : 1 = false;
	uint32 bIsJogTrigger : 1 = false;
	uint32 bIsCrouch : 1 = false;
	uint32 bIsPlayGetItem : 1 = false;
	uint32 bIsFallEndCheck : 1 = false;
	uint32 bIsFallLandingEnd: 1 = false;

	ELeonState LeonFSMState = ELeonState::Idle;

	UPROPERTY(EditAnywhere, Category = Animation)
	ELeonWeaponAnim LeonWeaponState = ELeonWeaponAnim::Empty;

	UPROPERTY(EditAnywhere, Category = Animation)
	EItemCode PutOutWeapon = EItemCode::Empty;

	UPROPERTY(EditAnywhere, Category = Animation)
	ELeonHealth LeonHealth = ELeonHealth::Normal;

	ELeonAim LeonAim = ELeonAim::Start;
	ELeonWeaponSwap  LeonWeaponSwap = ELeonWeaponSwap::None;

	float AimUpdateTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FAlphaBlend AbsolutAlphaBlend;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FAlphaBlend PutOutTurnAlphaBlend;
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FAlphaBlend PutAwayLocationAlphaBlend;

	FVector MoveDir = FVector::ZeroVector;
	double JogTurnAngle = 0.0;

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

	uint32 bIsWeaponShootTrigger : 1 = false;
	uint32 bIsGunRecoil : 1 = false;
	uint32 bIsCameraRecoil : 1 = false;
	uint32 bIsGunReload : 1 = false;

	EGunRecoilType GunRecoilType = EGunRecoilType::Pistol;
	float GunRecoilAlpha = 0.0f;

	FName LerpSocketStart = "";
	FName LerpSocketEnd = "";

	uint32 bIsHitEnd : 1 = false;
	uint32 bIsDeathEnd : 1 = false;
	uint32 bIsThrowingEnd : 1 = false;
	uint32 bIsKickEnd : 1 = false;

	FVector InteractionStartLocation = FVector::ZeroVector;
	FRotator InteractionStartRotation = FRotator::ZeroRotator;

	float MoveInteractionStartLerp = 0.0f;

	ELeonFallState FallState = ELeonFallState::FallEnd;
	ELeonFallAnimation FallAnimation = ELeonFallAnimation::HighHeight;
	ELeonJumpState JumpState = ELeonJumpState::End;

	ELeonKnifeAttackState KnifeAttackState = ELeonKnifeAttackState::EnterAttack;
	ELeonThrowingAnim ThrowingAnim = ELeonThrowingAnim::Top;

	FVector KickLocation = FVector::ZeroVector;

	//*****************************************************//


	//************** Input ���� **************//

	UPROPERTY(EditAnywhere, Category = Input)
	float TurnSpeed = 10.0f;
	UPROPERTY(EditAnywhere, Category = Input)
	float MaxTurnSpeed = 10.0f;

	// Todo : �����ڿ��� ã��

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

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* InventoryAction = nullptr;

	FVector MoveInput = FVector::ZeroVector;
	FVector LookInput = FVector::ZeroVector;

	uint32 bAbleComboInput : 1 = false;
	uint32 bAbleNextCombo : 1 = false;
	uint32 bIsComboEnd : 1 = false;
	
	ELeonDamageType DamageType = ELeonDamageType::Small;
	ELeonDamageDirection DamageDirection = ELeonDamageDirection::B;
	ELeonInteractionState InteractionState = ELeonInteractionState::Empty;

	//*****************************************************//

	//************** Camera ���� **************//

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(EditAnywhere)
	UCameraComponent* PlayerCamera = nullptr;

	//*****************************************************//


	//************** Actor, Component ���� **************//

	UBFsm* FsmComp = nullptr;
	ABLeonWeapon* CurrentWeapon = nullptr;

	EItemCode  UseWeaponCode = EItemCode::Empty;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FPlayerStat Stat;

	uint32 bDrawGrenadeAim : 1 = false;
	uint32 bIsThrowingWeapon : 1 = false;

	uint32 bIsKickAttackActive : 1 = false;

	FVector ThrowLocation = FVector::ZeroVector;
	FVector ThrowVelocity = FVector::ZeroVector;

	FVector JumpStart = FVector::ZeroVector;
	FVector JumpEnd = FVector::ZeroVector;
	FVector JumpDir = FVector::ZeroVector;

	FVector FallStart = FVector::ZeroVector;
	FVector FallEnd = FVector::ZeroVector;
	FVector FallDir = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = Input)
	ABCollisionObserverSphere* InteractionObserver = nullptr;

	UPROPERTY(EditAnywhere, Category = Stat)
	TSubclassOf<UDamageType> KickDamageType = nullptr;

	UPROPERTY(EditAnywhere, Category = Weapon)
	ABCollisionObserverCapsule* KickOverlapObserver = nullptr;

	UPROPERTY(EditAnywhere, Category = Weapon)
	ABDrawGrenadeAim* GrenadeAimActor = nullptr;

	UPROPERTY(EditAnywhere, Category = Weapon)
	USceneComponent* GrenadeThrowingLocation = nullptr;

	UPROPERTY(EditAnywhere, Category = Weapon)
	float GrenadeThrowingPower = 850.0f;

	AActor* InteractionActor = nullptr;
	IBInteraction* InteractionObject = nullptr;

	//************************UI***************************//

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<ABInventoryActor> InventoryClass;		// �κ��丮 ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = UI)
	FTransform InventoryTransform;						// �κ��丮 ���� Ʈ������


	//*****************************************************//

	void PlayMove(const FInputActionInstance& _MoveAction);
	void PlayIdle(const FInputActionInstance& _MoveAction);

	virtual void PlayLook(const FInputActionInstance& _LookAction);
	virtual void PlayLook(const FVector2D& _LookAction);

	void StopLook();

	void SpringArmUpdate(float _DeltaTime);
	void UseWeaponUpdate(float _DeltaTime);
	void WeaponSocketUpdate(float _DeltaTime);
	void SocketSwapUpdate(float _DeltaTime);
	void HealthStateUpdate(float _DeltaTime);
	void InteractionUpdate(float _DeltaTime);
	void GunRecoilUpdate(float _DeltaTime);

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
	void OpenInventory();

	void DrawGrenadeAim(float _DeltaTime);

	FVector GetGrenadeStartLocation(float _Angle) const;

	void CreateSprintArm();
	void CreateFSM();
	void CreateCollision();

	ABLeonWeapon* CreateWeapon(EItemCode _WeaponCode);
	void DeleteCurrentWeapon();

	ELeonWeaponAnim GetUseWeaponAnimation(EItemCode _WeaponCode) const;
	double GetAxisZAngle(const FVector& _Location) const;

	void KickAttack(AActor* _OverlapActor);

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

	void KickAttackEnter();
	void KickAttackUpdate(float _DeltaTime);
	void KickAttackExit();

	void KnifeAttackEnter();
	void KnifeAttackUpdate(float _DeltaTime);
	void KnifeAttackExit();

	void ThrowingEnter();
	void ThrowingUpdate(float _DeltaTime);
	void ThrowingExit();

	void DamageEnter();
	void DamageUpdate(float _DeltaTime);
	void DamageExit();

	void DeathEnter();
	void DeathUpdate(float _DeltaTime);
	void DeathExit();

	void JumpObstacleEnter();
	void JumpObstacleUpdate(float _DeltaTime);
	void JumpObstacleExit();

	void FallEnter();
	void FallUpdate(float _DeltaTime);
	void FallExit();
};