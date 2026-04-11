#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BaseButton.generated.h"

UCLASS()
class LOOK_OUT_API UBaseButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Button")
	UButton* ActionButton;

	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category="Button")
	UTextBlock* ButtonText;
	
protected:
	virtual void NativeConstruct() override;
};