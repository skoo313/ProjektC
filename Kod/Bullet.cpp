// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"


// Wartoœci poczatkow w konstruktorze
ABullet::ABullet()
{
	// Use a sphere as a simple collision representation.
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// zasieg kolizji.
	Collision->InitSphereRadius(15.0f);
	// Set the root component to be the collision component.
	RootComponent = Collision;

	// ustawienia pocisku.
	BulletMove = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	BulletMove->SetUpdatedComponent(Collision);
	BulletMove->InitialSpeed = 5000.0f;
	BulletMove->MaxSpeed = 5000.0f;
	BulletMove->bRotationFollowsVelocity = true;
	BulletMove->bShouldBounce = true;
	BulletMove->Bounciness = 0.3f;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// funkcja ,,wysylajaca" pocisk w okreslonym kierunku z V poczatkowa
void ABullet::FireDirection(const FVector& FireDirection)
{
	BulletMove->Velocity = FireDirection * BulletMove->InitialSpeed;
}