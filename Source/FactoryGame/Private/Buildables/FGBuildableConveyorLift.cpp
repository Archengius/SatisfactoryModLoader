// This file has been automatically generated by the Unreal Header Implementation tool

#include "Buildables/FGBuildableConveyorLift.h"
#include "Components/SceneComponent.h"
#include "FGConveyorInstanceSplineMesh.h"

#if WITH_EDITORONLY_DATA
#endif 
#if WITH_EDITOR
void AFGBuildableConveyorLift::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent){ }
#endif 
AFGBuildableConveyorLift::AFGBuildableConveyorLift() : Super() {
	this->mConveyorLiftSparesDataCDO = nullptr;
	this->mMeshHeight = 200.0;
	this->mBottomMesh = nullptr;
	this->mMidMesh = nullptr;
	this->mHalfMidMesh = nullptr;
	this->mTopMesh = nullptr;
	this->mBellowMesh = nullptr;
	this->mJointMesh = nullptr;
	this->mShelfMesh = nullptr;
	this->mTopTransform = FTransform(FQuat::Identity, FVector::ZeroVector, FVector::OneVector);
	this->mIsReversed = false;
	this->mVisibilityComponent = CreateDefaultSubobject<UFGConveyorLiftVisibilityMesh>(TEXT("VisibilityMesh"));
	this->mVisibilityComponent->SetupAttachment(RootComponent);
}
void AFGBuildableConveyorLift::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFGBuildableConveyorLift, mTopTransform);
	DOREPLIFETIME(AFGBuildableConveyorLift, mIsReversed);
	DOREPLIFETIME(AFGBuildableConveyorLift, mSnappedPassthroughs);
}
void AFGBuildableConveyorLift::BeginPlay(){ }
int32 AFGBuildableConveyorLift::GetDismantleRefundReturnsMultiplier() const{ return int32(); }
void AFGBuildableConveyorLift::Upgrade_Implementation(AActor* newActor){ }
void AFGBuildableConveyorLift::Dismantle_Implementation(){ }
TArray<struct FInstanceData> AFGBuildableConveyorLift::GetActorLightweightInstanceData_Implementation(){ return TArray<struct FInstanceData>(); }
float AFGBuildableConveyorLift::GetLastRenderTime() const{ return float(); }
void AFGBuildableConveyorLift::DestroyVisualItems(){ }
void AFGBuildableConveyorLift::TickItemTransforms(float dt){ }
void AFGBuildableConveyorLift::TickRadioactivity(){ }
void AFGBuildableConveyorLift::Factory_UpdateRadioactivity( AFGRadioactivitySubsystem* subsystem){ }
FBoxCenterAndExtent AFGBuildableConveyorLift::FitClearance(float transformZ,
		float stepHeight,
		const FVector2D& extend2D,
		const FVector& extentBias){ return FBoxCenterAndExtent(); }
void AFGBuildableConveyorLift::OnRep_TopTransform(){ }
const FVector2D AFGBuildableConveyorLift::CLEARANCE_EXTENT_2D = FVector2D();
