/*
 *  C Tic Tac Toe
 *  Copyright (c) 2021 Leonardo Zamboni
 *
 *  this program is free software: you can redistribute it and/or modify
 *  it under the terms of the gnu general public license as published by
 *  the free software foundation, either version 3 of the license, or
 *  (at your option) any later version.
 *
 *  this program is distributed in the hope that it will be useful,
 *  but without any warranty; without even the implied warranty of
 *  merchantability or fitness for a particular purpose.  see the
 *  gnu general public license for more details.
 *
 *  you should have received a copy of the gnu general public license
 *  along with this program.  if not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define COLS 3
#define ROW 3

void
print_table (int8_t **t, size_t row)
{
  if (!row)
    return;

  printf ("%ld ", row);
  for (size_t i = 0; i < COLS; ++i)
    printf ("%c", *(*t + i) == 0 ? ' ' : (*t)[i] > 0 ? 'X' : 'O');

  puts ("");

  t++, row--;

  print_table (t, row);
}

void
print_an (void)
{
  char an = 'a';

  printf ("  ");
  for (size_t i = 0; i < COLS; i++, an++)
    printf ("%c", an);
  puts ("");
}

uint8_t
is_win (int8_t **t)
{
  if (!t)
    return 0;
  size_t hor = 0, ver = 0, r_diag = 0, l_diag = 0;

  _Bool velha = 0;

  for (size_t i = 0; i < ROW; i++)
    {
      for (size_t j = 0; j < COLS; j++)
        {
          ver += t[i][j];
          if (j < ROW)
            hor += t[j][i];
          if (t[i][j])
            velha = 1;
        }
      if (ver == COLS || ver == (COLS * -1) || hor == COLS
          || hor == (COLS * -1))
        return 1;
      ver = hor = 0;
    }

  if (!velha)
    return 2;

  for (size_t i = 0; i < ROW; i++)
    r_diag += t[i][i], l_diag += t[i][ROW - i];

  return r_diag == ROW || r_diag == (ROW * -1) || l_diag == ROW
         || l_diag == (ROW * -1);
}

int8_t **
set_move (_Bool p, int8_t **t, char *i)
{
  int16_t col = 0, row = i[1] - '0';

  char a = 'a';

  row -= ROW;
  row *= -1;
  if (row < 0)
    row = 0;

  for (; col < COLS; col++, a++)
    if (i[0] == a)
      break;

  if (t[row][col])
    {
      fprintf (stderr, "invalid position\n");
      return NULL;
    }

  t[row][col] = p ? 1 : -1;
  return t;
}

int8_t **
reset (int8_t **t)
{
  for (size_t i = 0; i < ROW; i++)
    for (size_t j = 0; j < COLS; j++)
      t[i][j] = 0;
  return t;
}

int
main (void)
{
  int8_t **table, **auxtable;
  uint8_t win = 0;

  int x_score = 0, o_score = 0;

  _Bool player = 1;
  char input[3];

  table = (int8_t **)malloc (ROW * sizeof (int8_t *));
  for (size_t i = 0; i < ROW; i++)
    table[i] = (int8_t *)calloc (COLS, sizeof (int8_t));

  while (1)
    {
      print_an ();
      print_table (table, ROW);

      printf ("%c player: ", player ? 'X' : 'O');
      scanf ("%2s[a-z0-9]", input);

      auxtable = set_move (player, table, input);
      win = is_win (auxtable);
      if (win)
        {
          if (win == 2)
            {
              printf ("DRAW! X %d vs %d O\n", x_score, o_score);
            }
          else
            {
              player ? x_score++ : o_score++;
              printf ("%c WIN! X %d vs %d O\n", player ? 'X' : 'O', x_score,
                      o_score);
            }
          table = reset (table);
          player = 1;
        }
      else
        {
          table = auxtable, player = !player;
        }
    }

  free (table);
  return 0;
}
