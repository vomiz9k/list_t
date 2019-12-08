typedef int list_type;

enum ERR_CODES
{
    LIST_OK = 0,
    LIST_SIZE_ERR,
    LIST_STRUCT_ERR,
    LIST_NULLPTR_ERR
};

struct list_t
{
    list_type* data;
    int* prev;
    int* next;
    int head;
    int tail;
    int next_free;
    int size;
    int max_size;
    bool sorted;
};

int take_el(list_t* _list, int number);
int list_text_dump(list_t* _list, int err_int);
int list_sort(list_t* _list);
int list_construct(list_t* _list);
int list_destruct(list_t* _list);

int element_exists(list_t* _list, int number);
int list_dump(list_t* _list, int err_int);

int insert_after(list_t* _list, list_type value, int number);
int insert_before(list_t* _list, list_type value, int number);
int insert_first(list_t* _list, list_type value);
int insert_last(list_t* _list, list_type value);

int list_delete_el(list_t* _list, int num);
int list_realloc(list_t* _list, float param);
int list_verify(list_t* _list);
