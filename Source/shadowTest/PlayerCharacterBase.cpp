//------------------------------------------------
// �t�@�C�����FPlayerCharacterBase.h
// �������e	�FPlayer�̈ړ��ƃJ�����̑��������x�[�X�L�����N�^�[�N���X
// �쐬��	�F�O��V
// �X�V��	�F12/08		�쐬
//			�@12/10		�J�����̐���A�ړ������A����\�t���O�؂�ւ��A��Ԑ؂�ւ���ǉ�
//------------------------------------------------

#include "PlayerCharacterBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "ActionObject_Box.h"
#include "shadowTestGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "WallRunCharacter.h"

// �R���X�g���N�^
APlayerCharacterBase::APlayerCharacterBase()
	: m_pSpringArm(NULL)
	, m_pCamera(NULL)
	, m_pActionObject(NULL)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_PlayerCorrectPosition(FVector::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-70.f, -15.f))
	, m_moveSpeed(10.f)
	, m_ActionForwardLength(30.f)
	, m_CameraSensitivity(5.f)
	, m_DecreaseRateofMove(3.f)
	, m_InterpolationSpeed(5.f)
	, m_bCanControl(true)
	, m_bIsBoxPush(false)
	, m_bCanWallRunning(false)
	, m_bCameraLock(true)
	, m_state(normal)
	, m_ChangeSpeed(0.0f)
{
	// Tick()�𓮂������ǂ�����؂�ւ�
	PrimaryActorTick.bCanEverTick = true;

	//�f�t�H���g�v���C���[�Ƃ��Đݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//�X�v�����O�A�[���̃I�u�W�F�N�g����
	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != NULL)
	{
		m_pSpringArm->SetupAttachment(RootComponent);
		//�J�����̃R���W�����e�X�g���s�����ݒ�
		m_pSpringArm->bDoCollisionTest = true;
		//�J�����Ǐ]���O���g�����ݒ�
		m_pSpringArm->bEnableCameraLag = false;
		//�J�����Ǐ]���O�̑��x��ݒ�
		m_pSpringArm->CameraLagSpeed = 10.f;
		//�J������]���O���g�����ݒ�
		m_pSpringArm->bEnableCameraRotationLag = false;
		//�J������]���O�̑��x��ݒ�
		m_pSpringArm->CameraRotationLagSpeed = 10.f;
	}

	//�J�����̃I�u�W�F�N�g�𐶐�
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != NULL) && (m_pSpringArm != NULL))
	{
		//�J�������X�v�����O�A�[���ɃA�^�b�`
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}


}

// �������̏���
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// �t���[�����Ƃ̏���
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�J�����X�V����
	UpdateCamera(DeltaTime);

	//�ړ�����
	UpdateMove(DeltaTime);
}

// �C���v�b�g�L�[�Ƃ̃o�C���h
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�J������]
	InputComponent->BindAxis("CameraPitch", this, &APlayerCharacterBase::Cam_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &APlayerCharacterBase::Cam_RotateYaw);

	//�J�����̃��b�N����������
	InputComponent->BindAction("CameraUnLock", IE_Pressed, this, &APlayerCharacterBase::Cam_MoveUnlock);
	InputComponent->BindAction("CameraUnLock", IE_Released, this, &APlayerCharacterBase::Cam_MoveLock);

	//�ړ�
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacterBase::Chara_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacterBase::Chara_MoveRight);

	//�A�N�V����
	InputComponent->BindAction("Action", IE_Pressed, this, &APlayerCharacterBase::Chara_Action);

	//�Ǒ���
	InputComponent->BindAction("WallRun", IE_Pressed, this, &APlayerCharacterBase::Chara_WallRun);


}

//�J�����X�V����
void APlayerCharacterBase::UpdateCamera(float _deltaTime)
{
	//�R���g���[���\�ȏꍇ�ɂ̂�
	if (m_bCanControl == false) { return; }

	//���[�g�I�u�W�F�N�g�𒆐S�ɁA�X�v�����O�A�[���ɂ��Ă���J��������]������
	if (m_pSpringArm != NULL)
	{
		//���݂�FPS�𑪒�
		float fps = 1.0f / _deltaTime;

		//�����������Ă��A��葬�x�ŃJ����������悤�ɕ␳
		float rotateCorrection = 60.f / fps;

		//�J�����̐V�����p�x�����߂�
		//���݂̊p�x���擾
		FRotator NewRotation = m_pSpringArm->GetRelativeRotation();

		//Yaw�͓��͂�������
		NewRotation.Yaw += m_cameraRotateInput.X * rotateCorrection;

		//Pitch���㉺�����p�x�͈͓̔��Ő؂�
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + (m_cameraRotateInput.Y * rotateCorrection), m_cameraPitchLimit.X, m_cameraPitchLimit.Y);

		//�V�����p�x�𔽉f
		m_pSpringArm->SetRelativeRotation(NewRotation);
	}
}

//�ړ�����
void APlayerCharacterBase::UpdateMove(float _deltaTime)
{
	//�R���g���[���\�ȏꍇ�ɂ̂�
	if (m_bCanControl == false) { return; }

	//�ړ�����������ꍇ
	if (!m_charaMoveInput.IsZero())
	{
		//���͂ɍ��킹�āAActor��O�㍶�E�Ɉړ�
		if (m_pSpringArm != NULL)
		{
			FVector NewLocation = GetActorLocation();

			//�L�����N�^�[�̈ړ�
			{
				//�������������Ă����ꍇ
				if (m_bIsBoxPush == true)
				{
					//�v���C���[�̃��b�V���擾
					USkeletalMeshComponent* pMeshComp = GetMesh();

					//�������ɍ��킹�Ĉړ��ʂ����炷
					m_charaMoveInput.Y /= m_DecreaseRateofMove;
					AddMovementInput(pMeshComp->GetForwardVector(), m_charaMoveInput.Y * m_moveSpeed);
				}
				else
				{
					// 22cu0222 �c�� �ǋL�F2024/02/13---------------------------------
					if (m_state == damage)
					{
						m_charaMoveInput.Y /= m_ChangeSpeed;
						m_charaMoveInput.X /= m_ChangeSpeed;
					}
					// --------------------------------------------------------------

					//SpringArm�����������ɁA���͂ɂ��ړ��ʂ�PawnMovementComponent�ɓn
					const FVector forwardVec = m_pSpringArm->GetForwardVector();
					AddMovementInput(forwardVec, m_charaMoveInput.Y * m_moveSpeed);
					const FVector rightVec = m_pSpringArm->GetRightVector();
					AddMovementInput(rightVec, m_charaMoveInput.X * m_moveSpeed);
				}
			}

			//�ړ�����L�����N�^�[�̉�]
			if (m_bIsBoxPush == false)
			{
				//�p�x��Degree�̊p�x�͈̔͂ŕ\�L(-180���`180��)
				USkeletalMeshComponent* pMeshComp = GetMesh();
				if (pMeshComp != NULL)
				{
					//���݂̉�]��ێ�
					FRotator CurrentRotation = pMeshComp->GetRelativeRotation();

					//�A�[�N�^���W�F���g���g���ăR���g���[���[�̓��͕������Ȃ��p�x�����߂�
					float angle = atan2(m_charaMoveInput.X, m_charaMoveInput.Y);
					//Radian�l��Degree�ɕϊ�
					float angleDeg = FMath::RadiansToDegrees(angle);

					//���͂����p�x + ���b�V���̉�]���x + Actor�ɑ΂��ĉ�]���Ă���SpringArm�̑��Ίp�x
					float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + m_pSpringArm->GetRelativeRotation().Yaw;

					//�V������]��ێ�
					FRotator NewRotation = FRotator(pMeshComp->GetRelativeRotation().Pitch, newYaw, pMeshComp->GetRelativeRotation().Roll);

					if (_deltaTime == 0.f || CurrentRotation == NewRotation) { return; }

					if (m_InterpolationSpeed <= 0.f) { pMeshComp->SetRelativeRotation(NewRotation); }

					//��]���x���v�Z
					float DeltaInterpSpeed = m_InterpolationSpeed * _deltaTime;
					//��]�ʂ̍ŒZ�����߂�
					const FRotator DeltaRotate = (NewRotation - CurrentRotation).GetNormalized();

					if (DeltaRotate.IsNearlyZero()) { pMeshComp->SetRelativeRotation(NewRotation); }

					//��t���[���̉�]�ʁi�ŒZ�̉�]�� �~ ��]���x[0�`1]�j
					const FRotator DeltaMove = DeltaRotate * FMath::Clamp<float>(DeltaInterpSpeed, 0.f, 1.f);

					//���݂̉�]�Ɉ�t���[���̉�]�ʂ𑫂�������Ԃ�
					pMeshComp->SetRelativeRotation((CurrentRotation + DeltaMove).GetNormalized());
				}
			}
		}
	}
}

//���C���΂��֐�
APlayerCharacterBase::RayInfo APlayerCharacterBase::FlyRay()
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return { FHitResult(),FVector::ZeroVector, FVector::ZeroVector, false }; }

	//���̃I�u�W�F�N�g�̃��b�V�����擾����
	USkeletalMeshComponent* pMeshComp = GetMesh();

	if (pMeshComp != NULL)
	{
		//���C���΂�
		//�n�_ Start�FActor
		FVector Start = GetActorLocation();
		//�I�_ End�FStart����O����m_ActionForwardLength�̋���
		FVector End = GetActorLocation() + pMeshComp->GetForwardVector() * m_ActionForwardLength;

		////�f�o�b�O�p�Ƀ��C����`��
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);

		//�R���W��������Ŗ������鍀�ڂ��w��
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//���C�Ƀq�b�g�����I�u�W�F�N�g���i�[����ϐ�
		FHitResult RayHit;

		//�q�b�g�����T�[�t�F�C�X�̖@���x�N�g�����i�[����ϐ�
		FVector RayHitNormal;

		//�q�b�g���Ă���|�C���g�̍��W���i�[����ϐ�
		FVector RayHitPoint;

		//�q�b�g�����ۂ�true�ɂȂ�t���O,
		bool isHit = GetWorld()->LineTraceSingleByChannel(RayHit, Start, End, ECC_WorldStatic, CollisionParams);

		if (isHit)
		{
			RayHitNormal = RayHit.Normal;
			RayHitPoint = RayHit.ImpactPoint;
		}

		RayInfo NowHitInfo = { RayHit, RayHitNormal, RayHitPoint, isHit };

		return NowHitInfo;
	}

	return { FHitResult(),FVector::ZeroVector, FVector::ZeroVector, false };
}
//�����ړ�����ہA�v���C���[�̍��W��␳����֐�
void APlayerCharacterBase::PositionCorrect()
{
	////�f�o�b�O�p���O
	//UE_LOG(LogTemp, Warning, TEXT("CorrectTime = %f"), m_CorrectTime);
	//UE_LOG(LogTemp, Warning, TEXT("CorrectCnt = %f"), m_CorrectCnt);

	////�f�o�b�O�p���O
	//UE_LOG(LogTemp, Log, TEXT("Distance = %f"), m_Distance);

	//�ݒ肵�����W�Ɉړ�
	SetActorLocation(m_PlayerCorrectPosition);

	////�f�o�b�O�p���O
	//UE_LOG(LogTemp, Warning, TEXT("Correct Now"));

	////���C���΂�
	////�n�_ Start�FActor
	//FVector Start = GetActorLocation();
	////�I�_ End�FStart����O����m_ActionForwardLength�̋���
	//FVector End = m_PlayerCorrectPosition;

	////�f�o�b�O�p�Ƀ��C����`��
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f);

	//�f�o�b�O�p���O
	UE_LOG(LogTemp, Warning, TEXT("Correct End"));
}

//�y���̓o�C���h�z�J������]�FPitch(Y��)
void APlayerCharacterBase::Cam_RotatePitch(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.Y = _axisValue * m_CameraSensitivity;

	if (m_bCameraLock == true) { m_cameraRotateInput.Y = 0.f; }
}

//�y���̓o�C���h�z�J������]�FYaw(Z��)
void APlayerCharacterBase::Cam_RotateYaw(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.X = _axisValue * m_CameraSensitivity;

	if (m_bCameraLock == true) { m_cameraRotateInput.X = 0.f; }
}

//�y���̓o�C���h�z�J���������b�N����
void APlayerCharacterBase::Cam_MoveLock()
{
	m_bCameraLock = true;
}

//�y���̓o�C���h�z�J�����̃��b�N����������
void APlayerCharacterBase::Cam_MoveUnlock()
{
	m_bCameraLock = false;
}

//�y���̓o�C���h�z�L�����ړ��F�O��
void APlayerCharacterBase::Chara_MoveForward(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	m_charaMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}

//�y���̓o�C���h�z�L�����ړ��F���E
void APlayerCharacterBase::Chara_MoveRight(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false || m_bIsBoxPush == true) { return; }

	m_charaMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}

//�y���̓o�C���h�z�A�N�V�����F�M�~�b�N�N��
void APlayerCharacterBase::Chara_Action()
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	RayInfo RayHit = FlyRay();

	//�q�b�g�����I�u�W�F�N�g������ꍇ
	if (RayHit.HitFlag)
	{
		//�q�b�g����Actor�̃^�O�𒲂ׂ�
		//�A�N�V�������N�������Ƃ̂ł���I�u�W�F�N�g�������ꍇ
		if (RayHit.HitActor.GetActor()->ActorHasTag("ActionObject"))
		{
			////�f�o�b�O�m�F�p
			//FString str = RayHit.GetActor()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("HitActor [ActionObject]ObjectName : %s"), *str);

			//ActionObjectBase�ɃL���X�g
			m_pActionObject = Cast<AActionObjectBase>(RayHit.HitActor.GetActor());
		}
		else
		{
			//�A�N�V�������N�����I�u�W�F�N�g�ȊO�̏ꍇ��ActionObject�̒��g����ɂ��Ă���
			m_pActionObject = NULL;
		}
	}
	else
	{
		//�����q�b�g���Ă��Ȃ��ꍇ��ActionObject�̒��g����ɂ��Ă���
		m_pActionObject = NULL;
	}


	if (m_pActionObject != NULL)
	{
		//Actor���̊֐����Ăяo��
		m_pActionObject->OnActionthisObject();
	}

	if (m_bIsBoxPush)
	{
		m_bIsBoxPush = false;
		m_pActionObject = NULL;
		UE_LOG(LogTemp, Warning, TEXT("FALSE"));
	}

	if (m_pActionObject != NULL && m_bCanControl == true)
	{
		//���C�ɐG��Ă���I�u�W�F�N�g�������锠��������
		if (m_pActionObject->ActorHasTag("Box"))
		{
			//���̃��b�V���R���|�[�l���g���擾
			UStaticMeshComponent* pObjectMesh = m_pActionObject->m_pObjectMeshComp;


			//�v���C���[����]������(���C�Ƀq�b�g���Ă���T�[�t�F�C�X�̖@���̋t�x�N�g�����Q�Ƃ���]�Ƃ��ĕϊ��A���̌�������)
			FVector InvertedNormal = -RayHit.NormalVector;
			FRotator NowRotation = InvertedNormal.Rotation();

			//���b�V���R���|�[�l���g�̎擾
			USkeletalMeshComponent* pMeshComp = GetMesh();

			pMeshComp->SetWorldRotation(NowRotation);

			UE_LOG(LogTemp, Warning, TEXT("Rotation.Roll = %f"), NowRotation.Roll);
			UE_LOG(LogTemp, Warning, TEXT("Rotation.Yaw = %f"), NowRotation.Yaw);
			UE_LOG(LogTemp, Warning, TEXT("Rotation.Pitch = %f"), NowRotation.Pitch);

			//���̃��b�V���T�C�Y���擾����B
			FVector ObjectMeshSize = pObjectMesh->Bounds.GetBox().GetSize() * m_pActionObject->GetActorScale();

			////�f�o�b�O�p���O
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), ObjectMeshSize.X);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), ObjectMeshSize.Y);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), ObjectMeshSize.Z);

			//�v���C���[�̃��b�V���T�C�Y���擾����
			USkeletalMeshComponent* pPlayerMesh = this->GetMesh();
			FVector playerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * this->GetActorScale();

			////�f�o�b�O�p���O
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), playerMeshSize.X);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), playerMeshSize.Y);
			//UE_LOG(LogTemp, Warning, TEXT("ObjectSize = %f"), playerMeshSize.Z);

			//���̊֐����Ăяo���v���C���[���W�Ƃ̓��ς����߂�
			AActionObject_Box* pActionBox = Cast<AActionObject_Box>(m_pActionObject);
			float angle = pActionBox->GetAnglePlayer(this->GetActorLocation());
			////���Ԃ�ݒ肷��
			//pActionBox->SetTime(m_CorrectTime);

			//�v���C���[�̈ʒu��␳���邽�߂́A�u���b�N�ɑ΂�������x�N�g�����i�[����ϐ�
			FVector PlayerDirection = FVector(0.f, 0.f, 0.f);

			//���ς̐��l�ɉ����ăv���C���[���v���C���[�̃��b�V���Ɣ��̃��b�V���̃T�C�Y�����������ʒu�܂ňړ�����B
			if (angle >= -45.f && angle <= 45.f)
			{
				PlayerDirection = m_pActionObject->GetActorForwardVector();
			}
			else if (angle > 45.f && angle <= 135.f)
			{
				PlayerDirection = m_pActionObject->GetActorRightVector();
			}
			else if (angle > 135.f || angle < -135.f)
			{
				PlayerDirection = m_pActionObject->GetActorForwardVector();
				PlayerDirection = PlayerDirection * -1.0f;
			}
			else if (angle >= -135.f && angle < -45.f)
			{
				PlayerDirection = m_pActionObject->GetActorRightVector();
				PlayerDirection = PlayerDirection * -1.0f;
			}

			////�f�o�b�O�p���O
			//UE_LOG(LogTemp, Warning, TEXT("PlayerDirection.X = %f"), PlayerDirection.X);
			//UE_LOG(LogTemp, Warning, TEXT("PlayerDirection.Y = %f"), PlayerDirection.Y);
			//UE_LOG(LogTemp, Warning, TEXT("PlayerDirection.Z = %f"), PlayerDirection.Z);

			//�␳������W���v�Z����B
			FVector CorrecPos = PlayerDirection * (PlayerDirection * ((ObjectMeshSize + playerMeshSize) / 2.f));
			//�I�u�W�F�N�g�̍��W + �����x�N�g�� * ((�I�u�W�F�N�g�̃T�C�Y + �v���C���[���b�V���̃T�C�Y / 2))
			if (PlayerDirection.X == 1.0f || PlayerDirection.Y == 1.0f)
			{
				m_PlayerCorrectPosition = m_pActionObject->GetActorLocation() + CorrecPos;
			}
			else if (PlayerDirection.X == -1.0f || PlayerDirection.Y == -1.0f)
			{
				m_PlayerCorrectPosition = m_pActionObject->GetActorLocation() - CorrecPos;
			}

			//Z���ɂ����Ă̓v���C���[�̍��W���̂܂�
			m_PlayerCorrectPosition.Z = GetActorLocation().Z;

			//�f�o�b�O�p���O
			UE_LOG(LogTemp, Warning, TEXT("CorrectPosition.X = %f"), m_PlayerCorrectPosition.X);
			UE_LOG(LogTemp, Warning, TEXT("CorrectPosition.Y = %f"), m_PlayerCorrectPosition.Y);

			////�i�ޕ����x�N�g�����v�Z����
			//FVector Direction = m_PlayerCorrectPosition - this->GetActorLocation();
			//m_CorrectPositionVector = Direction.GetSafeNormal();

			////�i�ޑ��������v�Z����
			//m_ToCorrectPositionDistnce = m_PlayerCorrectPosition - GetActorLocation();
			////�v���C���[���ݒn����ړ�����l���v�Z����
			////m_CorrectPosition���猻�݂̃v���C���[�̍��W�������A�O�����̒藝���g�����Ƃňړ����������߂�
			//m_Distance = (float)sqrt(m_ToCorrectPositionDistnce.X * m_ToCorrectPositionDistnce.X) + (m_ToCorrectPositionDistnce.Y * m_ToCorrectPositionDistnce.Y);
			////�f�o�b�O�p���O
			//UE_LOG(LogTemp, Log, TEXT("Distance1 = %f"), m_Distance);

			//m_Distance = m_Distance / m_CorrectTime;

			////�f�o�b�O�p���O
			//UE_LOG(LogTemp, Warning, TEXT("CorrectPositionVector.X = %f"), m_CorrectPositionVector.X);
			//UE_LOG(LogTemp, Warning, TEXT("CorrectPositionVector.Y = %f"), m_CorrectPositionVector.Y);
			//UE_LOG(LogTemp, Warning, TEXT("CorrectPositionVector.Z = %f"), m_CorrectPositionVector.Z);

			//���̂��΂܂ňړ�����
			PositionCorrect();

			//���������Ă��邱�Ƃ�ʒB����t���O�𗧂Ă�
			m_bIsBoxPush = true;

		}
	}
}
//�y���̓o�C���h�z�A�N�V�����F�Ǒ���
void APlayerCharacterBase::Chara_WallRun()
{
	//���C���΂�
	RayInfo RayHit = FlyRay();

	//�q�b�g�����I�u�W�F�N�g������ꍇ
	if (RayHit.HitFlag)
	{
		//���ς����߂邽�߂�Z���ɕ��s�ȒP�ʃx�N�g��
		FVector Zaxis = FVector(0.f, 0.f, 1.f);

		////�m�F�p���O
		//UE_LOG(LogTemp, Error, TEXT("ImpactNormal.X = %f"), RayHit.ImpactNormal.X);
		//UE_LOG(LogTemp, Error, TEXT("ImpactNormal.Y = %f"), RayHit.ImpactNormal.Y);
		//UE_LOG(LogTemp, Error, TEXT("ImpactNormal.Z = %f"), RayHit.ImpactNormal.Z);
		// 
		//�q�b�g�����I�u�W�F�N�g�̃T�[�t�F�C�X�@�����擾���AZ���Ƃ̓��ς����߂�
		float product = FVector::DotProduct(RayHit.NormalVector, Zaxis);

		////�f�o�b�O�p���O
		//UE_LOG(LogTemp, Error, TEXT("product = %f"), product);

		//���ς�������x�����������ꍇ������ǂƂ݂Ȃ�
		if (product < 0.05f && product > -0.05f)
		{
			//�Ǒ���\�t���O�𗧂Ă�
			m_bCanWallRunning = true;

			////�f�o�b�O�p���O
			//UE_LOG(LogTemp, Warning, TEXT("m_GeneratePos.X = %f"), m_GeneratePosition.X);
			//UE_LOG(LogTemp, Warning, TEXT("m_GeneratePos.Y = %f"), m_GeneratePosition.Y);
			//UE_LOG(LogTemp, Warning, TEXT("m_GeneratePos.Z = %f"), m_GeneratePosition.Z);
		}
	}
	else
	{
		//�����q�b�g���Ă��Ȃ��ꍇ�A�Ǒ���\�t���O�����낵�Ă���
		m_bCanWallRunning = false;
	}

	if (m_bCanWallRunning == true)
	{
		////�f�o�b�O�p���O
		//UE_LOG(LogTemp, Warning, TEXT("WallRun_On"));

		// �X�|�[�����̉�]���w��(���C�Ƀq�b�g���Ă���T�[�t�F�C�X�̖@���̋t�x�N�g�����Q�Ƃ���]�Ƃ��ĕϊ��A���̌�������)
		FVector InvertedNormal = -RayHit.NormalVector;
		FRotator SpawnRotation = InvertedNormal.Rotation();

		// �X�|�[������A�N�^�[��Transform���쐬
		FTransform SpawnTransform(SpawnRotation, RayHit.HitPoint);

		//���C���G��Ă�����W�ɕʃA�N�^�[�𐶐�����
		AWallRunCharacter* SpawnActor = GetWorld()->SpawnActor<AWallRunCharacter>(m_pGenerateCharacter, SpawnTransform); // �X�|�[������

		if (SpawnActor)
		{
			//// ���������ꍇ�̏���
			//SpawnActor->SetActorLocation(m_GeneratePosition); // ���W��ݒ�

			//// ShadowGameMode����SwitchCharacter�֐����Ăяo���A�v���C���[�L�����N�^�[��؂�ւ���
			//AShadowGameMode* GameMode = Cast<AShadowGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
			//if (GameMode)
			//{
			//	GameMode->SwitchCharacter(SpawnActor);
			//}

			//���ݐݒ肵�Ă���Q�[�����[�h���擾
			AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
			//�L���X�g������
			AshadowTestGameMode* ShadowGameMode = Cast<AshadowTestGameMode>(CurrentGameMode);

			//NULL�`�F�b�N
			if (ShadowGameMode)
			{
				//�֐��Ăяo��
				//ShadowGameMode->SwitchCharacter(SpawnActor);
			}
			else
			{
				// �Q�[�����[�h���擾�ł��Ȃ������ꍇ�̃G���[�n���h�����O
				UE_LOG(LogTemp, Error, TEXT("Failed to get AShadowGameMode"));
			}
		}
		else
		{
			// ���s�����ꍇ�̏���
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn character."));
		}
	}
}


//�v���C���[�̏�Ԃ�؂�ւ���֐�
//normal
void APlayerCharacterBase::StateChangeNormal()
{
	m_state = normal;
}
//damage
void APlayerCharacterBase::StateChangeDamage()
{
	m_state = damage;
}
//dead
void APlayerCharacterBase::StateChangeDead()
{
	m_state = dead;
}

//����\�t���O��؂�ւ���֐�
void APlayerCharacterBase::SetCanMove(bool _flag)
{
	m_bCanControl = _flag;
}

// 22cu0222 �c�� �ǋL�F2024/02/13
// �v���C���[�̈ړ��ʂ�����������ʂ��i�[����֐�
void APlayerCharacterBase::ChangeSpeed(float m_Speed)
{
	m_ChangeSpeed = m_Speed;
}

/**
* �R���|�[�l���g�̑��Ή�]�����Z�b�g���āA�A�N�^�ƃ��b�V�������������������悤�ɂ���
*/
void APlayerCharacterBase::ResetRelativeRotation()
{
	if (m_pSpringArm != NULL)
	{
		m_pSpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	}

	// ���b�V���̑��Ή�]�����Z�b�g
	USkeletalMeshComponent* pMeshComponent = GetMesh();
	if (pMeshComponent != NULL)
	{
		pMeshComponent->SetRelativeRotation(FRotator(0.f, 270.f, 0.f));
	}

}

void APlayerCharacterBase::SetPlayerRotation(FRotator _rotation)
{
	m_pSpringArm->SetRelativeRotation(_rotation);

	USkeletalMeshComponent* pMeshComp = GetMesh();
	_rotation.Yaw += 270.f;
	pMeshComp->SetRelativeRotation(_rotation);
}