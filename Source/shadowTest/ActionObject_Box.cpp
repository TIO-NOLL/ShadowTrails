//---------------------------------------------------------------------------
// �t�@�C����		�FActionObject_Box.cpp
// �T�v				�F���^�̔�A�N�V�����I�u�W�F�N�g�A�A�N�V�����R�}���h�����s���ꂽ�ۂɂ͓��������Ƃ��ł���
// �쐬��			�F22cu0127�@�O�� �V
// �X�V���e			�F2023/12/14�@�쐬
//---------------------------------------------------------------------------

//�C���N���[�h
#include "ActionObject_Box.h"
#include "Kismet/GamePlayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"


AActionObject_Box::AActionObject_Box()
	: m_pPlayer(NULL)
	, m_bNowCorrect(false)
	, m_pStaticMesh(NULL)
	, m_bIsPushed(false)
	, m_meshSize(FVector::ZeroVector)
	/*, m_CorrectTime(0.f)
	, m_CorrectCnt(0.f)*/
{
	
}

void AActionObject_Box::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;

	////�v���C���[�̃|�C���^���擾����
	//GetPlayerActor();

	if (m_pObjectMeshComp != NULL)
	{
		//���b�V���̒��g���擾
		m_pStaticMesh = m_pObjectMeshComp->GetStaticMesh();
	}

	//���b�V����NULL�`�F�b�N
	if (m_pStaticMesh != NULL)
	{
		//���b�V���̃o�E���f�B���O�{�b�N�X�T�C�Y���擾����
		m_meshSize = m_pStaticMesh->GetBounds().GetBox().GetSize();

		//�I�u�W�F�N�g��Actor�̃T�C�Y����������
		m_meshSize *= GetActorScale();
	}

	//Tick�����𖳌�����
	SetActorTickEnabled(false);
}

void AActionObject_Box::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////�f�o�b�O�p���O
	//UE_LOG(LogTemp, Warning, TEXT("m_CorrectTime: %f, m_CorrectCnt: %f"), m_CorrectTime, m_CorrectCnt);

	//if (m_CorrectTime + 2.0f > m_CorrectCnt)
	//{
	//	//���݂�FPS�𑪒�
	//	float fps = 1.0f / DeltaTime;

	//	//�����������Ă��A���m�Ɉړ�����悤�ɕ␳
	//	float fpsCorrection = 60.f / fps;

	//	//���Z
	//	m_CorrectCnt += fpsCorrection;
	//}
	//else 
	//{
	//	//������Ă���Œ��̍��W�X�V����
	//	IsPushed(DeltaTime);
	//}

	//������Ă���Œ��̍��W�X�V����
	IsPushed(DeltaTime);
}

//������Ă����Ԃ̎��A�v���C���[�̓����ɍ��킹�Ĉړ�����֐�
void AActionObject_Box::IsPushed(float _deltaTime)
{
	//�v���C���[�̃��b�V�������擾
	USkeletalMeshComponent* pPlayerMesh = Cast<ACharacter>(m_pPlayer)->GetMesh();

	//�v���C���[�̃��b�V���T�C�Y�擾
	FVector PlayerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * m_pPlayer->GetActorScale();

	//�v���C���[�̍��W���擾
	FVector PlayerLocation = pPlayerMesh->GetComponentLocation();

	//�V�����v���C���[�ƁA���̃I�u�W�F�N�g�̃��b�V���T�C�Y���l���������ʂ�ݒ肷��B
	FVector NewLocation = PlayerLocation + pPlayerMesh->GetForwardVector() * (m_meshSize + PlayerMeshSize) / 2;

	//Z���Ɋւ��Ă̓v���C���[�̍��W���l�����Ȃ�
	NewLocation.Z = GetActorLocation().Z;

	//�I�u�W�F�N�g�̍��W��ݒ�
	SetActorLocation(NewLocation);

	//�^�я�ԂłȂ��Ƃ��I�u�W�F�N�gTick��؂�
	if (m_bIsPushed == false)
	{
		//Tick�𖳌��ɂ���
		SetActorTickEnabled(false);
		/*if (m_CorrectCnt != 0.0f)
		{
			m_CorrectCnt = 0.0f;
		}*/
	}
}

//�v���C���[������Ăяo�����֐�
void AActionObject_Box::OnActionthisObject()
{
	//�t���O�̐؂�ւ�
	m_bIsPushed = !m_bIsPushed;

	//�f�o�b�O�p���O
	UE_LOG(LogTemp, Warning, TEXT("Pushed"));

	if (m_bIsPushed == true) 
	{ 
		//Tick��L���ɂ���
		SetActorTickEnabled(true);
	}
}

//�v���C���[�̃A�h���X���擾����֐�
void AActionObject_Box::GetPlayerActor(FVector PlayerPos)
{
	////m_pPlayer����̎��̂�
	//if (m_pPlayer == NULL)
	//{
	//	//�Q�[���S�̂ւ�Actor��������(�R�X�g���A��񂾂�)
	//	TSubclassOf<AActor> findClass;
	//	findClass = AActor::StaticClass();
	//	TArray<AActor*> actors;
	//	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);

	//	//�����̌���Actro�������
	//	if (actors.Num() > 0)
	//	{
	//		//Actor�̒��g�����ԂɌ������Ă���
	//		for (int idx = 0; idx < actors.Num(); idx++)
	//		{
	//			AActor* pActor = Cast<AActor>(actors[idx]);
	//			//�^�O���v���C���[��������
	//			if (pActor->ActorHasTag("Player"))
	//			{
	//				m_pPlayer = pActor;
	//				break;
	//			}
	//		}
	//	}
	//}

	//���C���΂�
		//�n�_ Start�FActor
	FVector Start = GetActorLocation();
	//�I�_ End�FStart����O����m_ActionForwardLength�̋���th;
	FVector End = PlayerPos;

	////�f�o�b�O�p�Ƀ��C����`��
	//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.f);

	//�R���W��������Ŗ������鍀�ڂ��w��
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	//���C�Ƀq�b�g�����I�u�W�F�N�g���i�[����ϐ�
	FHitResult RayHit;

	//�q�b�g�����ۂ�true�ɂȂ�t���O,
	bool isHit = GetWorld()->LineTraceSingleByChannel(RayHit, Start, End, ECC_WorldStatic, CollisionParams);

	//�q�b�g�����I�u�W�F�N�g������ꍇ
	if (isHit)
	{
		//�q�b�g����Actor�̃^�O�𒲂ׂ�
		//�A�N�V�������N�������Ƃ̂ł���I�u�W�F�N�g�������ꍇ
		if (RayHit.GetActor()->ActorHasTag("Player"))
		{
			////�f�o�b�O�m�F�p
			//FString str = RayHit.GetActor()->GetName();
			//UE_LOG(LogTemp, Warning, TEXT("HitActor [ActionObject]ObjectName : %s"), *str);

			//ActionObjectBase�ɃL���X�g
			m_pPlayer = Cast<ACharacter>(RayHit.GetActor());
		}
		else
		{
			//�A�N�V�������N�����I�u�W�F�N�g�ȊO�̏ꍇ��ActionObject�̒��g����ɂ��Ă���
			m_pPlayer = NULL;
		}
	}
}

/*
* ���̃I�u�W�F�N�g�̃t�H���[�h�x�N�g���ƃv���C���[�̍��W�܂ł̃x�N�g���̊p�x�����߂�֐�
*
*	�����FPlayerPos		�v���C���[�̍��W
*/
float AActionObject_Box::GetAnglePlayer(FVector PlayerPos)
{
	//�ΏۂƎ����̍��W���ƍ�
	FVector diff = PlayerPos - this->GetActorLocation();
	//���l�����ƂɃ��W�A���l�����߂�
	float rad = atan2(diff.Y, diff.X);
	//���W�A���l����p�x�ɕϊ�����
	float degree = FMath::RadiansToDegrees(rad);

	//�v���C���[�����Ƀ��C���΂��A�G�ꂽ�v���C���[�̃A�h���X���擾����
	GetPlayerActor(PlayerPos);


	//�p�x��Ԃ�(-180���`180��)
	return degree;
}

////���Ԃ�ݒ肷��֐�
//void AActionObject_Box::SetTime(float _CorrectTime)
//{
//	m_CorrectTime = _CorrectTime;
//}