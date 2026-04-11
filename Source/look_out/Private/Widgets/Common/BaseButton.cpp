#include "Widgets/Common/BaseButton.h"

#include "Core/Libraries/ThemeColors.h"


void UBaseButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	ActionButton->SetStyle(UThemeColors::DefaultButtonStyle());
}
