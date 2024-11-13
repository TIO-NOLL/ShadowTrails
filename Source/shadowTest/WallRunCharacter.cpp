//------------------------------------------------
// �t�@�C�����FWallRunCharacter.cpp
// �������e	�F�ǂ𑖂邱�Ƃ̂ł���L�����N�^�[
// �쐬��	�F�O��V
// �X�V��	�F12/04		�쐬
//------------------------------------------------


#include "WallRunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "shadowTestGameMode.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"

// �R���X�g���N�^
AWallRunCharacter::AWallRunCharacter()
	: m_pSpringArm(NULL)
	, m_pCamera(NULL)
	, m_pGenerateCharacter(ACharacter::StaticClass())
	, m_charaMoveInput(FVector::ZeroVector)
	, m_cameraRotateInput(FVector::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-70.f, -15.f))
	, m_CameraSensitivity(0.f)
	, m_moveSpeed(10.f)
	, m_RayLength(20.f)
	, m_bCanControl(true)
{
	// ���̃A�N�^�[�̃e�B�b�N���񂷂��ǂ���
	PrimaryActorTick.bCanEverTick = true;

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

// �������ɌĂ΂�鏈��
void AWallRunCharacter::BeginPlay()
{
	Super::BeginPlay();

	//�L�����̈ړ����@���u���V�v�ɂ���
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

// �t���[�����ƂɌĂ΂�鏈��
void AWallRunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMove(DeltaTime);

	UpdateCamera(DeltaTime);

	UpdateRay();
}

// �C���v�b�g�L�[�Ƃ̃o�C���h
void AWallRunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�J������]
	InputComponent->BindAxis("CameraPitch", this, &AWallRunCharacter::Cam_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &AWallRunCharacter::Cam_RotateYaw);

	//�ړ�
	InputComponent->BindAxis("MoveForward", this, &AWallRunCharacter::Chara_MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AWallRunCharacter::Chara_MoveRight);
}

//�J�����X�V����
void AWallRunCharacter::UpdateCamera(float _deltaTime)
{
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
void AWallRunCharacter::UpdateMove(float _deltaTime)
{
	//�ړ�����������ꍇ
	if (!m_charaMoveInput.IsZero())
	{
		//�R���g���[���\�ȏꍇ�ɂ̂�
		if (m_bCanControl == false) { return; }

		//���͂ɍ��킹�āAActor��O�㍶�E�Ɉړ�
		if (m_pSpringArm != NULL)
		{
			FVector NewLocation = GetActorLocation();

			//�L�����N�^�[�̈ړ�
			{
				//�ق��̃I�u�W�F�N�g�ɉe�����ꂸ�AZ�������ƕ��s�ɓ���
				const FVector upVec = FVector(0.f, 0.f, 1.f);
				AddMovementInput(upVec, m_charaMoveInput.Y * m_moveSpeed);
				const FVector rightVec = GetActorRightVector();
				AddMovementInput(rightVec, m_charaMoveInput.X * m_moveSpeed);
			}

			////�ړ�����L�����N�^�[�̉�]
			//{
			//	//�p�x��Degree�̊p�x�͈̔͂ŕ\�L(-180���`180��)
			//	USkeletalMeshComponent* pMeshComp = GetMesh();
			//	if (pMeshComp != NULL)
			//	{
			//		//�A�[�N�^���W�F���g���g���ăR���g���[���[�̓��͕������Ȃ��p�x�����߂�
			//		float angle = atan2(m_charaMoveInput.X, m_charaMoveInput.Y);
			//		//Radian�l��Degree�ɕϊ�
			//		float angleDeg = FMath::RadiansToDegrees(angle);

			//		//���͂����p�x + ���b�V���̉�]���x + Actor�ɑ΂��ĉ�]���Ă���SpringArm�̑��Ίp�x
			//		float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + m_pSpringArm->GetRelativeRotation().Yaw;

			//		pMeshComp->SetRelativeRotation(FRotator(pMeshComp->GetRelativeRotation().Pitch, newYaw, pMeshComp->GetRelativeRotation().Roll));
			//	}
			//}
		}
	}
}

//���C���΂��֐�
void AWallRunCharacter::UpdateRay()
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	//���̃I�u�W�F�N�g�̃��b�V�����擾����
	USkeletalMeshComponent* pMeshComp = GetMesh();

	if (pMeshComp != NULL)
	{
		//�v���C���[�̃��b�V���T�C�Y���擾����
		USkeletalMeshComponent* pPlayerMesh = this->GetMesh();
		FVector playerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * this->GetActorScale();

		//���C���΂�
		//�n�_ Start�FActor�̔w��
		FVector Start = GetActorLocation() + (-GetActorUpVector()  * (playerMeshSize / 2));
		//�I�_ End�FStart����O����m_ActionForwardLength�̋���
		FVector End = GetActorLocation() + (-GetActorUpVector() * (playerMeshSize / 2)) + pMeshComp->GetUpVector() * -1.f * m_RayLength;

		//�f�o�b�O�p�Ƀ��C����`��
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);

		//�R���W��������Ŗ������鍀�ڂ��w��
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);

		//���C�Ƀq�b�g�����I�u�W�F�N�g���i�[����ϐ�
		FHitResult RayHit;

		//�q�b�g�����ۂ�true�ɂȂ�t���O,
		bool isHit = GetWorld()->LineTraceSingleByChannel(RayHit, Start, End, ECC_WorldStatic, CollisionParams);

		//�q�b�g�����I�u�W�F�N�g���Ȃ��Ȃ����ꍇ
		if (!isHit)
		{
			// �X�|�[���ʒu���w��
			FVector SpawnLocation = GetActorLocation();

			// �X�|�[�����̉�]���w��
			FRotator SpawnRotation(GetActorRotation());

			// �X�|�[������A�N�^�[��Transform���쐬
			FTransform SpawnTransform(SpawnRotation, SpawnLocation);

			//���̃A�N�^�[�̍��W�ɃA�N�^�[�𐶐�����
			APlayerCharacter* SpawnActor = GetWorld()->SpawnActor<APlayerCharacter>(m_pGenerateCharacter, SpawnTransform); // �X�|�[������

			if (SpawnActor)
			{
				//ShadowGameMode����SwitchCharacter�֐����Ăяo���A�v���C���[�L�����N�^�[��؂�ւ���
				AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
				AshadowTestGameMode* shadowTestGameMode = Cast<AshadowTestGameMode>(CurrentGameMode);

				if (shadowTestGameMode)
				{
					//shadowTestGameMode->SwitchCharacter(SpawnActor);
				}
				else
				{
					// �Q�[�����[�h���擾�ł��Ȃ������ꍇ�̃G���[�n���h�����O
					UE_LOG(LogTemp, Error, TEXT("Failed to get AShadowGameMode"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to Generate SpawnActor"));
			}
		}
	}
}

//�y���̓o�C���h�z�J������]�FPitch(Y��)
void AWallRunCharacter::Cam_RotatePitch(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.Y = _axisValue * m_CameraSensitivity;
}

//�y���̓o�C���h�z�J������]�FYaw(Z��)
void AWallRunCharacter::Cam_RotateYaw(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	m_cameraRotateInput.X = _axisValue * m_CameraSensitivity;
}

//�y���̓o�C���h�z�L�����ړ��F�㉺
void AWallRunCharacter::Chara_MoveForward(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	m_charaMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}

//�y���̓o�C���h�z�L�����ړ��F���E
void AWallRunCharacter::Chara_MoveRight(float _axisValue)
{
	//�R���g���[���\�ȏꍇ�܂��͔��������Ă��Ȃ��Ƃ�
	if (m_bCanControl == false) { return; }

	m_charaMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 1.0f) * 0.5f;
}