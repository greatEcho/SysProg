#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

extern int32_t f(int32_t **arr);

int main()
{
    int32_t **arr;  // указатель на указатель на строку элементов
    int i, j, n, m;
    n = 2;
    m = 2;
    // Выделение памяти под указатели на строки
    arr = (int32_t**)malloc(n * sizeof(int32_t*));
    // Ввод элементов массива
    for (i = 0; i<n; i++)  // цикл по строкам
    {
        // Выделение памяти под хранение строк
        arr[i] = (int32_t*)malloc(m * sizeof(int32_t));
        for (j = 0; j<m; j++)  // цикл по столбцам
        {
        arr[i][j] = 2;
        }
    }
    // Вывод элементов массива
    for (i = 0; i < n; i++)  // цикл по строкам
    {
        for (j = 0; j < m; j++)  // цикл по столбцам
        {
        printf("%4ld ", arr[i][j]); // кол-во знаков под элемент массива
        }
        printf("\n");
    }
    printf("%" PRId32 "\n", f(arr));
    // Очистка памяти
    for (i = 0; i < n; i++)  // цикл по строкам
        free(arr[i]);   // освобождение памяти под строку
    free(arr);
    return 0;
}
