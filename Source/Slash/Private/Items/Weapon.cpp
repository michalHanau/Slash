#include "Items/Weapon.h"
#include "Characters/SlashCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interface/HitInterface.h"

//בנאי
AWeapon::AWeapon()
{
	// יצירת הקופסא העוטפת
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(GetRootComponent());

	// הגדרת סוג ההתנגשות ל-Query Only (בדיקה האם פגע במשהו)
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// בתגובה תמיד תהייה Overlap (חפיפה)
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//  אל תתנגש בהשחקן עצמו כדי שהחרב לא תפגע בנו
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	//יצירת נקודות התחלה וסוף לחרב
	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//חיבור פונקציה המופעלת המגה לקופסא
	if (WeaponBox)
	{
		UE_LOG(LogTemp, Warning, TEXT("CHECKING WEAPON BOX..."));
		if (WeaponBox)
		{
			UE_LOG(LogTemp, Warning, TEXT("WEAPON BOX EXISTS!"));
		}
		WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WeaponBox)
	{
		// 1. קבלת המצב הנוכחי של הקוליז'ן
		ECollisionEnabled::Type CurrentCollision = WeaponBox->GetCollisionEnabled();

		// 2. המרה של המצב לטקסט קריא
		FString CollisionName = UEnum::GetValueAsName(CurrentCollision).ToString();

		// 3. הדפסה למסך (בצבע ורוד כדי שיהיה בולט)
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				1, // Key (1 מונע מההודעות להיערם אחת על השנייה)
				DeltaTime, // זמן תצוגה (מתאים ל-Tick)
				FColor::Red, // צבע
				FString::Printf(TEXT("Weapon Collision Mode: %s"), *CollisionName)
			);
		}
	}
}


void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	UE_LOG(LogTemp, Warning, TEXT("OVERLAP DETECTED WITH: %s"), *OtherActor->GetName());
	// קבלת המיקומים של נקודות ההתחלה והסוף
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	// יצירת רשימת שחקנים להתעלם מהם (כדי שהחרב לא תפגע בעצמה)
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	// משתנה שיכיל את תוצאת הפגיעה
	FHitResult BoxHit;

	// 4. הרצת ה-Box Trace
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(5.f, 5.f, 5.f),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		BoxHit,
		true
	);
	
	if (BoxHit.GetActor())
	{
		// IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
		// if (HitInterface)
		// {
		// 	HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint);
		// }
		// IgnoreActors.AddUnique(BoxHit.GetActor());
		CreateFields(BoxHit.ImpactPoint);
	}
}


void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// לוגיקה ששומרת את החרב אצל השחקן כשעומדים לידה
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingWeapon(this);
	}
}


void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// כשמתרחקים, השחקן כבר לא יכול להרים את החרב
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter)
	{
		SlashCharacter->SetOverlappingWeapon(nullptr);
	}
}


// המימוש של החיבור ליד!
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	GetMesh()->AttachToComponent(InParent, TransformRules, InSocketName);

	if (GetSphere()) // Sphere הוא הרכיב שיוצר את הטווח מסביב לחרב
	{
		GetSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (GetMesh()) //ביטול ההתנגשות כשחרב ביד
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	}

	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(InParent->GetOwner());
	if (SlashCharacter)
	{
		SlashCharacter->SetEquippedWeapon(this);
	}

	//שינוי מצב בנשק
	ItemState = EItemState::EIS_Equipped;
}


void AWeapon::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (WeaponBox)
	{
		WeaponBox->SetCollisionEnabled(CollisionEnabled);
	}
}
