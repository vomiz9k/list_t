#include <assert.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include "list_t.h"

int POISON = -228;
float UPSIZE = 2;

int list_verify(list_t* _list)
{
    if (!(_list && _list->data && _list->next && _list->prev))
    {
        list_dump(_list, LIST_NULLPTR_ERR);
        return LIST_NULLPTR_ERR;
    }

    if (_list->max_size <= _list->size)
    {
        list_dump(_list, LIST_SIZE_ERR);
        return LIST_SIZE_ERR;
    }

    bool * reached = (bool*)calloc(_list->max_size, sizeof(bool));

    for (int i = 1; i < _list->max_size; ++i)
    {
        reached[i] = false;
    }

    int next = _list->head;

    while (next != -1)
    {
        if (reached[next] == true)
        {
            list_dump(_list, LIST_STRUCT_ERR);
            free(reached);
            return LIST_STRUCT_ERR;
        }
        reached[next] = true;
        next = _list->next[next];
    }

    next = _list->next_free;
    while (next != -1)
    {
        if (reached[next] == true)
        {
            list_dump(_list, LIST_STRUCT_ERR);
            free(reached);
            return LIST_STRUCT_ERR;
        }
        reached[next] = true;
        next = _list->next[next];
    }
    free(reached);
    return LIST_OK;
}

int list_text_dump(list_t* _list, int err_int)
{
    switch(err_int)
    {
        case LIST_STRUCT_ERR:
            printf("ERROR IN LIST STRUCTURE. CHECK .PNG FILE TO SEE WHAT IS WRONG.\n");
        break;

        case LIST_NULLPTR_ERR:
            printf("NULLPTR ERROR.\n");
            printf("\tLIST PTR :: %d\n", _list);
            if (_list)
            {
                printf("\tLIST NEXT PTR :: %d\n", _list->next);
                printf("\tLIST PREV PTR :: %d\n", _list->prev);
                printf("\tLIST DATA PTR :: %d\n", _list->data);
            }
            return 0;
        break;

        case LIST_SIZE_ERR:
            printf("LIST SIZE ERROR!!!\n");
            printf("Dumping...\n\n");
        break;
    }
    for (int i = 0; i < _list->max_size; ++i)
        printf("NUMBER :: %-10d DATA :: %-10d NEXT :: %-10d PREV :: %-10d\n", i, _list->data[i], _list->next[i], _list->prev[i]);
    printf("\nHEAD == %d; TAIL == %d;\nSIZE == %d; MAX_SIZE == %d;\nNEXT_FREE == %d;\n\n",_list->head, _list->tail, _list->size, _list->max_size, _list->next_free);
    printf("\n\n");

}



int list_construct(list_t* _list)
{
    assert(_list);
    memset(_list, 0, sizeof(list_t));

    _list->size      = 0;
    _list->max_size  = 2;
    _list->next_free = 1;
    _list->head      = -1;
    _list->tail      = -1;
    _list->sorted    = false;;

    _list->data = (list_type*) calloc(sizeof(list_type), _list->max_size);
    _list->next = (int*)       calloc(sizeof(int),       _list->max_size);
    _list->prev = (int*)       calloc(sizeof(int),       _list->max_size);

    _list->next[0] = -1;
    _list->prev[0] = -1;
    _list->data[0] = -1;

    _list->next[1] = -1;
    _list->prev[1] = -1;
    _list->data[1] = POISON;
}

int list_destruct(list_t* _list)
{
    free(_list->next);
    free(_list->prev);
    free(_list->data);
}

int insert_after(list_t* _list, list_type value, int number)
{
    if (list_verify(_list) != 0)
    {
        system("pause");
    }

    if(_list->sorted && number != _list->head)
        _list->sorted = false;

    if (_list->next_free == -1)
        list_realloc(_list, UPSIZE);

    if (!element_exists(_list, number))
    {
        printf("ERROR! TRYING TO INSERT AFTER EMPTY ELEMENT!\n");
    }

    int curr_pos = _list->next_free;

    if (_list->next[curr_pos] > 0)
        _list->prev[_list->next[curr_pos]] = -1;

    _list->next_free = _list->next[curr_pos];

    _list->data[curr_pos] = value;

    if (number != 0)
    {
        _list->prev[curr_pos] = number;
        _list->next[curr_pos] = _list->next[number];

        if (_list->next[number] > 0)
            _list->prev[_list->next[number]] = curr_pos;

        _list->next[number]   = curr_pos;
    }
    else
    {
        _list->prev[curr_pos] = -1;

        if (curr_pos != _list->head)
        {
            if (_list->head > 0)
                _list->prev[_list->head] = curr_pos;
            else
                _list->tail = curr_pos;

            _list->next[curr_pos] = _list->head;
        }

        _list->head = curr_pos;
    }

    if (number == _list->tail)
    {
        _list->tail = curr_pos;
    }

    _list->size++;

    if (list_verify(_list) != 0)
    {
        system("pause");
    }
}

int element_exists(list_t* _list, int number)
{
    if (number == 0)
        return 1;

    int next_el = _list->head;

    while (next_el != number && next_el > 0)
        next_el = _list->next[next_el];

    if (next_el < 0)
        return 0;

    return 1;
}

int insert_before(list_t* _list, list_type value, int number)
{
    insert_after(_list, value, number - 1);
}

int list_delete_el(list_t* _list, int num)
{
    if (num < 1)
        return -1;

    if (num != _list->tail && num != _list->head)
    {
        _list->prev[_list->next[num]] = _list->prev[num];
        _list->next[_list->prev[num]] = _list->next[num];
    }

    if (num == _list->head)
    {
        _list->head = _list->next[num];
        _list->prev[_list->head] = -1;

    }
    if (num == _list->tail)
    {
        _list->tail = _list->prev[num];
        _list->next[_list->tail] = -1;
    }


    if (_list->next_free > 0)
        _list->prev[_list->next_free] = num;

    _list->data[num] = POISON;
    _list->next[num] = _list->next_free;
    _list->prev[num] = -1;
    _list->next_free = num;

    _list->size--;
}


int list_realloc(list_t* _list, float param)
{
    if (list_verify(_list) != 0)
    {
        system("pause");
    }

    size_t new_size =(_list->max_size) * param - 1;

    _list->data = (list_type*) realloc (_list->data, sizeof(list_type) * new_size);
    _list->next = (int*)       realloc (_list->next, sizeof(int) * new_size);
    _list->prev = (int*)       realloc (_list->prev, sizeof(int) * new_size);

    if (list_verify(_list) != 0)
    {
        system("pause");
    }

    if (param > 1)
    {
        _list->next_free = _list->max_size;

        _list->prev[_list->next_free] = -1;

        _list->next[_list->next_free] = _list->next_free + 1;

        _list->data[_list->next_free] = POISON;

        for (int i = _list->next_free; i < new_size; ++i)
        {
            _list->next[i] = i + 1;
            _list->prev[i] = i - 1;
            _list->data[i] = POISON;
        }

        if (new_size - 1 != _list->next_free)
            _list->prev[new_size - 1] = new_size - 2;

        _list->next[new_size - 1] = -1;
        _list->data[new_size - 1] = POISON;
    }
    _list->max_size = new_size;
}

int take_el(list_t* _list, int number)
{
    if (_list->sorted == true)
        return _list->data[number];

    list_sort(_list);
    return _list->data[number];
}


int insert_first(list_t* _list, list_type value)
{
    insert_after(_list, value, 0);
}

int insert_last(list_t* _list, list_type value)
{
    insert_after(_list, value, _list->tail);
}

int list_sort(list_t* _list)
{
    if (list_verify(_list) != 0)
    {
        system("pause");
    }

    int next = _list->head;

    for (int i = 0; next != -1; ++i)
    {
       _list->prev[i + 1] = _list->data[next];
       next = _list->next[next];
    }

    next = _list->head;
    for (int i = 0; next != -1; ++i)
    {
        _list->data[i + 1] = _list->prev[i + 1];
        next = _list->next[next];
    }

    _list->next = (int*) realloc(_list->next, sizeof(int) * (_list->size + 1));
    _list->prev = (int*) realloc(_list->prev, sizeof(int) * (_list->size + 1));

    for (int i = 1; i <= _list->size; ++i)
    {
        _list->next[i] = i + 1;
        _list->prev[i] = i - 1;
    }

    _list->prev[1] = -1;
    _list->next[_list->size] = -1;

    _list->next_free = -1;
    _list->head = 1;

    _list->tail = _list->size;
    _list->max_size = _list->size + 1;

    _list->sorted = true;

    if (list_verify(_list) != 0)
    {
       system("pause");
    }
}

int list_dump(list_t* _list, int err_int)
{
    list_text_dump(_list, err_int);
    FILE* fout = fopen("goutput.gv", "w");
    fprintf(fout, "digraph {\nnode[shape=Mrecord, fontname=\"Wasted Sans\"];\n");
    fprintf(fout, "rankdir=LR;\n");
    fprintf(fout, "%d [xlabel=\"%d\", label = \"<f0> HEAD |<f1> DATA :: %d |<f2> NEXT :: %d\"];\n",_list->head, _list->head, _list->data[_list->head], _list->next[_list->head]);

    int prev_node = _list->head;
    int current_node = _list->next[prev_node];

    fprintf(fout, "%d:<f0> -> %d:<f0>;\n", current_node, prev_node);
    int i = 0;
    while (_list->next[current_node] != -1)
    {
        fprintf(fout, "%d [xlabel=\"%d\", label = \"<f0> PREV :: %d|<f1> DATA :: %d |<f2> NEXT :: %d\"];\n",current_node,  current_node, _list->prev[current_node], _list->data[current_node], _list->next[current_node]);
        fprintf(fout, "%d:<f2> -> %d:<f2>;\n", prev_node, current_node);

        prev_node = current_node;
        current_node = _list->next[current_node];

        fprintf(fout, "%d:<f0> -> %d:<f0>;\n", current_node, prev_node);
    }
    fprintf(fout, "%d [xlabel=\"%d\", label = \"<f0> PREV :: %d|<f1> DATA :: %d |<f2> TAIL \"];\n", _list->tail,  _list->tail, _list->prev[_list->tail], _list->data[_list->tail]);
    fprintf(fout, "%d:<f2> -> %d:<f2>;\n", _list->prev[_list->tail], _list->tail);
    fprintf(fout, "}");
    fclose(fout);

    system("dot -Tpng goutput.gv -o foutput.png");
    system("foutput.png");
}
