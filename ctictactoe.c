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

typedef struct table Table;

struct table
{
  uint16_t row;
  uint16_t col;
  int8_t **v;
};

Table *
create (uint16_t row, uint16_t col)
{
  Table *m = (Table *)malloc (sizeof (Table));
  if (!m)
    exit (EXIT_FAILURE);
  m->row = row;
  m->col = col;
  m->v = (int8_t **)malloc (row * sizeof (int8_t *));
  for (size_t i = 0; i < row; ++i)
    {
      m->v[i] = (int8_t *)calloc (col, sizeof (int8_t));
    }
  return m;
}

void
output (Table *t)
{
  char an = 'a';

  printf ("  ");
  for (size_t i = 0; i < t->col; i++, an++)
    printf ("%c", an);
  puts ("");

  for (size_t i = 0; i < t->row; ++i)
    {
      printf ("%d ", t->row - i);
      for (size_t j = 0; j < t->col; ++j)
        {
          printf ("%c", *(*(t->v + i) + j) == 0
                            ? ' '
                            : (*(t->v + i))[j] > 0 ? 'X' : 'O');
        }
      puts ("");
    }
}

uint8_t
is_win (Table *t)
{
  size_t hor = 0, ver = 0, r_diag = 0, l_diag = 0;
  uint8_t draw = 2;

  for (size_t i = 0; i < t->row; i++)
    {
      for (size_t j = 0; j < t->col; j++)
        {
          if (!t->v[i][j])
            draw = 0;
          ver += t->v[i][j];
          if (j < t->row)
            hor += t->v[j][i];
        }
      if (ver == t->col || ver == (t->col * -1) || hor == t->col
          || hor == (t->col * -1))
        return 1;
      ver = hor = 0;
    }

  for (size_t i = 0; i < t->row; i++)
    r_diag += t->v[i][i], l_diag += t->v[i][(t->row - 1) - i];

  if (r_diag == t->row || r_diag == (t->row * -1) || l_diag == t->row
      || l_diag == (t->row * -1))
    return 1;
  return draw;
}

Table *
move (_Bool p, Table *t, char *i)
{
  int16_t col = 0, row = i[1] - '0';

  char a = 'a';

  row -= t->row;
  row *= -1;
  if (row < 0)
    row = 0;

  for (; col < t->col; col++, a++)
    if (i[0] == a)
      break;

  if (t->v[row][col])
    {
      fprintf (stderr, "invalid position\n");
      return t;
    }

  t->v[row][col] = p ? 1 : -1;
  return t;
}

Table *
reset (Table *t)
{
  for (size_t i = 0; i < t->row; i++)
    for (size_t j = 0; j < t->col; j++)
      t->v[i][j] = 0;
  return t;
}

int
main (void)
{
  Table *t, *auxt;
  uint8_t win = 0;
  uint16_t x_score = 0, o_score = 0;
  _Bool player = 1;
  char input[3];

  t = create (3, 3);

  while (1)
    {
      output (t);

      printf ("Player %c: ", player ? 'X' : 'O');
      scanf ("%2s[a-z0-9]", input);

      auxt = move (player, t, input);
      if (!auxt)
        continue;
      t = auxt;

      win = is_win (t);
      if (win)
        {
          if (win == 2)
            {
              printf ("DRAW! - X: %d | O: %d\n", x_score, o_score);
            }
          else
            {
              player ? x_score++ : o_score++;
              printf ("%c WIN! - X: %d | O: %d\n", player ? 'X' : 'O', x_score,
                      o_score);
            }
          t = reset (t);
          player = 1;
        }
      else
        {
          player = !player;
        }
    }

  free (t);
  return 0;
}
