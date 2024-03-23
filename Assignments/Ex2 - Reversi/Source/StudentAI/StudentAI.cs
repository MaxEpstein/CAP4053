using GameAI.GamePlaying.Core;
using System.Collections.Generic;
using System.Data;
using GameAI.GamePlaying.ExampleAI;
using System.Security.Principal;

namespace GameAI.GamePlaying
{
    public class StudentAI : Behavior
    {
        public StudentAI() { }

        public ComputerMove Run(int _color, Board _board, int _lookAheadDepth)
        {
            ComputerMove bestMove = null;
            Board newState = new Board();

            List<ComputerMove> moves = new List<ComputerMove>();

            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++) {
                    if (_board.IsValidMove(_color, row, col))
                    {
                        ComputerMove validMove = new ComputerMove(row, col);
                        moves.Add(validMove);
                    }
                }
            }

            foreach (ComputerMove move in moves)
            {
                newState.Copy(_board);
                newState.MakeMove(_color, move.row, move.column);

                if (newState.IsTerminalState() || _lookAheadDepth == 0)
                {
                    move.rank = Evaluate(newState);
                }  
                else
                {
                    // go look through next iteration and therefore decrease the depth by 1
                    move.rank = Run(get_next_player(_color, newState), newState, _lookAheadDepth - 1).rank;
                }

                if (bestMove == null || better(_color, move, bestMove))
                {
                    bestMove = move;
                }
            }

            return bestMove;
        }

        public int get_next_player(int color, Board board)
        {
            // if the other player has any valid moves
            if (board.HasAnyValidMove(color * -1))
            {
                return color * -1;
            }

            else
                return color;
        }

        public bool better(int color, ComputerMove toMake, ComputerMove best)
        {
            if (color == 1 && toMake.rank > best.rank)
                return true;
            else if (color == -1 && toMake.rank < best.rank)
                return true;
            return false;
        }

        public int Evaluate(Board board)
        {
            int total = 0;

            for (int row = 0; row < 8; row++)
            {
                for (int col = 0; col < 8; col++)
                {
                    int tile = board.GetTile(row, col);

                    // if it is a corner tile
                    if ((row == 0 && col == 0) || (row == 7 && col == 7) || (row == 0 && col == 7) || (row == 7 && col == 0))
                    {
                        tile *= 100;
                    }

                    // otherwise check if it is an edge tile
                    else if (row == 0 || row == 7 || col == 0 || col == 7)
                    {
                        tile *= 10;
                    }

                    total += tile;
                }
            }

            return total;
        }
    }
}
