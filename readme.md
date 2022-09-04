# Universal menu

Данный модуль создать с целью упросить написание интерфейса для лабораторных работ по дисциплине "ТИСД"

Модуль работает под системами linux и windows(на маке не проверял, но должно работать. Естественно работает в WSL и MSYS)
Если работаете в CLion или в другой IDE которая эмулирует собственный терминал, то необходимо запускать программу во внешнем терминале.



### Использование

Попробуем создать простейшее меню


#### Добавление команды в меню

Итак, есть функция:

>**_Важно:_**  Функция должна соответствовать сигнатуре `void funcname()`.
> Функция не должна принимать или возвращать параметры. Функции с аргументами будут рассмотрены ниже.
```
void func()
{
    printf("Hello menu");
}
```
Изменим `main` подобным образом:
```
int main()
{
    //Создаем меню
    MENU menu = create_menu();
    
    //Добавляем команду в меню
    // Сигнатура: void add_command(меню, навзание функции(указатель на функцию), строка для отображения в меню)
    add_command(menu, func, "func");
    
    //Запускаем меню
    execute_menu(menu);

    //Освобождаем ресурсы
    free_menu(menu);
}
```

При запуске получим подобный вывод 
```
func
Exit
```
Взаимодействие с меню осуществляется с помощью стрелок `↓ ↑` и `Enter`
При выборе пункта `func` произойдет вызов функции, и вывод станет таким:
```
Hello menu
Back
```
для выхода из меню нажмите `Enter`, а после - выберите `Exit`

#### Добавление вложенного меню

Добавим еще одну функцию:
```
void foobuzz()
{
    printf("foobuzz\n");
}
```
Модифицируем `main`

```
int main()
{
    MENU menu = create_menu();

    add_command(menu, func, "func");

    //Создадим меню которое будет вложенным в основное
    MENU sub_menu = create_menu();
    //Добавим в него необходимые команды
    add_command(sub_menu, foobuzz, "foobuzz");
    
    //"Вкладываем" sub_menu в menu
    add_sub_menu(menu, sub_menu, "foo_menu");

    execute_menu(menu);

    free_menu(menu);
}
```

#### Добавление команды, которая принимает аргументы

Представим что у нас есть функция которая должна принимать некоторые аргументы:
```
void foo_w_data(int a, int b, int c)
{
    printf("%d\n", a);
    printf("%d\n", b);
    printf("%d\n", c);
}
```

Эта функция не подходит по сигнатуре для функции `add_command`
Для функций, которым необходимо передать какие-то аргументы, следует использовать функцию

`void add_command_with_data(MENU menu, void(*func)(void *), const char *description, void *data);`

Но как `int a, int b, int c` превратить в `void* data` ?

Для этого необходимо перейти в файл `args.h` и прописать следующее: 

`ARG_STRUCT(foo_w_data, int, a, int, b, int, c);`


Данный макрос создает "упаковщик" аргументов:
   - Создается структура
     ```
     struct foo_w_data_args
     {
        int a;
        int b;
        int c;
     }
     ```
   - Создается функция `pack`

        ```    
         struct foo_w_data_args * foo_w_data_pack(int a, int b, int c)                   
         {                                                                            
            struct foo_w_data_args* tmp = malloc(sizeof(struct foo_w_data_args));   
            tmp->a = a;
            tmp->b = b;
            tmp->c = c;
            return tmp;     
         }
        ```


> **_Важно_**: Обратите внимание, что типы данных и имена переменных пишутся через запятую
> Первым аргументом является название функции для которой мы создаем "упаковщик". Далее идут аргументы функции
> 
> Ваша функция не должна принимать более 5 аргументов. При необходимости увеличить количество передаваемых параметров, можете попробовать понять что происходит в `macro_magic.h` и расширить их до необходимого уровня
> 
> **_Важно_**: для имени упаковщика рекомендуется использовать имя функции для которой и создается упаковщик

Чтобы воспользоваться "упаковкой", необходимо вызвать функцию, название которой состоит из `названия функции + _pack`,
и передать необходимые аргументы. В данном случае `foo_w_data_pack(1, 2, 3)`

`add_command_with_data(menu, foo_w_data, "wdata", foo_w_data_pack(1, 2, 3));`

Но `foo_w_data` все еще не подходит по сигнатуре. Перепишем ее подобным образом:
```
//Сигнатура void func(void *data)
void foo_w_data(void *data)
{
    //Чтобы вытянуть из data необходимые аргументы, нужно воспользоваться следующей инструкцией.
    //Первый аргумент - название текущей функции(или название упаковщика)
    //Второй аргумент - имя аргумента для данных
    UNPACK_STRUCT(foo_w_data, data);
    
    
    //Теперь мы можем получить доступ к  a, b, c через args->arg_name
    printf("%d\n", args->a);
    printf("%d\n", args->b);
    printf("%d\n", args->c);
}
```

