class MidEvaluator implements Evaluator
{
	class EdgeParam
	{
		public EdgeParam add(EdgeParam src)
		{
			stable += src.stable;
			wing += src.wing;
			mountain += src.mountain;
			Cmove += src.Cmove;

			return this;
		}
		
		// ������Z�q�̑���
		public void set(EdgeParam e)
		{
			stable = e.stable;
			wing = e.wing;
			mountain = e.mountain;
			Cmove = e.Cmove;
		}

		public byte stable = 0; // �m��΂̌�
		public byte wing = 0; // �E�C���O�̌�
		public byte mountain = 0; // �R�̌�
		public byte Cmove = 0; // �댯��C�ł��̌�
	}

	class CornerParam
	{
		public byte corner = 0; // ���ɂ���΂̐�
		public byte Xmove = 0;  // �댯��X�ł��̌�
	}
	
	class EdgeStat
	{
		private EdgeParam[] data = new EdgeParam[3];
		
		public EdgeStat()
		{
			for(int i=0; i<3; i++) data[i] = new EdgeParam();
		}
		
		public void add(EdgeStat e)
		{
			for(int i=0; i<3; i++) data[i].add(e.data[i]);
		}
		
		public EdgeParam get(int color)
		{
			return data[color+1];
		}
	}

	class CornerStat
	{
		private CornerParam[] data = new CornerParam[3];
		public CornerStat()
		{
			for(int i=0; i<3; i++)
				data[i] = new CornerParam();
		}
		public CornerParam get(int color)
		{
			return data[color+1];
		}
	}


	// �d�݌W�����K�肷��\����
	class Weight
	{
		int mobility_w;
		int liberty_w;
		int stable_w;
		int wing_w;
		int Xmove_w;
		int Cmove_w;
	}
	
	private Weight EvalWeight;

	private static final int TABLE_SIZE = 6561; // 3^8
	private static EdgeStat[] EdgeTable = new EdgeStat[TABLE_SIZE];
	private static boolean TableInit = false;


	public MidEvaluator()
	{
		if(!TableInit)
		{
			//
			//	����N�����Ƀe�[�u���𐶐�
			//

			int[] line = new int[Board.BOARD_SIZE];
			generateEdge(line, 0);

			TableInit = true;
		}

		// �d�݌W���̐ݒ� (�S�ǖʋ���)
		
		EvalWeight = new Weight();

		EvalWeight.mobility_w = 67;
		EvalWeight.liberty_w  = -13;
		EvalWeight.stable_w   = 101;
		EvalWeight.wing_w     = -308;
		EvalWeight.Xmove_w    = -449;
		EvalWeight.Cmove_w    = -552;
	}
	
	public int evaluate(Board board)
	{
		EdgeStat edgestat;
		CornerStat cornerstat;
		int result;

		//
		//	�ӂ̕]��
		//

		edgestat  = EdgeTable[idxTop(board)];
		edgestat.add(EdgeTable[idxBottom(board)]);
		edgestat.add(EdgeTable[idxRight(board)]);
		edgestat.add(EdgeTable[idxLeft(board)]);

		//
		//	���̕]��
		//

		cornerstat = evalCorner(board);

		// �m��΂Ɋւ��āA���̐΂�2�񐔂��Ă��܂��Ă���̂ŕ␳�B

		edgestat.get(Disc.BLACK).stable -= cornerstat.get(Disc.BLACK).corner;
		edgestat.get(Disc.WHITE).stable -= cornerstat.get(Disc.WHITE).corner;

		//
		//	�p�����[�^�̐��`����
		//

		result =
			  edgestat.get(Disc.BLACK).stable * EvalWeight.stable_w
			- edgestat.get(Disc.WHITE).stable * EvalWeight.stable_w
			+ edgestat.get(Disc.BLACK).wing * EvalWeight.wing_w
			- edgestat.get(Disc.WHITE).wing * EvalWeight.wing_w
			+ cornerstat.get(Disc.BLACK).Xmove * EvalWeight.Xmove_w
			- cornerstat.get(Disc.WHITE).Xmove * EvalWeight.Xmove_w
			+ edgestat.get(Disc.BLACK).Cmove * EvalWeight.Cmove_w
			- edgestat.get(Disc.WHITE).Cmove * EvalWeight.Cmove_w
			;

		// �J���x�E����\�萔�̕]��

		if(EvalWeight.liberty_w != 0)
		{
			ColorStorage liberty = countLiberty(board);
			result += liberty.get(Disc.BLACK) * EvalWeight.liberty_w;
			result -= liberty.get(Disc.WHITE) * EvalWeight.liberty_w;
		}

		// ���݂̎�Ԃ̐F�ɂ��Ă̂݁A����\�萔�𐔂���
		result +=
			  board.getCurrentColor()
			* board.getMovablePos().size()
			* EvalWeight.mobility_w;

		return board.getCurrentColor() * result;

	}

	private void generateEdge(int[] edge, int count)
	{
		if(count == Board.BOARD_SIZE)
		{
			//
			//	���̃p�^�[���͊��������̂ŁA�ǖʂ̃J�E���g
			//

			EdgeStat stat = new EdgeStat();

			stat.get(Disc.BLACK).set(evalEdge(edge, Disc.BLACK));
			stat.get(Disc.WHITE).set(evalEdge(edge, Disc.WHITE));

			EdgeTable[idxLine(edge)] = stat;

			return;
		}

		// �ċA�I�ɑS�Ẵp�^�[����ԗ�

		edge[count] = Disc.EMPTY;
		generateEdge(edge, count +1);

		edge[count] = Disc.BLACK;
		generateEdge(edge, count +1);

		edge[count] = Disc.WHITE;
		generateEdge(edge, count +1);

		return ;

	}

	EdgeParam evalEdge(int line[], int color)
	{
		EdgeParam edgeparam = new EdgeParam();

		int x;

		//
		//	�E�B���O���̃J�E���g
		//

		if(line[0] == Disc.EMPTY && line[7] == Disc.EMPTY)
		{
			x = 2;
			while(x <= 5)
			{
				if(line[x] != color) break;
				x++;
			}
			if(x == 6) // ���Ȃ��Ƃ��u���b�N���ł��Ă���
			{
				if(line[1] == color && line[6] == Disc.EMPTY)
					edgeparam.wing = 1;
				else if(line[1] == Disc.EMPTY && line[6] == color)
					edgeparam.wing = 1;
				else if(line[1] == color && line[6] == color)
					edgeparam.mountain = 1;
			}
			else // ����ȊO�̏ꍇ�ɁA���ɗאڂ���ʒu�ɒu���Ă�����
			{
				if(line[1] == color)
					edgeparam.Cmove++;
				if(line[6] == color)
					edgeparam.Cmove++;
			}
		}

		//
		//	�m��΂̃J�E���g
		//

		// ������E�����ɑ���
		for(x = 0; x < 8; x++)
		{
			if(line[x] != color) break;
			edgeparam.stable++;
		}

		if(edgeparam.stable < 8)
		{
			// �E������̑������K�v
			for(x = 7; x > 0; x--)
			{
				if(line[x] != color) break;
				edgeparam.stable++;
			}
		}


		return edgeparam;

	}


	CornerStat evalCorner(Board board)
	{
		CornerStat cornerstat = new CornerStat();

		cornerstat.get(Disc.BLACK).corner=0; cornerstat.get(Disc.BLACK).Xmove=0;
		cornerstat.get(Disc.WHITE).corner=0; cornerstat.get(Disc.WHITE).Xmove=0;
		
		Point p = new Point();

		//	����
		p.x = 1; p.y = 1;
		cornerstat.get(board.getColor(p)).corner++;
		if(board.getColor(p) == Disc.EMPTY)
		{
			p.x = 2; p.y = 2;
			cornerstat.get(board.getColor(p)).Xmove++;
		}

		//	����
		p.x = 1; p.y = 8;
		cornerstat.get(board.getColor(p)).corner++;
		if(board.getColor(p) == Disc.EMPTY)
		{
			p.x = 2; p.y = 7;
			cornerstat.get(board.getColor(p)).Xmove++;
		}

		//	�E��
		p.x = 8; p.y = 8;
		cornerstat.get(board.getColor(p)).corner++;
		if(board.getColor(p) == Disc.EMPTY)
		{
			p.x = 7; p.y = 7;
			cornerstat.get(board.getColor(p)).Xmove++;
		}

		//	�E��
		p.x = 8; p.y = 1;
		cornerstat.get(board.getColor(p)).corner++;
		if(board.getColor(p) == Disc.EMPTY)
		{
			p.x = 7; p.y = 7;
			cornerstat.get(board.getColor(p)).Xmove++;
		}

		return cornerstat;

	}

	int idxTop(Board board)
	{
		int index = 0;
		
		int m = 1;
		Point p = new Point(0, 1);
		for(int i=Board.BOARD_SIZE; i>0; i--)
		{
			p.x = i;
			index += m * (board.getColor(p) + 1);
			m *= 3;
		}

		return index;
	}

	int idxBottom(Board board)
	{
		int index = 0;
		
		int m = 1;
		Point p = new Point(0, 8);
		for(int i=Board.BOARD_SIZE; i>0; i--)
		{
			p.x = i;
			index += m * (board.getColor(p) + 1);
			m *= 3;
		}

		return index;

	}

	int idxRight(Board board)
	{
		int index = 0;
		
		int m = 1;
		Point p = new Point(8, 0);
		for(int i=Board.BOARD_SIZE; i>0; i--)
		{
			p.y = i;
			index += m * (board.getColor(p) + 1);
			m *= 3;
		}

		return index;

	}

	int idxLeft(Board board)
	{
		int index = 0;
		
		int m = 1;
		Point p = new Point(1, 0);
		for(int i=Board.BOARD_SIZE; i>0; i--)
		{
			p.y = i;
			index += m * (board.getColor(p) + 1);
			m *= 3;
		}

		return index;

	}

	private ColorStorage countLiberty(Board board)
	{
		ColorStorage liberty = new ColorStorage();

		liberty.set(Disc.BLACK, 0); liberty.set(Disc.WHITE, 0); liberty.set(Disc.EMPTY, 0);

		Point p = new Point();

		for(int x = 1; x <= Board.BOARD_SIZE; x++)
		{
			p.x = x;
			for(int y = 1; y <= Board.BOARD_SIZE; y++)
			{
				p.y = y;
				int l = liberty.get(board.getColor(p));
				l += board.getLiberty(p);
				liberty.set(board.getColor(p), l);
			}
		}

		return liberty;

	}

	private int idxLine(int[] l)
	{
		return 3*(3*(3*(3*(3*(3*(3*(l[0]+1)+l[1]+1)+l[2]+1)+l[3]+1)+l[4]+1)+l[5]+1)+l[6]+1)+l[7]+1;
	}

}
