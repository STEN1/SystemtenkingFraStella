// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

static float CalcScale(int maxAge, int age)
{
    
    float scale = 1.f;
    scale += ((float)age / (float)maxAge) * 2.f;
    return scale;
}

// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();/*
    m_childPopultation.resize(simulationSteps);
    m_adultFertilePopulation.resize(simulationSteps);
    m_childPopultation[0] = 400;
    m_adultFertilePopulation[0] = 300;

    float birthRate = 0.052f;
    float childMortality = 0.16f;

    std::vector<Age> children(m_childPopultation[0]);
    for (auto& age : children)
        age = (Age)(rand() % 17);
    std::vector<Age> adultFertile(m_adultFertilePopulation[0]);
    for (auto& age : adultFertile)
        age = (Age)(rand() % 42 + 17);*/

    Children.SetNumUninitialized(400);
    AdultFertile.SetNumUninitialized(300);


    World = GetWorld();
    
    for (auto& Child : Children)
    {
        FVector spawnLocation = GetActorLocation();
        FRotator spawnRotation = GetActorRotation();
        spawnRotation.Yaw += 90.f;
        FVector spawnOffset = FVector::ZeroVector;
        spawnOffset.Y -= PopulationSpread;
        FVector random{ FMath::RandRange(-PopulationSpread, PopulationSpread), FMath::RandRange(-PopulationSpread, PopulationSpread), 0.f };
        spawnLocation = spawnLocation + spawnOffset + random;
        Child = World->SpawnActor<AHuman>(ChildBP, spawnLocation, spawnRotation);
        Child->Age = rand() % 17;
        Child->SetActorScale3D(FVector(CalcScale(17, Child->Age)));
    }
    for (auto& Adult : AdultFertile)
    {
        FVector spawnLocation = GetActorLocation();
        FRotator spawnRotation = GetActorRotation();
        spawnRotation.Yaw += 90.f;
        FVector spawnOffset = FVector::ZeroVector;
        spawnOffset.Y += PopulationSpread;
        FVector random{ FMath::RandRange(-PopulationSpread, PopulationSpread), FMath::RandRange(-PopulationSpread, PopulationSpread), 0.f };
        spawnLocation = spawnLocation + spawnOffset + random;
        Adult = World->SpawnActor<AHuman>(AdultBP, spawnLocation, spawnRotation);
        Adult->Age = rand() % 47 + 17;
        Adult->SetActorScale3D(FVector(CalcScale(47, Adult->Age)));
    }
    UpdateAdults(AdultFertile.Num());
    UpdateChildren(Children.Num());
}

void ASpawner::TickSimulation()
{
    // Aging
    for (auto Child : Children)
    {
        Child->Age += 1;
        Child->SetActorScale3D(FVector(CalcScale(17, Child->Age)));
    }
    for (auto Adult : AdultFertile)
    {
        Adult->Age += 1;
        Adult->SetActorScale3D(FVector(CalcScale(47, Adult->Age)));
    }

    // push children that are born to the population
    int32 childrenToBirth = (float)AdultFertile.Num() * BirthRate;
    for (int32 i = 0; i < childrenToBirth; i++)
    {
        FVector spawnLocation = GetActorLocation();
        FRotator spawnRotation = GetActorRotation();
        spawnRotation.Yaw += 90.f;
        FVector spawnOffset = FVector::ZeroVector;
        spawnOffset.Y -= PopulationSpread;
        FVector random{ FMath::RandRange(-PopulationSpread, PopulationSpread), FMath::RandRange(-PopulationSpread, PopulationSpread), 0.f };
        spawnLocation = spawnLocation + spawnOffset + random;
        Children.Emplace(World->SpawnActor<AHuman>(ChildBP, spawnLocation, spawnRotation));
    }
    // kill random children based on mortality rate
    int32 childrenToKill = (float)Children.Num() * ChildMortality / 17.f;
    for (int32 i = 0; i < childrenToKill; i++)
    {
        int32 childToKill = rand() % Children.Num();
        Children[childToKill]->Destroy();
        Children.RemoveAt(childToKill);
    }
    // Move children that are to old to the adult fertile population
    for (auto child : Children)
    {
        if (child->Age > 17.f)
        {
            Children.Remove(child);
            child->Destroy();

            FVector spawnLocation = GetActorLocation();
            FRotator spawnRotation = GetActorRotation();
            spawnRotation.Yaw += 90.f;
            FVector spawnOffset = FVector::ZeroVector;
            spawnOffset.Y += PopulationSpread;
            FVector random{ FMath::RandRange(-PopulationSpread, PopulationSpread), FMath::RandRange(-PopulationSpread, PopulationSpread), 0.f };
            spawnLocation = spawnLocation + spawnOffset + random;
            AdultFertile.Emplace(World->SpawnActor<AHuman>(AdultBP, spawnLocation, spawnRotation));
        }
    }
    // Kill old people
    for (auto adult : AdultFertile)
    {
        if (adult->Age > 47.f)
        {
            AdultFertile.Remove(adult);
            adult->Destroy();
        }
    }
    // Update UI
    UpdateAdults(AdultFertile.Num());
    UpdateChildren(Children.Num());
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    Timer += DeltaTime;

    if (Timer > 1.f)
    {
        Timer -= 1.f;
        TickSimulation();
    }
}

