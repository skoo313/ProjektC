// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Tworzy "camera component".
		FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
		FPSCameraComponent->SetupAttachment(RootComponent);
		// Pozycja kamery: oczy (trochę nad ale to na razie może zostać).
		FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f+BaseEyeHeight));
		// Pozwala na poruszanie obrazem (jak tego nie ma to nie da sie patrzec góra-dół)
		FPSCameraComponent->bUsePawnControlRotation = true;

		// Tworzy mesha którego widzi gracz (tylko ramiona)
		FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
		FPSMesh->SetOnlyOwnerSee(true);
		// Attach the FPS mesh to the FPS camera.
		FPSMesh->SetupAttachment(FPSCameraComponent);
		// Wyłącza mesh ciała żeby widoczne były tylko rece
		FPSMesh->bCastDynamicShadow = false;
		FPSMesh->CastShadow = false;
		GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Welcome MyCharacter."));
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// CHODZENIE
	PlayerInputComponent->BindAxis("MoveUpDown", this, &AMyCharacter::MoveUpDown);
	PlayerInputComponent->BindAxis("MoveRightLeft", this, &AMyCharacter::MoveRightLeft);

	// Poruszanie kamerą
	PlayerInputComponent->BindAxis("Turn", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look", this, &AMyCharacter::AddControllerPitchInput);

	// SKAKANIE
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJump);

	// KUCANIE
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacter::StopCrouch);

	//STRZELANIE
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);
}

void AMyCharacter::MoveUpDown(float Value)
{
	// ,,znajduje" gdzie jest przód 
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::MoveRightLeft(float Value)
{
	// ,,znajduje" gdzie jest prawo
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMyCharacter::StartJump()
{	
	//wbudowana zmienna (boolean)
	bPressedJump = true;
}

void AMyCharacter::StopJump()
{
	//wbudowana zmienna (boolean)
	bPressedJump = false;
}

void AMyCharacter::StartCrouch()
{
	Crouch();
}

void AMyCharacter::StopCrouch()
{
	UnCrouch();
}

void AMyCharacter::Fire()
{
	// Attempt to fire a projectile.
	if (ProjectileClass)
	{
		// pobiera polozenia kamery
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// Ustawia kamere.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;
		
		UWorld* World = GetWorld();
		
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Spawnuje pocisk.
			ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile)
			{
				// Ustawia trajektoeie poczatkowa
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireDirection(LaunchDirection);
			}
		}
	}
}