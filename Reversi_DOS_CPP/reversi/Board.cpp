#include "Board.h"
#include "Reversi.h"


Board::Board()
{
	init();
}


//������
void Board::init()
{
	// �S�}�X���󂫃}�X�ɐݒ�
	for(int x=1; x <= BOARD_SIZE; x++)
	{
		for(int y=1; y <= BOARD_SIZE; y++)
		{
			RawBoard[x][y] = EMPTY;
		}
	}
	
	// �ǂ̐ݒ�i�c�j
	for(int y=0; y < BOARD_SIZE + 2; y++)
	{
		RawBoard[0][y] = WALL;
		RawBoard[BOARD_SIZE+1][y] = WALL;
	}

	// �ǂ̐ݒ�i���j
	for(int x=0; x < BOARD_SIZE + 2; x++)
	{
		RawBoard[x][0] = WALL;
		RawBoard[x][BOARD_SIZE+1] = WALL;
	}


	// �����z�u
	RawBoard[4][4] = WHITE;
	RawBoard[5][5] = WHITE;
	RawBoard[4][5] = BLACK;
	RawBoard[5][4] = BLACK;
	

	// �ΐ��̏����ݒ�
	Discs[BLACK] = 2;
	Discs[WHITE] = 2;
	Discs[EMPTY] = BOARD_SIZE*BOARD_SIZE - 4;

	Turns = 0; // �萔��0���琔����
	CurrentColor = BLACK; // ���͍�
	
	// update��S�ď���
	UpdateLog.clear();

	//
	initMovable();
}



//�΂��u���邩�ǂ������肷��
unsigned Board::checkMobility(const Disc& disc) const
{
	// ���ɐ΂���������u���Ȃ�
	if(RawBoard[disc.x][disc.y] != EMPTY) return NONE;

	int x, y;
	unsigned dir = NONE;

	// ��
	if(RawBoard[disc.x][disc.y-1] == -disc.color)
	{
		x = disc.x; y = disc.y-2;
		while(RawBoard[x][y] == -disc.color) { y--; }
		if(RawBoard[x][y] == disc.color) dir |= UPPER;
	}

	// ��
	if(RawBoard[disc.x][disc.y+1] == -disc.color)
	{
		x = disc.x; y = disc.y+2;
		while(RawBoard[x][y] == -disc.color) { y++; }
		if(RawBoard[x][y] == disc.color) dir |= LOWER;
	}

	// ��
	if(RawBoard[disc.x-1][disc.y] == -disc.color)
	{
		x = disc.x-2; y = disc.y;
		while(RawBoard[x][y] == -disc.color) { x--; }
		if(RawBoard[x][y] == disc.color) dir |= LEFT;
	}

	// �E
	if(RawBoard[disc.x+1][disc.y] == -disc.color)
	{
		x = disc.x+2; y = disc.y;
		while(RawBoard[x][y] == -disc.color) { x++; }
		if(RawBoard[x][y] == disc.color) dir |= RIGHT;
	}


	// �E��
	if(RawBoard[disc.x+1][disc.y-1] == -disc.color)
	{
		x = disc.x+2; y = disc.y-2;
		while(RawBoard[x][y] == -disc.color) { x++; y--; }
		if(RawBoard[x][y] == disc.color) dir |= UPPER_RIGHT;
	}

	// ����
	if(RawBoard[disc.x-1][disc.y-1] == -disc.color)
	{
		x = disc.x-2; y = disc.y-2;
		while(RawBoard[x][y] == -disc.color) { x--; y--; }
		if(RawBoard[x][y] == disc.color) dir |= UPPER_LEFT;
	}

	// ����
	if(RawBoard[disc.x-1][disc.y+1] == -disc.color)
	{
		x = disc.x-2; y = disc.y+2;
		while(RawBoard[x][y] == -disc.color) { x--; y++; }
		if(RawBoard[x][y] == disc.color) dir |= LOWER_LEFT;
	}

	// �E��
	if(RawBoard[disc.x+1][disc.y+1] == -disc.color)
	{
		x = disc.x+2; y = disc.y+2;
		while(RawBoard[x][y] == -disc.color) { x++; y++; }
		if(RawBoard[x][y] == disc.color) dir |= LOWER_RIGHT;
	}


	return dir;

}


//point�Ŏw�肳�ꂽ�ʒu�ɐ΂�łB
//���������� return true
//�y�сA���̃^�[���̌v�Z���ʂ𒲍��J�n����B
bool Board::move(const Point& point)
{
	//�΂��łĂ�ʒu�����肷��
	if(point.x <= 0 || point.x > BOARD_SIZE) return false;
	if(point.y <= 0 || point.y > BOARD_SIZE) return false;
	if(MovableDir[Turns][point.x][point.y] == NONE) return false; //checkMobility�̌��ʁB���ɐ΂����邩�A�u���Ȃ��ꏊ���w�肵���Ȃ� false��Ԃ�

	//�΂�łA�ύX�_�ǐՁA�Η��Ԃ��A�ΐ��J�E���g�AUpdateLog�L�^
	flipDiscs(point);

	//�萔���C���N�������g
	Turns++;
	CurrentColor = -CurrentColor;//�������@or�@������

	//MovableDir��MovablePos�𒲂ג����B�i�Čv�Z�j
	initMovable(); //���݂̋ǖʂŁA���݂̔Ԏ�(��or��)���u����ꏊ���v�Z����

	return true;
}


//���݂̋ǖʂŁA���݂̔Ԏ�(��or��)���u����ꏊ���v�Z����
//MovableDir��MovablePos���Čv�Z����
void Board::initMovable()
{
	Disc disc(0, 0, CurrentColor);//�\���̂̃C���X�^���X�����ݐF�ō쐬 

	int dir;

	//�|�W�V�����N���A
	MovablePos[Turns].clear();

	//�Ղ̑S�Ă̖ڂɂ����āA�u���邩�ǂ����𒲂ׂ�B
	for(int x=1; x<=BOARD_SIZE; x++)//x=0�̎��͕ǂȂ̂�1����J�n����
	{
		disc.x = x;
		for(int y=1; y<=BOARD_SIZE; y++)
		{
			disc.y = y;
			
			//�΂�u����ꏊ������
			dir = checkMobility(disc); //disc�̈ʒu�ɒu����Ȃ�Adisc���猩�Đ΂��Ԃ������\�����r�b�g���擾
			if(dir != NONE)
			{
				// �u����
				MovablePos[Turns].push_back(disc); //�����Ɋi�[�idisc�̃R�s�[���i�[�����H�j
			}

			MovableDir[Turns][x][y] = dir; //�u���������\�����r�b�g�iNONE�ł�NONE�ȊO�ł����ʂ��i�[�j
		}
	}


}


//�΂�łA�ύX�_�ǐՁA�Η��Ԃ��A�ΐ��J�E���g�AUpdateLog�L�^
void Board::flipDiscs(const Point& point)
{
	int x, y;

	//point���猩�āA�΂����]����������r�b�g�Ŏ擾
	int dir = MovableDir[Turns][point.x][point.y];

	//���I�y���[�V�����̑��샍�O�L�^�p
	std::vector<Disc> update;

	//�u�����ꏊ��point��u�����F�ɂ���
	RawBoard[point.x][point.y] = CurrentColor;

	//���O�ۑ��p�\���́B�i�Ԃ����΂̍��W�ƐF�j
	Disc operation(point.x, point.y, CurrentColor); // �s���������\����
	
	//�ŏ��ɒu�����΂����O�L�^
	update.push_back(operation);//�\���̂Ȃ̂œs�x�R�s�[��push�����

	// ��
	if(dir & UPPER)
	{
		y = point.y;
		operation.x = point.x;
		while(RawBoard[point.x][--y] != CurrentColor)
		{
			RawBoard[point.x][y] = CurrentColor;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// ��
	if(dir & LOWER)
	{
		y = point.y;
		operation.x = point.x;
		while(RawBoard[point.x][++y] == -CurrentColor)
		{
			RawBoard[point.x][y] = CurrentColor;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// ��

	if(dir & LEFT)
	{
		x = point.x;
		operation.y = point.y;
		while(RawBoard[--x][point.y] == -CurrentColor)
		{
			RawBoard[x][point.y] = CurrentColor;
			operation.x = x;
			update.push_back(operation);
		}
	}

	// �E
	if(dir & RIGHT)
	{
		x = point.x;
		operation.y = point.y;
		while(RawBoard[++x][point.y] == -CurrentColor)
		{
			RawBoard[x][point.y] = CurrentColor;
			operation.x = x;
			update.push_back(operation);
		}
	}

	// �E��
	if(dir & UPPER_RIGHT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[++x][--y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// ����
	if(dir & UPPER_LEFT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[--x][--y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// ����
	if(dir & LOWER_LEFT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[--x][++y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}

	// �E��
	if(dir & LOWER_RIGHT)
	{
		x = point.x;
		y = point.y;
		while(RawBoard[++x][++y] == -CurrentColor)
		{
			RawBoard[x][y] = CurrentColor;
			operation.x = x;
			operation.y = y;
			update.push_back(operation);
		}
	}


	x = point.x;
	y = point.y;


	// �΂̐����X�V
	unsigned discdiff = update.size();

	Discs[CurrentColor]  += discdiff;
	Discs[-CurrentColor] -= discdiff -1;
	Discs[EMPTY]--;
	
	//����� �s�������샍�O�ɒǉ�
	UpdateLog.push_back(update);


}



//�Q�[���I�����Ă����True�A�I�����Ă��Ȃ����false
bool Board::isGameOver() const
{
	// 60��ɒB���Ă�����Q�[���I��
	if(Turns == MAX_TURNS) return true;
	
	// �łĂ�肪����Ȃ�Q�[���I���ł͂Ȃ�
	if(MovablePos[Turns].size() != 0) return false;
	
	//	���݂̎�ԂƋt�̐F���łĂ邩�ǂ������ׂ�
	Disc disc;
	disc.color = -CurrentColor;
	for(int x=1; x<=BOARD_SIZE; x++)
	{
		disc.x = x;
		for(int y=1; y<=BOARD_SIZE; y++)
		{
			disc.y = y;
			// �u����ӏ���1�ł�����΃Q�[���I���ł͂Ȃ�
			if(checkMobility(disc) != NONE) return false;
		}
	}
	
	return true;
}


//�p�X����B�p�X�ł��Ȃ��ꍇ=false
bool Board::pass()
{
	// �ł肪����΃p�X�ł��Ȃ�
	if(MovablePos[Turns].size() != 0) return false;
	
	// �Q�[�����I�����Ă���Ȃ�A�p�X�ł��Ȃ�
	if(isGameOver()) return false;
	
	//�F�𔽓]����
	CurrentColor = -CurrentColor;
	
	// ���update��}�����Ă���
	std::vector<Disc> nullupdate;
	UpdateLog.push_back(nullupdate);
	
	//MovableDir��MovablePos���Čv�Z����
	initMovable();
	
	return true;
}


//undo�@���Ƃɖ߂��Ȃ��������ł��Ă��Ȃ��ꍇfalse
bool Board::undo()
{
	// �Q�[���J�n�n�_�Ȃ�����߂�Ȃ�
	if(Turns == 0) return false;

	CurrentColor = -CurrentColor;
	
	const std::vector<Disc> &update = UpdateLog.back();

	// �O�񂪃p�X���ǂ����ŏꍇ����
	if(update.empty()) // �O��̓p�X
	{
		// MovablePos�y��MovableDir���č\�z�Bpass�����Ƃ������͒u����ꏊ����������A�S��NONE�ɂ���
		MovablePos[Turns].clear();
		for(unsigned x=1; x<=BOARD_SIZE; x++)
		{
			for(unsigned y=1; y<=BOARD_SIZE; y++)
			{
				MovableDir[Turns][x][y] = NONE;
			}
		}
	}
	else // �O��̓p�X�łȂ�
	{
		//���߂�
		Turns--;

		//update[0] = �O��ł����ꏊ
		int x = update[0].x;
		int y = update[0].y;

		// �΂����ɖ߂�
		RawBoard[x][y] = EMPTY; //�O��ł����ꏊ����ɂ���
		for(unsigned i=1; i<update.size(); i++)
		{
			RawBoard[update[i].x][update[i].y] = -update[i].color;
		}

		// �ΐ��̍X�V
		unsigned discdiff = update.size();
		Discs[CurrentColor] -= discdiff;
		Discs[-CurrentColor] += discdiff -1;
		Discs[EMPTY]--;
	}

	// �s�v�ɂȂ���update��1�폜
	UpdateLog.pop_back();

	return true;
}


