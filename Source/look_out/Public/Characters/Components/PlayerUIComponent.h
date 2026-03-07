// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Components/ActorComponent.h"
#include "Widgets/BasePlayerWindowWidget.h"
#include "Widgets/Gameplay/BasePlayerUI.h"
#include "Widgets/Gameplay/Window/PlayerInventoryWindowWidget.h"
#include "PlayerUIComponent.generated.h"


class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOK_OUT_API UPlayerUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interface Classes")
	TSubclassOf<UBasePlayerUI> PlayerUIClass;
	
	UPROPERTY(BlueprintReadOnly, Category="Interface")
	UBasePlayerUI* PlayerUI;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interface Classes")
	TSubclassOf<UPlayerInventoryWindowWidget> InventoryWindowClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interface Classes")
	TSubclassOf<UBasePlayerWindowWidget> PlayerWindowClass;
	
	UFUNCTION(BlueprintCallable, Category = "Interface")
	void ToggleInventoryWindow(UInventoryComponent* AdditionalInventory);
	
	UPROPERTY(BlueprintReadWrite, Category="Interface")
	bool bIsInventoryWindowOpen = false;

private:
	UPROPERTY()
	ABaseCharacter* OwnerCharacter;
	
	UPROPERTY()
	UBasePlayerWindowWidget* SoloInventoryWindow;

	UPROPERTY()
	UBasePlayerWindowWidget* AdditionalInventoryWindow;
};
