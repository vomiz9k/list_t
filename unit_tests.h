int test_insert_firstlast();
int test_insert_beforeafter();
int test_sort();
int test_delete();

int list_test()
{
    if (test_insert_firstlast() == 1)
        printf("INSERT FIRST/LAST TEST OK\n");
    else
        printf("INSERT FIRST/LAST TEST FAILED\n");

    if (test_insert_beforeafter() == 1)
        printf("INSERT BEFORE/AFTER TEST OK\n");
    else
        printf("INSERT BEFORE/AFTER TEST FAILED\n");

    if (test_sort() == 1)
        printf("LIST SORT TEST OK\n");
    else
        printf("LIST SORT TEST FAILED\n");

    if (test_delete() == 1)
        printf("ELEMENT DELETE TEST OK\n");
    else
        printf("ELEMENT DELETE TEST FAILED\n");
}

int test_insert_firstlast()
{
    list_t _list = {};
    list_construct(&_list);

    insert_first(&_list, 2458);

    if (_list.data[1] != 2458)
        return -1;

    insert_last(&_list, 3213);

    if (_list.data[2] != 3213)
        return -1;

    return 1;
}

int test_insert_beforeafter()
{
    list_t _list = {};
    list_construct(&_list);
    const int count = 10;

    for (int i = 0; i < count; ++i)
    {
        insert_after(&_list, i * 100, i);
    }

    for (int i = _list.head; i != -1; i = _list.next[i])
    {
        if (_list.data[i] != 100 * (i -1))
        {
            return -1;
        }
    }

    list_destruct(&_list);
    list_construct(&_list);

    insert_first(&_list, 0);
    for(int i = 1; i < count; ++i)
        insert_before(&_list, i, 1);

    for (int i = _list.head; i != -1; i = _list.next[i])
        if (_list.data[i] != i - 1)
        {
            printf("%d %d\n", _list.data[i], i);
            return -1;
        }

    return 1;
}

int test_sort()
{
    list_t _list = {};
    list_construct(&_list);
    insert_first(&_list, 10);
    insert_after(&_list, 20, 1);
    insert_before(&_list, 30, 2);
    insert_last(&_list, 40);  //expect 10->30->20->40
    list_sort(&_list);

    if (_list.data[1] != 10 || _list.data[2] != 30 || _list.data[3] != 20 || _list.data[4] != 40)
        return -1;

    return 1;
}

int test_delete()
{
    list_t _list = {};
    list_construct(&_list);
    insert_first(&_list, 10);
    insert_after(&_list, 20, 1);
    insert_before(&_list, 30, 2);
    insert_last(&_list, 40);  //10->30->20->40

    list_delete_el(&_list, 2);

    if (_list.data[1] != 10 || _list.data[3] != 30 || _list.data[4] != 40 || _list.data[2] != -228)
        return -1;

    return 1;
}

