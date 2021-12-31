/*
 *  Tic Tac Toe
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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct ticTacToe TicTacToe;

struct ticTacToe
{
    int8_t **v;
    size_t row;
    size_t col;
    size_t scoreX;
    size_t scoreO;
    char x;
    size_t y;
    _Bool p;
    char win;
};

TicTacToe *
create (size_t row, size_t col)
{
    TicTacToe *m = malloc (sizeof (TicTacToe));
    if (!m) exit (EXIT_FAILURE);
    m->row = row;
    m->col = col;
    m->win = 0;
    m->p = 0;
    m->x = 0;
    m->y = 0;
    m->scoreX = 0;
    m->scoreO = 0;
    m->v = malloc (row * sizeof (int8_t *));
    for (size_t i = 0; i < row; ++i)
        {
            m->v[i] = calloc (col, sizeof (int8_t));
        }
    return m;
}

void
print (TicTacToe *t)
{
    char an = 'a';

    printf ("  ");
    for (size_t i = 0; i < t->col; ++i, ++an)
        printf ("%c", an);
    puts ("");

    for (size_t i = 0; i < t->row; ++i)
        {
            printf ("%ld ", t->row - i);
            for (size_t j = 0; j < t->col; ++j)
                {
                    printf ("%c", *(*(t->v + i) + j) == 0
                            ? ' '
                            : *(*(t->v + i) + j) > 0 ? 'X' : 'O');
                }
            puts ("");
        }
}

char
is_win (TicTacToe *t)
{
    size_t hor = 0, ver = 0, r_diag = 0, l_diag = 0;
    char draw = 2;

    for (size_t i = 0; i < t->row; ++i)
        {
            r_diag += t->v[i][i], l_diag += t->v[i][(t->row - 1) - i];
            for (size_t j = 0; j < t->col; ++j)
                {
                    ver += t->v[i][j];
                    if (!t->v[i][j])
                        draw = 0;
                    if (j < t->row)
                        hor += t->v[j][i];
                }
            if (ver == t->col || ver == (t->col * -1) || hor == t->col
                    || hor == (t->col * -1))
                return 1;
            ver = hor = 0;
        }

    if (r_diag == t->row || r_diag == (t->row * -1) || l_diag == t->row
            || l_diag == (t->row * -1))
        return 1;

    return draw;
}

TicTacToe *
move (TicTacToe *t)
{
    size_t col = 0, row = t->y;

    row -= t->row;
    row *= -1;
    if (row < 0)
        row = 0;

    char a = 'a';
    for (; col < t->col; ++col, ++a)
        if (t->x == a)
            break;

    if (row > t->row || col > t->col || t->v[row][col])
        {
            fprintf (stderr, "invalid position\n");
            return NULL;
        }

    *(*(t->v + row) + col) = t->p ? 1 : -1;
    return t;
}

void
reset (TicTacToe **t)
{
    for (size_t i = 0; i < (*t)->row; ++i)
        for (size_t j = 0; j < (*t)->col; ++j)
            *(*((*t)->v + i) + j) = 0;
}

TicTacToe *
free_ttt (TicTacToe *t)
{
    for (size_t i = 0; i < t->row; ++i)
        free (t->v[i]);
    free(t);
    return NULL;
}

int
main (void)
{
    TicTacToe *t, *auxt;
    t = create (3, 3);

    while (1)
        {
            print (t);

            printf ("Player %c: ", t->p ? 'X' : 'O');
            scanf("%c%zu", &t->x, &t->y);
            getchar(); // remove \n from buffer

            auxt = move (t);
            if (!auxt)
                continue;
            t = auxt;

            t->win = is_win (t);
            if (t->win)
                {
                    if (t->win == 2)
                        {
                            printf ("DRAW! - X: %zu | O: %zu\n", t->scoreX, t->scoreO);
                        }
                    else
                        {
                            t->p ? t->scoreX++ : t->scoreO++;
                            printf ("%c WIN! - X: %zu | O: %zu\n", t->p ? 'X' : 'O', t->scoreX,
                                    t->scoreO);
                        }
                    reset (&t);
                    t->p = 1;
                }
            else
                {
                    t->p = !t->p;
                }
        }

    t = free_ttt (t);
    return 0;
}
