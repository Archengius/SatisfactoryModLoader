// Copyright Coffee Stain Studios. All Rights Reserved.

#pragma once
#include "FactoryGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FGCharacterMovementComponent.generated.h"

USTRUCT( BlueprintType )
struct FACTORYGAME_API FPlayerPipeHyperData
{
	GENERATED_BODY()
	/** The pipe we are currently traveling in */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = "Hyper Tube" )
	AActor* mTravelingPipeHyper;
	UPROPERTY()
	AActor* mTravelingPipeHyperLast;
	float mTravelingPipeHyperLastLength; //used for faster position calc if needed. If negative, it means we moved ot through it's start
	float mLastTransitTime = 0.0f;

	/**The spline progress in the pipe we are currently in*/
	float mPipeProgressReal;
	/**The velocity we are currently traveling in along the spline of the pipe*/
	float mPipeVelocityReal;
	float mPipeCurrentLength;

	/**The spline progress in the pipe we are currently in*/
	float mPipeProgressLast;
	/**The velocity we are currently traveling in along the spline of the pipe*/
	float mPipeVelocityLast;

	float mTimeInterpreter;

	/**The spline progress in the pipe we are currently in */
	float mPipeProgress;

	/**The velocity we are currently traveling in along the spline of the pipe*/
	float mPipeVelocity;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Hyper Tube" )
	float mMinPipeSpeed = 300;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Hyper Tube" )
	float mPipeGravityStrength = 1500.0f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Hyper Tube" )
	float mPipeFriction = 0.05f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Hyper Tube" )
	float mPipeConstantAcceleration = 110.0f;

	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Hyper Tube" )
	float mPipeCurveDamping = 0.4f;;

	/**World space direction of pipe last frame.*/
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	FVector mPipeDirectionLast;

	/**World space direction of pipe in current location.*/
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	FVector mPipeDirection;

	/**Distance to the end of the whole pipe chain, but maximum of 2000 (To save time on calculating it on update)*/
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	float mDistanceToEndOfPipe;
	
	/** maximum of 2000 (To save time on calculating it on update)*/
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	float mCombinedLengthTillEndOfPipesIncludingCurrent;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	float mMaxCurveDiffThisFrame;

	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	float mMaxCurveDiffSmooth;


	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	float mPipeTempMinSpeed; //used so we can reduce even further due to curves, but not have the pipes too slow in upwards slopes


	/**World space position of the end of the pipe chain.. Only guaranteed to be accurate if the end is within 2000 units.*/
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	FVector mFulPipeEndPoint;
	/**World space direction of the end of the pipe chain.. Only guaranteed to be accurate if the end is within 2000 units.*/
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Transient, Category = "Hyper Tube" )
	FVector mFulPipeEndDir;

	FVector mSoftVelocity;
	FVector mCameraPush;
	float mCamFovMod;
};

USTRUCT( BlueprintType )
struct FACTORYGAME_API FPlayerZiplineData
{
	GENERATED_BODY()

	/** Multiplier that controls speed based on angle of travel */
	UPROPERTY( BlueprintReadOnly, Category = "Zipline Data" )
	float SpeedMultiplier;

	/* World direction we're headed */
	UPROPERTY( BlueprintReadOnly, Category = "Zipline Data" )
	FVector Direction;
	
	FVector Point1;
	FVector Point2;
	
	FVector EndPoint;

	/* Velocity applied last frame from zipline movement */
	FVector LastVelocityApplied;
	
	FVector LastCorrectionVelocityApplied;
	
	FVector ProjectedZiplineLocation;
	
	UPROPERTY()
	AActor* AttachActor;
};

/**
 * Used for deferred collision state changes. Needed to make sure overlap updates don't happen mid logic steps and can cause recursive triggering of event calls.
 //[DavalliusA:Thu/27-02-2020] added when working on hyper tube bugs, as leaving one hyper tube and turning on collision in the movement mode change function could cause over movement mode changed functions to trigger.

 */
UENUM( BlueprintType )
enum class EDeferredCollisionChange : uint8
{
	DCC_None = 0		UMETA( DisplayName = "Do nothing" ),
	DCC_TURN_ON		UMETA( DisplayName = "Turn On Collision" ),
	DCC_TURN_OFF	UMETA( DisplayName = "Turn Off Collision" ),
};

/**
 * Custom states we can be in
 */
UENUM( BlueprintType )
enum class ECustomMovementMode : uint8
{
	CMM_None = 0			UMETA( DisplayName = "None" ),
	CMM_Ladder				UMETA( DisplayName = "Ladder" ),
	CMM_PipeHyper			UMETA( DisplayName = "Hyper Pipe" ),
	CMM_Zipline				UMETA( DisplayName = "Zipline" ),

	// Three hover modes to better represent the "current" hover state with the SavedMove system
	CMM_Hover				UMETA( DisplayName = "Hover" ),
	CMM_HoverSlowFall		UMETA( DisplayName = "Hover Slow Fall" )
};
inline bool operator==(const uint8 a, const ECustomMovementMode b)
{
	return a == static_cast<uint8>( b );
}

inline bool operator==(const ECustomMovementMode b , const uint8 a)
{
	return a == static_cast<uint8>( b );
}

UCLASS()
class FACTORYGAME_API UFGCharacterMovementComponent final : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UFGCharacterMovementComponent();
	
	// Begin UActorComponent
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction ) override;
	// End UActorComponent

	// Begin UCharacterMovementComponent
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	virtual bool DoJump( bool isReplayingMoves ) override;
	virtual void CalcVelocity( float dt, float friction, bool isFluid, float brakingDeceleration ) override;
	virtual float GetMaxSpeed() const override;
	virtual void StartNewPhysics( float deltaTime, int32 Iterations ) override;
	virtual void SetDefaultMovementMode() override;
	virtual float GetMaxJumpZVelocity() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual void SmoothClientPosition( float DeltaSeconds ) override;
	virtual float ImmersionDepth() const override;
	// End UCharacterMovementComponent

	UFUNCTION( Reliable, Server, WithValidation )
	void ServerSetHookLocation( const FVector& hookLocation );

	/** Sets the location of the current hook */
	void SetHookLocation( const FVector& hookLocation);

	/** Get hook location */
	FORCEINLINE const FVector& GetHookLocation() const { return mHookLocation; }

	/** Get mIsSprinting */
	UFUNCTION( BlueprintPure, Category = "Sprint" )
	FORCEINLINE bool GetIsSprinting() const { return mIsSprinting; }

	/** Get mGetWantsToSprint */
	UFUNCTION( BlueprintPure, Category = "Sprint" )
	FORCEINLINE bool GetWantsToSprint() const { return mWantsToSprint; }

	/** Toggles mWantsToSprint and sets mHoldToSprint to isHoldSprintOn. */
	void ToggleWantsToSprintAndHoldSprint( const bool isHoldSprintOn );
	
	/** Get mBaseVelocity */
	UFUNCTION( BlueprintPure, Category = "Movement" )
	FORCEINLINE FVector GetBaseVelocity() const { return mBaseVelocity; }

	/** Set if the player wants to sprint or not */
	void SetWantsToSprintAndHoldSprint( const bool wantsToSprint, const bool isHoldSprintOn );

	/** Get mIsSliding */
	FORCEINLINE bool IsSliding() const { return mIsSliding; }

	/** Gets the accumulated impulse we want to apply */
	FORCEINLINE FVector GetPendingImpulseToApply() const { return PendingImpulseToApply; }

	/** Gets last slide time */
	FORCEINLINE float GetLastSlideTime() const{ return mLastSlideTime; }

	/** Gets last slide time */
	FORCEINLINE float GetBoostJumpTimeWindow() const { return mBoostJumpTimeWindow; }

	/** Setter for the chute */
	FORCEINLINE void SetCachedParachute( class AFGParachute* inParachute ) { mCachedParachute = inParachute; } 

	/** Setter for the chute */
	FORCEINLINE void SetCachedJumpingStilts( class AFGJumpingStilts* inJumpingStilts ) { mCachedJumpingStilts = inJumpingStilts; }

	//////////////////Ladder functions//////////////////
	bool CanGrabLadder( const class UFGLadderComponent* ladder ) const;
	void StartClimbLadder( class UFGLadderComponent* ladder );
	void StopClimbLadder();
	bool IsClimbingOnLadder() const { return CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMM_Ladder); }
	void JumpOffLadder();

	/** @return The ladder we're climbing on; nullptr if not climbing. */
	UFUNCTION( BlueprintPure, Category = "Ladder" )
	class UFGLadderComponent* GetOnLadder() const;


	//////////////////Hyper Pipe Functions///////////////////////
	
	/** IsInHyperPipe
	 * @return	bool - returns true if we are currently moving in a hyper tube
	 */
	UFUNCTION( BlueprintPure, Category = "Hyper Tube" )
	 bool IsInHyperPipe() const { return CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMM_PipeHyper); }

	/** IsOnZipline
	* @return	bool - returns true if we are currently moving on a zipline
	*/
	UFUNCTION( BlueprintPure, Category = "Zipline" )
    bool IsOnZipline() const { return CustomMovementMode == static_cast<uint8>(ECustomMovementMode::CMM_Zipline); }

	/** EnterHyperPipe
	 * Enter a pipe and change movement mode to custom pipe movement. You can not enter pipes directly. You always need a part as an entrance. That is how it's designed atm. Only start parts as well. But we'll see how that changes.
	 * @param	pipe - the pipe start we want to enter
	 * @return	bool - returns true if we could enter.
	 */
	UFUNCTION( BlueprintCallable, Category = "Hyper Tube" )
	bool EnterPipeHyper( class AFGBuildablePipeHyperPart* pipe );

	UFUNCTION( BlueprintPure, Category = "Hyper Tube" )
	FVector GetPipeTravelDirectionWorld() const;

	UFUNCTION( BlueprintPure, Category = "Hyper Tube" )
	FRotator GetPipeCharacterTransform(FVector cameraForwardAxis) const;

	void PipeHyperForceExit();
	/** updates if the player wants to slide*/
	bool WantsToSlide() const;

	/** Get mBoostJumpVelocityMultiplier */
	FORCEINLINE float GetBoostJumpVelocityMultiplier() const { return mBoostJumpVelocityMultiplier; }

	/** Checks if we still can slide */
	void UpdateSlideStatus();

	// Use this to attempt to climb over an obstacle in front of the player
	bool AttemptLedgeClimb();

	UFUNCTION()
	void OnLedgeClimbFinished();

	/** Update loop for if we are zip lining */
	void UpdateZiplineEffects() const;

	inline void SetPipeTempMinSpeed( const float speed )
	{
		mPipeData.mPipeTempMinSpeed = speed;
	}
	bool SetTravelingPipeHyperActor( AActor* hyperPipeInterfaceActor );
	inline void SetPipeProgress( const float distance )
	{
		mPipeData.mPipeProgress = distance;
	}
	inline void SetPipeVelocity( const float speed )
	{
		mPipeData.mPipeVelocity = speed;
	}
	inline void SetGeneralVelocity( const FVector& velocity )
	{
		Velocity = velocity;
	}
	inline void SetPipeDirectionLast( const FVector& direction )
	{
		mPipeData.mPipeDirectionLast = direction;
	}
	UFUNCTION( BlueprintPure, Category = "Hyper Tube" )
	inline float GetPipeVelocity() const{ return mPipeData.mPipeVelocity; }
	UFUNCTION( BlueprintPure, Category = "Hyper Tube" )
	inline float GetPipeMinSpeed() const{ return mPipeData.mMinPipeSpeed; }
	inline float GetPipeTempMinSpeed() const{ return mPipeData.mPipeTempMinSpeed; }
	inline float GetPipeProgress() const{ return mPipeData.mPipeProgress; }
	inline float GetPipeConstantAcceleration() const{ return mPipeData.mPipeConstantAcceleration; }
	inline float GetPipeGravityStrength() const{ return mPipeData.mPipeGravityStrength; }
	inline float GetPipeCurveDamping() const{ return mPipeData.mPipeCurveDamping; }
	inline float GetPipeFriction() const{ return mPipeData.mPipeFriction; }
 	inline FVector GetVelocity() const{ return Velocity; }
	inline FVector GetPipeDirectionLast() const{ return mPipeData.mPipeDirectionLast; }
	inline AActor* GetTravelingPipeHyperActor() const{ return mPipeData.mTravelingPipeHyper; }
	UFUNCTION( BlueprintPure, Category = "Hyper Tube" )
	inline FPlayerPipeHyperData& GetPipeHyperDataRef() { return mPipeData; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Hyper Tube" )
	void OnNewTravelPipeSection();

	const USceneComponent* GetUpdateComponent() const;

	void SetZiplineData( const FPlayerZiplineData &inData ) { mZiplineData = inData; }

	UFUNCTION( BlueprintPure, Category = "Zipline" )
	FORCEINLINE FPlayerZiplineData& GetZiplineDataRef() { return mZiplineData; }
	
	UFUNCTION( BlueprintPure, Category = "Zipline" )
	const FVector& GetZipLineDirection() const { return mZiplineData.Direction; }
	
	UFUNCTION( BlueprintPure, Category = "Zipline" )
	const float& GetZipLineSpeedMultiplier() const { return mZiplineData.SpeedMultiplier; }
	
	bool IsZiplineAttachActorValid() const { return IsValid( mZiplineData.AttachActor ); }
	
	UFUNCTION( BlueprintPure, Category = "Zipline" )
	float GetZiplineSpeed() const;

	void StopZiplineMovement( FVector exitForce = FVector::ZeroVector );

	void StartZiplineMovement( AActor* ziplineActor, FVector actorForward );
	
protected:
	// Begin UCharacterMovementComponent
	virtual void UpdateFromCompressedFlags(uint8 flags) override;
	virtual void OnMovementUpdated(float deltaSeconds, const FVector & oldLocation, const FVector & oldVelocity) override;
	virtual void OnMovementModeChanged( EMovementMode PreviousMovementMode, uint8 PreviousCustomMode ) override;
	virtual void PhysCustom( float deltaTime, int32 Iterations ) override;

	void ExecuteDeferredCollisionChange();
	// End UCharacterMovementComponent

	void SetOnLadder( class UFGLadderComponent* ladder );
	
	virtual void PhysFlying( float deltaTime, int32 iterations ) override;
	
private:
	/** Apply ladder climb physics */
	void PhysLadder( const float deltaTime, int32 iterations );

	/** Apply Hyper Tube physics */
	void PhysPipe( const float deltaTime );

	/** Apply Zipline physics */
	void PhysZipline( const float deltaTime );

	/** Apply Hover physics */
	void PhysHover( const float deltaTime );

	/** Updates everything that has to do with JetPack */
	void UpdateJetPack( float deltaSeconds );

	/** Updates everything that has to do with Hookshot */
	void UpdateHookshot( float deltaSeconds, FVector oldLocation );

	/** Checks if we still can sprint */
	void UpdateSprintStatus();

	/** Updates everything that has to do with Parachute */
	void UpdateParachute( float delta );

	/** Returns true if the player is allowed to sprint */
	bool CanSprint() const;

	/** Returns true if the player can slide with current values */
	bool CanSlide() const;

	/** Returns true if the player is allowed to start a slide */
	bool CanStartSlide() const;

	/** Returns or finds the jet pack */
	class AFGJetPack* GetCachedJetPack();

	/** Returns or finds the cached Hookshot */
	class AFGHookshot* GetCachedHookshot();

	/** Returns or finds the parachute */
	class AFGParachute* GetCachedParachute();

	/** Returns or finds the jumping stilts */
	class AFGJumpingStilts* GetCachedJumpingStilts();

	/** Returns or finds the Hoverpack */
	class AFGHoverPack* GetCachedHoverPack();

	/** Ticks the slide timer so we know for how long the slide has been ongoing */
	void TickSlide( float delta );

	/** Used by AttemptLedgeClimb in order to move the character up over a duration. */
	bool StartLedgeClimb( float duration, float speed );

	/** Used to stop ledge climbing. */
	void StopLedgeClimb( const bool interrupt );
	
public:
	/** Timestamp of last time we jumped */
	UPROPERTY( BlueprintReadOnly, Category = "Jump" )
	float mLastJumpTimeStamp;

	/** If true, try to parachuting on next update. If false, try to stop parachuting on next update. */
	UPROPERTY( BlueprintReadOnly, Category = "JetPack" )
	uint32 mIsParachuting : 1;

	/** The speed at which a character climbs ladders */
	UPROPERTY( EditDefaultsOnly, Category = "Sprint" )
	float mClimbSpeed;

	/** The speed at witch a character sprints */
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint" )//ReadWrite is only to enable blueprint prototyping of shit.
	float mMaxSprintSpeed;

	/** True if the player wants to sprint. */
	bool mWantsToSprint;

	/** True if the player wants to sprint while on Zipline. */
	bool mWantsToSprintOnZipline;
	
	/** Keeps is the player sprinting this update or not? */
	bool mIsSliding;

	/** Keep track of what status was for mIsSliding */
	bool mLastIsSliding; 

	/** True if the player is pressing jump */
	bool mIsPressingJump;

	/** The minimum dot value between velocity and character forward to allow sprint.*/
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Sprint" )
	float mSprintMinDotResult;

	/** Force to apply when jumping off a ladder. */
	UPROPERTY( EditDefaultsOnly, Category = "Ladder" )
	float mJumpOffLadderVelocity;

	/** Cached Reference to the Owning FGPlayerCharacter */
	UPROPERTY()
	class AFGCharacterPlayer* mFGCharacterOwner;

private:
	friend class FSavedMove_FGMovement;

	/** Keeps is the player sprinting this update or not? */
	bool mIsSprinting;

	/** Holds if sprint toggle is on from FGCharacterPlayer. Updates every time SetWantsToSprint() is called.*/
	bool mHoldToSprint;
	
	/** A cached instance of the equipment that issued jet pack thrust */
	UPROPERTY()
	class AFGJetPack* mCachedJetPack;

	/** A cached instance of the equipment that issued parachuting */
	UPROPERTY()
	class AFGParachute* mCachedParachute;

	/** A cached instance of the equipment that set our hookshot location */
	UPROPERTY()
	class AFGHookshot* mCachedHookshot;

	/** A cached instance of the equipment that set our jumping stilts */
	UPROPERTY()
	class AFGJumpingStilts* mCachedJumpingStilts;

	/** A cached instance of the equipment that set our hover pack */
	UPROPERTY()
	class AFGHoverPack* mCachedHoverPack;

	/** A cached instance of rail we're surfing with the hoverpack */
	UPROPERTY()
	class AFGBuildableRailroadTrack* mCachedSurfedRailroadTrack;

	/** The location that our hook sits at */
	FVector mHookLocation;

	/** The ladder we're climbing; null if not climbing. */
	UPROPERTY()
	UFGLadderComponent* mOnLadder;

	UPROPERTY(EditAnywhere, meta = ( ShowOnlyInnerProperties ) )
	FPlayerPipeHyperData mPipeData;

	/** Get velocity from curve when sliding */
	UPROPERTY( EditDefaultsOnly, Category = "Movement" )
	UCurveFloat* mSlideCurve;

	/** Gets the multiplier for slope velocity */
	UPROPERTY( EditDefaultsOnly, Category = "Movement" )
	UCurveFloat* mSlopeCurve;

	/** How long have we been sliding */
	float mSlideTime;

	/** Max angle ( in radians ) for allowing to slide */
	UPROPERTY( EditDefaultsOnly, Category = "Movement" )
	float mMaxSlideAngle;

	/** Velocity added from conveyor belts or other sources */
	FVector mAddedVelocity;

	/** Velocity for the actor without external influence like conveyor belts */
	UPROPERTY()
	FVector mBaseVelocity;

	/* Multiplier for boost jump for Z velocity */
	UPROPERTY( EditDefaultsOnly, Category = "Movement" )
	float mBoostJumpZMultiplier;

	/* Multiplier for velocity in 2D when boost jumping */
	UPROPERTY( EditDefaultsOnly, Category = "Movement" )
	float mBoostJumpVelocityMultiplier;

	/** Timestamp for when we ended the last slide */
	float mLastSlideTime;

	/* How long time after a slide a jump can be input and be counted as a boost jump*/
	UPROPERTY( EditDefaultsOnly, Category = "Movement" )
	float mBoostJumpTimeWindow;

	// TODO: PROTOTYPE
	/** Whether or not to enabled the vault / ledge climb prototype. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Vault" )
	bool mEnableVaultPrototype;
	
	/** How far forward we should sweep to check if vaulting is possible. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Vault" )
	float mVaultSweepDistance;

	/** How high something can be for us to vault over it. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Vault" )
	float mVaultHeightThreshold;

	/** How much forward velocity to add when vaulting. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Vault" )
	float mVaultForwardVelocity;

	/** How much vertical velocity to add when vaulting. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Vault" )
	float mVaultVerticalVelocity;

	/** How high something can be for us to ledge climb over it, should be higher than vault height limit. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Vault" )
	float mLedgeClimbHeightThreshold;

	FTimerHandle mLedgeClimbTimerHandle;

	float mCurrentLedgeClimbingSpeed;
	
	bool mIsLedgeClimbing;
	bool mIsVaulting;

	EDeferredCollisionChange mDeferredCollisionAction = EDeferredCollisionChange::DCC_None;

	UPROPERTY(EditAnywhere, meta = ( ShowOnlyInnerProperties ) )
    FPlayerZiplineData mZiplineData;

	/* Base speed for moving straight on zipline */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineSpeed;

	/* Sprint speed for moving straight on zipline */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineSprintSpeed;
	
	/* The speed of which the character corrects its position when zip lining. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineCorrectionSpeedMultiplier;

	/* How fast we interpolate to max speed when zip lining. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineVelocityInterpolationSpeed;

	/* How fast we interpolate to max speed when zip lining while sprinting. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineVelocityInterpolationSprintSpeed;
	
	/* Speed multiplier used when going upwards on zipline */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineSpeedMultiplierUp;

	/* Speed multiplier used when going downwards on zipline */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline" )
	float mZiplineSpeedMultiplierDown;

	/* When sprinting on zipline we use the normal interpolation speed until we reach this percentage of the normal zipline velocity. */
	UPROPERTY( EditDefaultsOnly, Category = "Movement|Zipline", meta=(ClampMin=0.0f, ClampMax=1.0f, UIMin=0.0f, UIMax=1.0f ))
	float mZiplineSprintSettingsCutoff;
	
	//Cheat
	public:
	bool mCheatGhost : 1;
	float CheatFlySpeedVertical;
	void ZeroOutFallVelocity();
	//end Cheat
};

class FSavedMove_FGMovement final : public FSavedMove_Character
{
	typedef FSavedMove_Character Super;
public:
	///@brief Resets all saved variables.
	virtual void Clear() override;

	///@brief Store input commands in the compressed flags.
	virtual uint8 GetCompressedFlags() const override;

	///@brief This is used to check whether or not two moves can be combined into one.
	///Basically you just check to make sure that the saved variables are the same.
	virtual bool CanCombineWith(const FSavedMovePtr& newMove, ACharacter* character, float maxDelta) const override;

	///@brief Sets up the move before sending it to the server. 
	virtual void SetMoveFor(ACharacter* character, float inDeltaTime, FVector const& newAccel, class FNetworkPredictionData_Client_Character & clientData) override;
	///@brief Sets variables on character movement component before making a predictive correction.
	virtual void PrepMoveFor(class ACharacter* character) override;

	uint8 mSavedIsThrusting : 1;

	uint8 mSavedIsSprinting : 1;

	uint8 mSavedIsParachuting : 1;

	uint8 mSavedIsSliding : 1;

	uint8 mSavedIsPressingJump : 1;

	FVector mSavedHookLocation;

	float mPipeMoveProgress;
	float mPipeMoveVel;
	float mPipeMoveTime;
	AActor* mPipeMovePipe = nullptr;
};

class FNetworkPredictionData_Client_FGMovement final : public FNetworkPredictionData_Client_Character
{
public:
	explicit FNetworkPredictionData_Client_FGMovement(const UCharacterMovementComponent& clientMovement);

	typedef FNetworkPredictionData_Client_Character Super;

	///@brief Allocates a new copy of our custom saved move
	virtual FSavedMovePtr AllocateNewMove() override;
};