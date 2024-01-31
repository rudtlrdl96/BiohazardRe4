// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Item/ItemData.h"
#include "BInterface_WeaponPutOut.h"
#include "BInterface_WeaponPutAway.h"
#include "BLeon.generated.h"

struct FInputActionInstance;
class UBFsm;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class ABLeonWeapon;

#define TO_KEY(EnumValue) static_cast<int32>(EnumValue)

UENUM(BlueprintType)
enum class ELeonState : uint8
{
	Idle			UMETA(DisplayName = "Idle"),
	Walk			UMETA(DisplayName = "Walk"),
	Jog				UMETA(DisplayName = "Jog"),
	Aim				UMETA(DisplayName = "Aim"),
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

UCLASS()
class BIOHAZARDRE4_API ABLeon : public ACharacter, public IBInterface_WeaponPutAway, public IBInterface_WeaponPutOut
{
	GENERATED_BODY()

private:
	static const FVector StandSocketOffset;
	static const FVector CrouchSocketOffset;

public:
	// Sets default values for this character's properties
	ABLeon();

	// Called every frame
	virtual void Tick(float _DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* _PlayerInputComponent) override;

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


	UFUNCTION(BlueprintCallable)
	inline bool HasWeapon() const
	{
		return nullptr != CurrentWeapon;
	}

	UFUNCTION(BlueprintCallable)
	FVector GetWeaponLeftSocketLocation() const;


	virtual void WeaponPutOutStart() override;
	virtual void WeaponPutOutEnd() override;
	virtual void WeaponPutAwayStart() override;
	virtual void WeaponPutAwayEnd() override;

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

	USpringArmComponent* SpringArm = nullptr;
	UCameraComponent* PlayerCamera = nullptr;

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

	uint32 bIsLerpWeaponChange : 1 = false;

	void PlayMove(const FInputActionInstance& _MoveAction);
	void PlayIdle(const FInputActionInstance& _MoveAction);

	virtual void PlayLook(const FInputActionInstance& _LookAction);
	virtual void PlayLook(const FVector2D& _LookAction);

	void StopLook();

	void SpringArmUpdate(float _DeltaTime);
	void UseWeaponUpdate(float _DeltaTime);
	void WeaponSocketUpdate(float _DeltaTime);

	void VPlayerCameraToWorld(FVector& _Result) const;

	void JogLookAt(float _DeltaTime);
	void Aim(float _DeltaTime);

	void ActiveJog();
	void DisableJog();
	void ActiveAim();
	void DisableAim();
	void TryCrouch();
	void TryInteraction();
	bool AbleAim() const;

	void CreateSprintArm();
	void CreateFSM();

	void UseQuickSlot(const uint32 _Index);

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
};