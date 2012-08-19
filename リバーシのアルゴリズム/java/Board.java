import java.util.*;

class Board
{
	public static final int BOARD_SIZE =  8;
	public static final int MAX_TURNS  = 60;

	private static final int NONE		 =   0;
	private static final int UPPER		 =   1;
	private static final int UPPER_LEFT	 =   2;
	private static final int LEFT		 =   4;
	private static final int LOWER_LEFT	 =   8;
	private static final int LOWER		 =  16;
	private static final int LOWER_RIGHT =  32;
	private static final int RIGHT		 =  64;
	private static final int UPPER_RIGHT = 128;

	private int RawBoard[][] = new int[BOARD_SIZE+2][BOARD_SIZE+2];
	private int Turns; // �萔(0����͂��܂�)
	private int CurrentColor; // ���݂̃v���C���[

	private Vector UpdateLog = new Vector();

	private Vector MovablePos[] = new Vector[MAX_TURNS+1];
	private int MovableDir[][][] = new int[MAX_TURNS+1][BOARD_SIZE+2][BOARD_SIZE+2];
	
	private ColorStorage Discs = new ColorStorage();
	
	public Board()
	{
		// Vector�̔z���������
		for(int i=0; i<=MAX_TURNS; i++)
		{
			MovablePos[i] = new Vector();
		}

		init();
	}
	
	public void init()
	{
		// �S�}�X���󂫃}�X�ɐݒ�
		for(int x=1; x <= BOARD_SIZE; x++)
		{
			for(int y=1; y <= BOARD_SIZE; y++)
			{
				RawBoard[x][y] = Disc.EMPTY;
			}
		}

		// �ǂ̐ݒ�
		for(int y=0; y < BOARD_SIZE + 2; y++)
		{
			RawBoard[0][y] = Disc.WALL;
			RawBoard[BOARD_SIZE+1][y] = Disc.WALL;
		}

		for(int x=0; x < BOARD_SIZE + 2; x++)
		{
			RawBoard[x][0] = Disc.WALL;
			RawBoard[x][BOARD_SIZE+1] = Disc.WALL;
		}


		// �����z�u
		RawBoard[4][4] = Disc.WHITE;
		RawBoard[5][5] = Disc.WHITE;
		RawBoard[4][5] = Disc.BLACK;
		RawBoard[5][4] = Disc.BLACK;

		// �ΐ��̏����ݒ�
		Discs.set(Disc.BLACK, 2);
		Discs.set(Disc.WHITE, 2);
		Discs.set(Disc.EMPTY, BOARD_SIZE*BOARD_SIZE - 4);

		Turns = 0; // �萔��0���琔����
		CurrentColor = Disc.BLACK; // ���͍�

		initMovable();
	}
	
	public boolean move(Point point)
	{
		if(point.x <= 0 || point.x > BOARD_SIZE) return false;
		if(point.y <= 0 || point.y > BOARD_SIZE) return false;
		if(MovableDir[Turns][point.x][point.y] == NONE) return false;
		
		flipDiscs(point);

		Turns++;
		CurrentColor = -CurrentColor;

		initMovable();

		return true;
	}
	
	public boolean undo()
	{
		// �Q�[���J�n�n�_�Ȃ�����߂�Ȃ�
		if(Turns == 0) return false;

		CurrentColor = -CurrentColor;
		
		Vector update = (Vector) UpdateLog.remove(UpdateLog.size()-1);

		// �O�񂪃p�X���ǂ����ŏꍇ����
		if(update.isEmpty())
		{
			// �O��̓p�X

			// MovablePos�y��MovableDir���č\�z
			MovablePos[Turns].clear();
			for(int x=1; x<=BOARD_SIZE; x++)
			{
				for(int y=1; y<=BOARD_SIZE; y++)
				{
					MovableDir[Turns][x][y] = NONE;
				}
			}
		}
		else
		{
			// �O��̓p�X�łȂ�

			Turns--;

			// �΂����ɖ߂�
			Point p = (Point) update.get(0);
			RawBoard[p.x][p.y] = Disc.EMPTY;
			for(int i=1; i<update.size(); i++)
			{
				p = (Point) update.get(i);
				RawBoard[p.x][p.y] = -CurrentColor;
			}
			
			// �ΐ��̍X�V
			int discdiff = update.size();
			Discs.set(CurrentColor, Discs.get(CurrentColor) - discdiff);
			Discs.set(-CurrentColor, Discs.get(-CurrentColor) + (discdiff - 1));
			Discs.set(Disc.EMPTY, Discs.get(Disc.EMPTY) + 1);
		}

		return true;
	}
	
	public boolean pass()
	{
		// �ł肪����΃p�X�ł��Ȃ�
		if(MovablePos[Turns].size() != 0) return false;
		
		// �Q�[�����I�����Ă���Ȃ�A�p�X�ł��Ȃ�
		if(isGameOver()) return false;

		CurrentColor = -CurrentColor;
		
		UpdateLog.add(new Vector());

		initMovable();
		
		return true;

	}
	
	public int getColor(Point point)
	{
		return RawBoard[point.x][point.y];
	}
	
	public int getCurrentColor()
	{
		return CurrentColor;
	}
	
	public int getTurns()
	{
		return Turns;
	}
	
	public boolean isGameOver()
	{
		// 60��ɒB���Ă�����Q�[���I��
		if(Turns == MAX_TURNS) return true;
		
		// �łĂ�肪����Ȃ�Q�[���I���ł͂Ȃ�
		if(MovablePos[Turns].size() != 0) return false;
		
		//
		//	���݂̎�ԂƋt�̐F���łĂ邩�ǂ������ׂ�
		//
		Disc disc = new Disc();
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
	
	public int countDisc(int color)
	{
		return Discs.get(color);
	}
	
	public Vector getMovablePos()
	{
		return MovablePos[Turns];
	}
	
	public Vector getHistory()
	{
		Vector history = new Vector();
		
		for(int i=0; i<UpdateLog.size(); i++)
		{
			Vector update = (Vector) UpdateLog.get(i);
			if(update.isEmpty()) continue; // �p�X�͔�΂�
			history.add(update.get(0));
		}
		
		return history;
	}
	
	public Vector getUpdate()
	{
		if(UpdateLog.isEmpty()) return new Vector();
		else return (Vector) UpdateLog.lastElement();
	}
	
	public int getLiberty(Point p)
	{
		// ��
		return 0;
	}
	
	private int checkMobility(Disc disc)
	{
		// ���ɐ΂���������u���Ȃ�
		if(RawBoard[disc.x][disc.y] != Disc.EMPTY) return NONE;

		int x, y;
		int dir = NONE;

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
	
	// MovableDir�y��MovablePos������������
	private void initMovable()
	{
		Disc disc;
		int dir;

		MovablePos[Turns].clear();

		for(int x=1; x<=BOARD_SIZE; x++)
		{
			for(int y=1; y<=BOARD_SIZE; y++)
			{
				disc = new Disc(x, y, CurrentColor);
				dir = checkMobility(disc);
				if(dir != NONE)
				{
					// �u����
					MovablePos[Turns].add(disc);
				}
				MovableDir[Turns][x][y] = dir;
			}
		}
	}
	
	private void flipDiscs(Point point)
	{
		int x, y;
		int dir = MovableDir[Turns][point.x][point.y];

		Vector update = new Vector();

		RawBoard[point.x][point.y] = CurrentColor;
		update.add(new Disc(point.x, point.y, CurrentColor));


		// ��

		if((dir & UPPER) != NONE) // ��ɒu����
		{
			y = point.y;
			while(RawBoard[point.x][--y] != CurrentColor)
			{
				RawBoard[point.x][y] = CurrentColor;
				update.add(new Disc(point.x, y, CurrentColor));
			}
		}


		// ��

		if((dir & LOWER) != NONE)
		{
			y = point.y;
			while(RawBoard[point.x][++y] != CurrentColor)
			{
				RawBoard[point.x][y] = CurrentColor;
				update.add(new Disc(point.x, y, CurrentColor));
			}
		}

		// ��

		if((dir & LEFT) != NONE)
		{
			x = point.x;
			while(RawBoard[--x][point.y] != CurrentColor)
			{
				RawBoard[x][point.y] = CurrentColor;
				update.add(new Disc(x, point.y, CurrentColor));
			}
		}

		// �E

		if((dir & RIGHT) != NONE)
		{
			x = point.x;
			while(RawBoard[++x][point.y] != CurrentColor)
			{
				RawBoard[x][point.y] = CurrentColor;
				update.add(new Disc(x, point.y, CurrentColor));
			}
		}

		// �E��

		if((dir & UPPER_RIGHT) != NONE)
		{
			x = point.x;
			y = point.y;
			while(RawBoard[++x][--y] != CurrentColor)
			{
				RawBoard[x][y] = CurrentColor;
				update.add(new Disc(x, y, CurrentColor));
			}
		}

		// ����

		if((dir & UPPER_LEFT) != NONE)
		{
			x = point.x;
			y = point.y;
			while(RawBoard[--x][--y] != CurrentColor)
			{
				RawBoard[x][y] = CurrentColor;
				update.add(new Disc(x, y, CurrentColor));
			}
		}

		// ����

		if((dir & LOWER_LEFT) != NONE)
		{
			x = point.x;
			y = point.y;
			while(RawBoard[--x][++y] != CurrentColor)
			{
				RawBoard[x][y] = CurrentColor;
				update.add(new Disc(x, y, CurrentColor));
			}
		}

		// �E��

		if((dir & LOWER_RIGHT) != NONE)
		{
			x = point.x;
			y = point.y;
			while(RawBoard[++x][++y] != CurrentColor)
			{
				RawBoard[x][y] = CurrentColor;
				update.add(new Disc(x, y, CurrentColor));
			}
		}

		// �΂̐����X�V

		int discdiff = update.size();

		Discs.set(CurrentColor, Discs.get(CurrentColor) + discdiff);
		Discs.set(-CurrentColor, Discs.get(-CurrentColor) - (discdiff - 1));
		Discs.set(Disc.EMPTY, Discs.get(Disc.EMPTY) - 1);
		
		UpdateLog.add(update);
	}

}
