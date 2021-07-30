// Fill out your copyright notice in the Description page of Project Settings.

#include "TC_BaseWeapon.h"
#include "Components/SceneComponent.h"
#include "Engine.h"

// Sets default values
ATC_BaseWeapon::ATC_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	this->Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = this->Root;

	this->WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	this->WeaponMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

ATC_BaseWeapon* ATC_BaseWeapon::MakeWeapon()
{
	ATC_BaseWeapon* Weapon = NewObject<ATC_BaseWeapon>();
	return Weapon;
}

int32 ATC_BaseWeapon::RandInt()
{
	unsigned short lfsr = 0xACE1u;
	unsigned bit;
	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
	return lfsr = (lfsr >> 1) | (bit << 15);
}

void ATC_BaseWeapon::Fire()
{
	switch (this->ProjectileType)
	{
	case EWeaponProjectile::EBullet:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Bullet"));
		this->Instant_Fire();
	case EWeaponProjectile::ESpread:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Spread"));
		for (int32 i = 0; i <= this->WeaponConfig.WeaponSpread; i++)
		{
			this->Instant_Fire();
		}
	case EWeaponProjectile::EProjectile:
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, TEXT("Projectile"));

	default:
		break;
	}
}

void ATC_BaseWeapon::Instant_Fire()
{
	const int32 RandomSeed = this->RandInt();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = this->WeaponConfig.WeaponSpread;
	const float SpreadCone = FMath::DegreesToRadians(this->WeaponConfig.WeaponSpread * 0.5f);
	const FVector AimDir = this->WeaponMesh->GetSocketRotation("MF").Vector();
	const FVector StartTrace = this->WeaponMesh->GetSocketLocation("MF");
	const FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	const FVector EndTrace = StartTrace + ShootDir * this->WeaponConfig.WeaponRange;
	const FHitResult Impact = this->WeaponTrace(StartTrace, EndTrace);

	this->ProcessInstantHit(Impact, StartTrace, ShootDir, RandomSeed, CurrentSpread);
}

FHitResult ATC_BaseWeapon::WeaponTrace(const FVector& TraceForm, const FVector& TraceTo) const
{
	static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
	
	FCollisionQueryParams TraceParams(WeaponFireTag, true, GetInstigator());
	//TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	this->GetWorld()->LineTraceSingleByChannel(Hit, TraceForm, TraceTo, TRACE_WEAPON, TraceParams);

	return Hit;
}

void ATC_BaseWeapon::ProcessInstantHit(const FHitResult& Impact, const FVector& Origin, const FVector& ShootDir, int32 RandomSeed, float ReticleSpread)
{
	const FVector EndTrace = Origin + ShootDir * this->WeaponConfig.WeaponRange;
	const FVector EndPoint = Impact.GetActor() ? Impact.ImpactPoint : EndTrace;
	DrawDebugLine(this->GetWorld(), Origin, Impact.TraceEnd, FColor::Red, true, 10000, 10.0f);
}