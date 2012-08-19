import java.io.*;

interface Player
{
	public void onTurn(Board board) throws Exception;
}

class UndoException extends Exception
{
}

class ExitException extends Exception
{
}

class GameOverException extends Exception
{
}

class HumanPlayer implements Player
{
	public void onTurn(Board board) throws Exception
	{
		if(board.getMovablePos().isEmpty())
		{
			// �p�X
			System.out.println("���Ȃ��̓p�X�ł��B");
			board.pass();
			return;
		}

		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

		while(true)
		{
			System.out.print("���\"f5\"�̂悤�ɓ��́A��������(U:���/X:�I��)����͂��Ă�������:");
			String in = br.readLine();

			if(in.equalsIgnoreCase("U")) throw new UndoException();
			
			if(in.equalsIgnoreCase("X")) throw new ExitException();
			
			Point p;
			try{
				p = new Point(in);
			}
			catch(IllegalArgumentException e)
			{
				System.out.println("�������`���œ��͂��Ă��������I");
				continue;
			}
			
			if(!board.move(p))
			{
				System.out.println("�����ɂ͒u���܂���I");
				continue;
			}
			
			if(board.isGameOver()) throw new GameOverException();
			
			break;
		}
	}
}

class AIPlayer implements Player
{

	private AI Ai = null;

	public AIPlayer()
	{
		Ai = new AlphaBetaAI();
	}

	public void onTurn(Board board) throws GameOverException
	{
		System.out.print("�R���s���[�^���v�l��...");
		Ai.move(board);
		System.out.println("����");
		if(board.isGameOver()) throw new GameOverException();
	}
};

class ReversiGame
{
	public static void main(String[] args)
	{
		Player[] player = new Player[2];
		int current_player = 0;
		ConsoleBoard board = new ConsoleBoard();
		boolean reverse = false;
		
		if(args.length > 0)
		{
			// �R�}���h���C���I�v�V���� -r ���^������ƃR���s���[�^���ɂ���
			if(args[0].equals("-r")) reverse = true;
		}

		// ���E���̐ݒ�
		if(reverse)
		{
			player[0] = new AIPlayer();
			player[1] = new HumanPlayer();
		}
		else
		{
			player[0] = new HumanPlayer();
			player[1] = new AIPlayer();
		}

		while(true)
		{
			board.print();

			try{
				player[current_player].onTurn(board);
			}
			catch(UndoException e)
			{
				do
				{
					board.undo(); board.undo();
				} while(board.getMovablePos().isEmpty());
				continue;
			}
			catch(ExitException e)
			{
				return;
			}
			catch(GameOverException e)
			{
				System.out.println("�Q�[���I��");
				System.out.print("����" + board.countDisc(Disc.BLACK) + " ");
				System.out.println("����" + board.countDisc(Disc.WHITE));

				return;
			}
			catch(Exception e)
			{
				// �\�����Ȃ���O
				System.out.println("Unexpected exception: " + e);
				return;
			}

			// �v���C���[�̌��
			current_player = ++current_player % 2;
		}

	}
}
