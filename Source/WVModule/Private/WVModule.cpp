#include "WVModule/Public/WVModule.h"

DEFINE_LOG_CATEGORY(LogWVModule);

void FWVModule::StartupModule()
{
	UE_LOG(LogWVModule, Display, TEXT("===WV module has started!==="));
}
 
void FWVModule::ShutdownModule()
{
	UE_LOG(LogWVModule, Display, TEXT("===WV module has shut down==="));
}

IMPLEMENT_GAME_MODULE(FWVModule, WVModule)