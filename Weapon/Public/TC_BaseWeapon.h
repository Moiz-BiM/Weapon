// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define TRACE_WEAPON ECC_GameTraceChannel1

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TC_BaseWeapon.generated.h"

UENUM(BlueprintType, Category = "TC")
namespace EWeaponProjectile
{
	enum ProjectileType
	{
		EBullet			UMETA(DisplayName = "Bullet"),
		ESpread			UMETA(DisplayName = "Spread"),
		EProjectile		UMETA(DisplayName = "Projectile"),
	};
}

UENUM(BlueprintType, Category = "TC")
namespace EWeaponType
{
	enum Weapon
	{
		EHandgun_silenced				UMETA(DisplayName = "Handgun_silenced"),
		EHandgun_nonSilenced			UMETA(DisplayName = "Handgun_nonSilenced"),
		ERifle_silenced					UMETA(DisplayName = "Rifle_silenced"),
		ERifle_nonSilenced				UMETA(DisplayName = "Rifle_nonSilenced"),
		EShotgun						UMETA(DisplayName = "Shotgun"),
		EGranade						UMETA(DisplayName = "Granade"),
	};
}

USTRUCT(BlueprintType, Category = "TC")
struct WEAPON_API FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "TC|Ammo")
		int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "TC|Config")
		float TimeBetweenShots;

	UPROPERTY(EditDefaultsOnly, Category = "TC|Ammo")
		int32 ShotCost;

	UPROPERTY(EditDefaultsOnly, Category = "TC|Config")
		float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = "TC|Config")
		float WeaponSpread;
};

UCLASS(BlueprintType, Category = "TC")
class WEAPON_API ATC_BaseWeapon : public AActor
{
	GENERATED_BODY()

private:
	// Functions
	UFUNCTION()
		void Fire();
	UFUNCTION()
		void Instant_Fire();
	UFUNCTION()
		int32 RandInt();
	// Variables
	UPROPERTY(EditDefaultsOnly, Category = "TC|Config")
		FWeaponData WeaponConfig;
public:	
	// Sets default values for this actor's properties
	ATC_BaseWeapon();
	// Functions
	UFUNCTION(BlueprintCallable, Category = "TC")
		ATC_BaseWeapon* MakeWeapon();
	// Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TC|Config")
		TEnumAsByte<EWeaponProjectile::ProjectileType> ProjectileType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TC|Config")
		TEnumAsByte<EWeaponType::Weapon> WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TC|Collision")
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TC|Collision")
		class USkeletalMeshComponent* WeaponMesh;
protected:
	// functions
	FHitResult WeaponTrace(const FVector &TraceForm, const FVector &TraceTo) const;

	void ProcessInstantHit(const FHitResult &Impact, const FVector &Origin, const FVector &ShootDir, int32 RandomSeed, float ReticleSpread);
};