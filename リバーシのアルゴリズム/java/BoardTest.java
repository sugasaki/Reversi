import java.io.*;

class BoardTest
{
	public static void main(String args[])
	{
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		ConsoleBoard board = new ConsoleBoard();

		while(true)
		{
			board.print();
			System.out.print("����" + board.countDisc(Disc.BLACK) + " ");
			System.out.print("����" + board.countDisc(Disc.WHITE) + " ");
			System.out.println("��}�X" + board.countDisc(Disc.EMPTY));
			System.out.println();

			System.out.print("�����͂��ĉ�����: ");
			Point p;
			String in;

			try
			{
				in = br.readLine();
			}
			catch(IOException e)
			{
				return;
			}
			
			if(in.equals("p"))
			{
				if(!board.pass())
				{
					System.out.println("�p�X�ł��܂���I");
				}
				continue;
			}
			
			if(in.equals("u"))
			{
				board.undo();
				continue;
			}

			try
			{
				p = new Point(in);
			}
			catch(IllegalArgumentException e)
			{
				System.out.println("���o�[�V�`���̎����͂��ĉ������I");
				continue;
			}
			
			if(board.move(p) == false)
			{
				System.out.println("�����ɂ͒u���܂���I");
				continue;
			}

			if(board.isGameOver())
			{
				System.out.println("----------------�Q�[���I��----------------");
				return;
			}
		}
	}
}
