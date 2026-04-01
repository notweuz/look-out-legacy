// Copyright (c) 2025 Team Diff Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Input/Reply.h"
#include "BasePlayerWindowWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOOK_OUT_API UBasePlayerWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TitleTextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UNamedSlot* BodySlot;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Player Window")
	void OnCloseClicked();

	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	bool bIsDraggingWindow = false;
	FVector2D DragStartMousePosition = FVector2D::ZeroVector;
	FVector2D DragStartWindowTranslation = FVector2D::ZeroVector;
};
