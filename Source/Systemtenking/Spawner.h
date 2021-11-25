// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Human.h"
#include "Spawner.generated.h"


UCLASS()
class SYSTEMTENKING_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UWorld* World;
	TArray<AHuman*> Children;
	TArray<AHuman*> AdultFertile;

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateChildren(int32 childrens);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAdults(int32 adults);

	float BirthRate = 0.052f;
	float ChildMortality = 0.16f;

	float PopulationSpread = 3000.f;

	void TickSimulation();
	float Timer{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Humans", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHuman> ChildBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Humans", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AHuman> AdultBP;
};
